#include "headers/applicationClass.hpp"

int main() {
	unsigned int
	height = 1000,
	width = 1000,
	glMinorVersion = 3,
	glMajorVersion = 3;

	Application app(glMajorVersion, glMinorVersion, height, width);
	int status = app.Run();

	return status;
}
