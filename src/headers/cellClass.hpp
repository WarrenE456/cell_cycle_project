# pragma once

#include "../headers/shaderClass.hpp"
#include "../headers/shaderClass.hpp"
#include "../../include/glad/glad.h"
#include <vector>

class Cells {
	GLuint N; // Number of cells
	GLfloat r; // Largest cell radius
	Shader shaderProgram;	
	std::vector<float> pos; // Position of each particle
	std::vector<GLfloat> verts; // Vertex data
	std::vector<GLuint> indices; // Index data
	GLuint VAO, VBO, EBO;

	void Init();
	void Terminate();
public:
	# define X 0
	# define Y 1
	float* GetPos(unsigned int dimension, unsigned int index);
	Cells(GLuint N, GLfloat r);
	~Cells();
	void Draw();
};
