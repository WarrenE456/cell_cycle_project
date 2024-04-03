# pragma once

#include "../../include/glad/glad.h"
#include <sstream>
#include <exception>
#include <iostream>
#include <string>

// OpenGL error class for throwing exceptions
// relating to OpenGL problems
class OpenGLError : public std::exception {
    std::string message;
public:
    OpenGLError(GLenum error, const char* function, const char* file, int line) {
        std::stringstream ss;
        ss << "OpenGL Error: 0x" << std::hex << error << " at " << function << " in " << file << " line " << line;
        message = ss.str();
    }
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

#ifndef NDEBUG
    #define GLCALL(x) \
        do { \
            x; \
            glCheckError(#x, __FILE__, __LINE__); \
        } while(0)
#else
    #define GLCALL(x) x
#endif


// Function to check for OpenGL errors
inline void glCheckError(const char* function, const char* file, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        throw OpenGLError(error, function, file, line);
    }
}

// Macro for checking for shader compilation failures
#ifndef NDEBUG
    # define CHECK_SHADER_COMPILE_STATUS(shader, shader_type)\
        do {\
            int success;\
            char infoLog[512];\
            GLCALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));\
            if (!success) {\
                GLCALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));\
                std::cerr << shader_type << " shader compilation failed:\n" << infoLog << "\n";\
            }\
        } while(0)

#else
    # define CHECK_SHADER_COMPILE_STATUS(shader, shader_type)
#endif
