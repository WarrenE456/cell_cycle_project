#include "headers/cellClass.hpp"
#include "headers/openGLdebug.hpp"
#include "headers/shaderClass.hpp"
#include "srcDir.hpp"
#include <GL/gl.h>
#include <cmath>
#include <random>
#include <vector>


float& Cells::GetPos(unsigned int dimension, unsigned int index) {
    return this->pos[index * 2 + dimension];
}

float& Cells::GetVel(unsigned int dimension, unsigned int index) {
    return this->vel[index * 2 + dimension];
}

float& Cells::GetVerts(unsigned int dimension, unsigned int vertex, unsigned int index) {
    return this->verts[index * 20 + vertex * 5 + 2 + dimension];
}

const static unsigned int DECIMAL_PERCISION = 5;

void Cells::Init() {

    // Generate random particle positions
    this->pos.reserve(this->N * 2);
    this->vel.reserve(this->N * 2);
    for (int i = 0; i < this->N; ++i) {
        std::uniform_int_distribution<int> uniform_distribution(-std::pow(10, DECIMAL_PERCISION), std::pow(10, DECIMAL_PERCISION));
        std::random_device random_device;

        // Randomly generate position in [-1, 1]
        float xPos = uniform_distribution(random_device) / (float)std::pow(10, DECIMAL_PERCISION);
        float yPos = uniform_distribution(random_device) / (float)std::pow(10, DECIMAL_PERCISION);

        // Asign position
        this->pos.push_back(xPos);
        this->pos.push_back(yPos);

        // Randomly generate velocity
        float xVel = uniform_distribution(random_device) / (float)std::pow(10, DECIMAL_PERCISION) / 14.0;
        float yVel = uniform_distribution(random_device) / (float)std::pow(10, DECIMAL_PERCISION) / 14.0;

        // Asign velocity
        this->vel.push_back(xVel);
        this->vel.push_back(yVel);
    }

    // x position in quad, y position in quad, particle position x, particle position y, particle position z
    this->verts.reserve(5 * 4 * this->N);
    for (int i = 0; i < this->N; ++i) {
        std::vector<GLfloat> quad = {
            -1.0,  1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r,
            1.0,  1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r,
            1.0, -1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r,
            -1.0, -1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r,
        };
        verts.insert(verts.end(), quad.begin(), quad.end());
    };

    // Calculate indices for index buffer
    indices.reserve(3 * 2 * this->N);
    for (unsigned int i = 0; i < this->N; ++i) {
        GLuint offset = i * 4;
        std::vector<GLuint> quad_vertices = {
            offset, offset + 1, offset + 2,
            offset, offset + 2, offset + 3,
        };
        indices.insert(indices.end(), quad_vertices.begin(), quad_vertices.end());
    }

    // Generate buffers
    GLCALL(glGenVertexArrays(1, &VAO));
    GLCALL(glGenBuffers(1, &VBO));
    GLCALL(glGenBuffers(1, &EBO));

    // Bind VAO
    GLCALL(glBindVertexArray(VAO));

    // Bind VBO
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    // Fill VBO with vertex data
    GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verts.size(), verts.data(), GL_STATIC_DRAW));

    // Tell OpenGL how the vertex data in VBO is layed out
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0));
    GLCALL(glEnableVertexAttribArray(0));

    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))));
    GLCALL(glEnableVertexAttribArray(1));

    GLCALL(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat))));
    GLCALL(glEnableVertexAttribArray(2));

    // Bind index buffer bbject for vertices
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

    // Fill Index Buffer Object with vertex data
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));

    // Unbind buffers
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}

void Cells::Draw() {
    // Activate shader program
    this->shaderProgram.Activate();

    // Draw triangles
    GLCALL(glBindVertexArray(VAO));
    GLCALL(glDrawElements(GL_TRIANGLES, 6 * this->N, GL_UNSIGNED_INT, 0));
}

void Cells::Update(float deltaSeconds) {
    // Update cells positon based on velocity
    for (int i = 0; i < this->N; ++i) {
        this->pos[i * 2] += this->vel[i * 2] * deltaSeconds;
        this->pos[i * 2 + 1] += this->vel[i * 2 + 1] * deltaSeconds;
    }

    // Update vertex data with new position
    for (int i = 0; i < this->N; ++i) {
        this->GetVerts(X, 0, i) = this->GetPos(X, i);
        this->GetVerts(X, 1, i) = this->GetPos(X, i);
        this->GetVerts(X, 2, i) = this->GetPos(X, i);
        this->GetVerts(X, 3, i) = this->GetPos(X, i);
        
        this->GetVerts(Y, 0, i) = this->GetPos(Y, i);
        this->GetVerts(Y, 1, i) = this->GetPos(Y, i);
        this->GetVerts(Y, 2, i) = this->GetPos(Y, i);
        this->GetVerts(Y, 3, i) = this->GetPos(Y, i);
    }
}


void Cells::UpdateBufferData() {
    // Bind Vertex Buffer
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, this->VBO));

    // Fill vertex buffer with new data
    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(GLfloat), verts.data()));

    // Unbind Vertex Buffer
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

const static std::string vertexFilePath = SOURCE_DIRECTORY + "/shaders/cell.vert.glsl";
const static std::string fragmentFilePath = SOURCE_DIRECTORY + "/shaders/cell.frag.glsl";

Cells::Cells(GLuint N, GLfloat r) : N(N), r(r), shaderProgram(vertexFilePath.c_str(), fragmentFilePath.c_str()) {
    this->Init();
}

void Cells::Terminate() {
    GLCALL(glDeleteVertexArrays(1, &this->VAO));
    GLCALL(glDeleteBuffers(1, &this->VBO));
    GLCALL(glDeleteBuffers(1, &this->EBO));
}

Cells::~Cells() {
    this->Terminate();
}
