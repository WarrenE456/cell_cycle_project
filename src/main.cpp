#include "headers/applicationClass.hpp"

int main() {
	unsigned int
	height = 800,
	width = 800,
	glMinorVersion = 3,
	glMajorVersion = 3;

	Application app(glMajorVersion, glMinorVersion, height, width);
	int status = app.Run();

	return status;
}
