#include "headers/openGLdebug.hpp"
#include <stdexcept>
#include <string>
#include "headers/applicationClass.hpp"
#include "../include/GLFW/glfw3.h"

// Path to source directory
const std::string SOURCE_DIRECTORY = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\"));

void Application::Init(GLuint glMajorVersion, GLuint glMinorVersion) {
    // Initalize GLFW
    glfwInit();

    // Use CORE profile, version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    this->window = glfwCreateWindow(this->width, this->height, "SPH Simulation", NULL, NULL);
    if (!window) {
        throw std::runtime_error("Failed to create window.");
    }

    glfwMakeContextCurrent(window);

    // Load glad
    gladLoadGL();

    // Define viewport
    GLCALL(glViewport(0,0, this->width, this->height));
    GLCALL(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));

    // Enable blending
    GLCALL(glEnable(GL_BLEND));

    // Set the blend function
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Application::Application(GLuint glMajorVersion, GLuint glMinorVersion, unsigned int width, unsigned int height)
: width(width), height(height) {
    this->Init(glMajorVersion, glMinorVersion);
}

void Application::Terminate() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

Application::~Application() {
    this->Terminate();
}

int Application::Run() {
	std::cout << "Hello, world\n";
	return 0;
}

