#include "headers/openGLdebug.hpp"
#include "../include/GLFW/glfw3.h"
#include <stdexcept>
#include "headers/applicationClass.hpp"
#include "headers/cellClass.hpp"

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
    Cells cells(1, 0.1);
    
    while (!glfwWindowShouldClose(this->window)) {

        // Clear screen
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));

        // Draw particles to screen
        cells.Draw();

        // Swap buffers and pole events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

