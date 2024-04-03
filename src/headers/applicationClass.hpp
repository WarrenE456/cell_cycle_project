#pragma once
#include "../../include/glad/glad.h"
#include "../../include/GLFW/glfw3.h"

class Application {
private:
	unsigned int width, height;
	GLFWwindow* window;
	void Init(GLuint glMajorVersion, GLuint glMinorVersion);
	void Terminate();
public:
	Application(GLuint glMajorVersion, GLuint glMinorVersion, unsigned int width, unsigned int height);
	~Application();
	int Run();
};
