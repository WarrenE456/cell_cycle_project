# pragma once

#include "../headers/shaderClass.hpp"
#include "../headers/shaderClass.hpp"
#include "../../include/glad/glad.h"
#include <vector>

// Used for the dimension param of Cells:GetPos method
# define X 0
# define Y 1
# define R 2

class Cells {
	GLuint N; // Number of cells
	GLfloat r; // Largest cell radius
	Shader shaderProgram;	
	std::vector<float> pos; // Position of each particle
	std::vector<float> vel; // Velocity of each particle
	std::vector<GLfloat> verts; // Vertex data
	std::vector<GLuint> indices; // Index data
	GLuint VAO, VBO, EBO;

	float& GetPos(unsigned int dimension, unsigned int index);
	float& GetVel(unsigned int dimension, unsigned int index);
	float& GetVerts(unsigned int dimension, unsigned int vertex, unsigned int index);
	void Init();
	void Terminate();
public:
	void Draw();
	void Update(float deltaSeconds);
	void UpdateBufferData();
	Cells(GLuint N, GLfloat r);
	~Cells();
};
