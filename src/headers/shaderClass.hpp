#pragma once

#include "../../include/glad/glad.h"
#include <exception>
#include <string>

// Class for thrownig file releated exceptions
class FileError : public std::exception {
public:
    std::string message;
    FileError(const char* message); 
    virtual const char* what() const throw();
};

// Parses file into a string
std::string parse_file(const char* filepath);

// Class for encapsulating shaders
class Shader {
    void Delete();
public:
    GLuint ID;
    Shader(const char* vertexFilePath, const char* fragmentFilePath);
    ~Shader();
    void Activate();
};
