# pragma once

#include "../headers/shaderClass.hpp"
#include "../headers/shaderClass.hpp"
#include "../../include/glad/glad.h"
#include <vector>

// Used for the dimension param of Cells:GetPos method
# define X 0
# define Y 1
# define R 2
# define S 3

class Cells {
	GLuint N; // Number of cells
	GLfloat r; // Largest cell radius
	Shader shaderProgram;	
	std::vector<float> pos; // Position of each particle
	std::vector<float> vel; // Velocity of each particle
	// Multipies the speed that each cell goes through the cell cycle, > 2.0 = cancer cell
	// The higher the speed multiplier, the more resistant the cell is to apoptosis
	std::vector<float> speedMultiplier;
	std::vector<float> statusDurationSeconds; // Duration in seconds in current stage of cycle
	std::vector<GLfloat> verts; // Vertex data
	std::vector<GLuint> indices; // Index data
	bool duplicationOcured;
	GLuint VAO, VBO, EBO, texture[8];

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
