#include "headers/cellClass.hpp"
#include "headers/openGLdebug.hpp"
#include "headers/shaderClass.hpp"
#include "srcDir.hpp"
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>


float* Cells::GetPos(unsigned int dimension, unsigned int index) {
    return &this->pos[index * 2 + dimension];
}

const static unsigned int DECIMAL_PERCISION = 5;

void Cells::Init() {

    // Generate random particle positions
    this->pos.reserve(this->N * 2);
    for (int i = 0; i < this->N; ++i) {

        std::srand((unsigned) std::time(NULL));
        float x = std::rand() % (int)std::pow(10, DECIMAL_PERCISION) / std::pow(10, DECIMAL_PERCISION) * 2.0 - 1.0;

        std::srand((unsigned) std::time(NULL));
        float y = std::rand() % (int)std::pow(10, DECIMAL_PERCISION) / std::pow(10, DECIMAL_PERCISION) * 2.0 - 1.0;

        this->pos[i * 2] = x;
        this->pos[i * 2 + 1] = y;
    }

     // x position in quad, y position in quad, particle position x, particle position y, particle position z
    this->verts.reserve(5 * 4 * this->N);
    for (int i = 0; i < this->N; ++i) {
        std::vector<GLfloat> quad = {
            -1.0,  1.0, *this->GetPos(X, i), *this->GetPos(X, i), this->r,
             1.0,  1.0, *this->GetPos(X, i), *this->GetPos(X, i), this->r,
             1.0, -1.0, *this->GetPos(X, i), *this->GetPos(X, i), this->r,
            -1.0, -1.0, *this->GetPos(X, i), *this->GetPos(X, i), this->r,
        };
        verts.insert(verts.end(), quad.begin(), quad.end());
    };

    // Calculate indices for index buffer
    for (unsigned int i = 0; i < this->N; ++i) {
        GLuint offset = i * 4;
        std::vector<GLuint> quad_vertices = {
            offset, offset + 1, offset + 2,
            offset, offset + 2, offset + 3,
        };
        indices.insert(indices.begin(), quad_vertices.begin(), quad_vertices.end());
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
    // Draw triangles
    GLCALL(glBindVertexArray(VAO));
    GLCALL(glDrawElements(GL_TRIANGLES, 6 * this->N, GL_UNSIGNED_INT, 0));
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

