#include "headers/openGLdebug.hpp"
#include "../include/glad/glad.h"
#include "headers/shaderClass.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

// Constructor takes the message and what() spits out the message
FileError::FileError(const char* message) : message(message) {}
const char* FileError::what() const throw() {
    return message.c_str();
}

// Parse file into string
std::string parse_file(const char *filepath) {
    // Open file
    std::ifstream file(filepath, std::ios::binary);
    
    // Check if file is valid
    if (!file) {
        std::string message = "Failed to open file ";
        message += filepath;
        message += ".";
        throw FileError(message.c_str());
    }
    
    // Read file into string
    std::string txt;
    file.seekg(0, std::ios::end);
    txt.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&txt[0], txt.size());
    file.close();
    
    // Return contents of the file
    return txt;
}


// Create the shaders from the path of the shader files
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
    // Parse shaders into strings
    std::string vertexCode = parse_file(vertexFilePath);
    std::string fragmentCode = parse_file(fragmentFilePath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();
    
    //  Compile shaders and check compilation status 

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLCALL(glShaderSource(vertexShader, 1, &vertexSource, NULL));
    GLCALL(glCompileShader(vertexShader));
    CHECK_SHADER_COMPILE_STATUS(vertexShader, "Vertex");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    GLCALL(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
    GLCALL(glCompileShader(fragmentShader));
    CHECK_SHADER_COMPILE_STATUS(fragmentShader, "Fragment");

    //  Create shader program and attach shaders 

    ID = glCreateProgram();
    GLCALL(glAttachShader(ID, vertexShader));
    GLCALL(glAttachShader(ID, fragmentShader));
    GLCALL(glLinkProgram(ID));

    // Delete shaders 
    GLCALL(glDeleteShader(vertexShader));
    GLCALL(glDeleteShader(fragmentShader));
}

void Shader::Activate() {
    GLCALL(glUseProgram(ID));
}

void Shader::Delete() {
    GLCALL(glDeleteProgram(ID));
}

Shader::~Shader() {
    this->Delete();
}
