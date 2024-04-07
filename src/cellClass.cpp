#include "../include/STB/stb_image.h"
#include "headers/cellClass.hpp"
#include "headers/openGLdebug.hpp"
#include "headers/shaderClass.hpp"
#include "srcDir.hpp"
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>


namespace CellPhases {
    const unsigned int count = 7;

    enum Status: unsigned char {
        g1, s, g2, // Interphase
        pro, meta, ana, telo // Mitosis
        // Note that cytokinesis happens instatly, this there is not status for it
    };

    // The Duration of each phase
    static const float durationSeconds[count] = {
        2.4, 2.1, 1.5,
        1.0, 1.0, 1.0, 1.0,
    };

    // The radius at the start of each phase
    static const float minRadius[count] = {
        0.5, 0.9, 0.9,
        1.0, 1.0, 1.0, 1.0
    };

    // The radius at the end of each phase
    static const float maxRadius[count] = {
        0.9, 0.9, 1.0,
        1.0, 1.0, 1.0, 1.0
    };
}

float& Cells::GetPos(unsigned int dimension, unsigned int index) {
    return this->pos[index * 2 + dimension];
}

float& Cells::GetVel(unsigned int dimension, unsigned int index) {
    return this->vel[index * 2 + dimension];
}

float& Cells::GetVerts(unsigned int dimension, unsigned int vertex, unsigned int index) {
    size_t vertexSize = verts.size() / this->N / 4; // Number of floats per vertex
    return this->verts[index * vertexSize * 4 + vertex * vertexSize + 2 + dimension];
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
        std::uniform_int_distribution<unsigned int> uniformDistribution(1, 100);
        std::random_device randomDevice;

        CellPhases::Status phase;
        using namespace CellPhases;

        unsigned int randomNumber = uniformDistribution(randomDevice);

        if (randomNumber <= 24) {
            phase = g1;
        }
        else if (randomNumber <= 45) {
            phase = s;
        }
        else if (randomNumber <= 60) {
            phase = g2;
        }
        else if (randomNumber <= 70) {
            phase = pro;
        }
        else if (randomNumber <= 80) {
            phase = meta;
        }
        else if (randomNumber <= 90) {
            phase = ana;
        }
        else {
            phase = telo;
        }

        std::vector<GLfloat> quad = {
            -1.0,  1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r, (float)phase,
            1.0,  1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r, (float)phase,
            1.0, -1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r, (float)phase,
            -1.0, -1.0, this->GetPos(X, i), this->GetPos(Y, i), this->r, (float)phase,
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

    // Number of floats per vertex
    size_t vertexSize = verts.size() / this->N / 4;

    // Tell OpenGL how the vertex data in VBO is layed out
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)0));
    GLCALL(glEnableVertexAttribArray(0));

    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))));
    GLCALL(glEnableVertexAttribArray(1));

    GLCALL(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat))));
    GLCALL(glEnableVertexAttribArray(2));

    GLCALL(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat))));
    GLCALL(glEnableVertexAttribArray(3));

    // Bind index buffer bbject for vertices
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

    // Fill Index Buffer Object with vertex data
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));

    // Unbind buffers
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCALL(glBindVertexArray(0));
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    // Load images as bytes
    int widthImg, heightImg, numCol;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imageBytes[8];

    std::string texturesPath = SOURCE_DIRECTORY + "/../textures/";
    {
        using namespace CellPhases;

        # define STBILOG(x)\
        x;\
        if (stbi_failure_reason()) throw std::runtime_error(stbi_failure_reason());

        STBILOG(imageBytes[g1] = stbi_load(std::string(texturesPath + "g1.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[s] = stbi_load(std::string(texturesPath + "s.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[g2] = stbi_load(std::string(texturesPath + "g2.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[pro] = stbi_load(std::string(texturesPath + "pro.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[meta] = stbi_load(std::string(texturesPath + "meta.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[ana] = stbi_load(std::string(texturesPath + "ana.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[telo] = stbi_load(std::string(texturesPath + "telo.png").c_str(), &widthImg, &heightImg, &numCol, 0));
        STBILOG(imageBytes[7] = stbi_load(std::string(texturesPath + "ball.png").c_str(), &widthImg, &heightImg, &numCol, 0));
    }

    // Create textures
    for (int i = 0; i < CellPhases::count + 1; ++i) {
        GLCALL(glGenTextures(1, &this->texture[i]));
        GLCALL(glActiveTexture(GL_TEXTURE0 + i));
        GLCALL(glBindTexture(GL_TEXTURE_2D, this->texture[i]));

        // Texture settings
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        // Load image into texture
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes[i]));

        // Generate mini textures
        GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

        // Free memory and unbind texture
        stbi_image_free(imageBytes[i]);
    }

    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Cells::Draw() {
    // Activate shader program
    this->shaderProgram.Activate();

    // Bind texture
    GLCALL(glBindTexture(GL_TEXTURE_2D, this->texture[0]));

    // Texture uniform
    const char* uniformNames[7] = {
        "g1Texture", "sTexture", "g2Texture",
        "proTexture", "metaTexture", "anaTexture", "teloTexture"
    };

    for (int i = 0; i < CellPhases::count; ++i) {
        GLuint textureLoc;
        GLCALL(textureLoc = glGetUniformLocation(this->shaderProgram.ID, uniformNames[i]));
        GLCALL(glUniform1i(textureLoc, i));
    }

    // Draw triangles
    GLCALL(glBindVertexArray(VAO));
    GLCALL(glDrawElements(GL_TRIANGLES, 6 * this->N, GL_UNSIGNED_INT, 0));
}

void Cells::Update(float deltaSeconds) {

    // Check bounds
    for (int i = 0; i < this->N; ++i) {

        if (this->GetPos(X, i) >= 1.0) {
            this->GetVel(X, i) *= -1.0;
            this->GetPos(X, i) -= this->GetPos(X, i) - 1.0;
        }

        if (this->GetPos(X, i) <= -1.0) {
            this->GetVel(X, i) *= -1.0;
            this->GetPos(X, i) -= this->GetPos(X, i) + 1.0;
        }

        if (this->GetPos(Y, i) >= 1.0) {
            this->GetVel(Y, i) *= -1.0;
            this->GetPos(Y, i) -= this->GetPos(Y, i) - 1.0;
        }

        if (this->GetPos(Y, i) <= -1.0) {
            this->GetVel(Y, i) *= -1.0;
            this->GetPos(Y, i) -= this->GetPos(Y, i) + 1.0;
        }
    }

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
    for (int i = 0; i < CellPhases::count; ++i) {
        GLCALL(glDeleteTextures(1, &this->texture[i]));
    }
}

Cells::~Cells() {
    this->Terminate();
}
