#pragma once

// GLM
#include "glm/vec3.hpp"
#include <glad/glad.h>

enum class PrimitiveType {
	ICOSAHEDRON = 0
};

struct PrimitiveMesh {
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLsizei n_index = 0;
};

struct PrimitiveEntity {
	PrimitiveType type = PrimitiveType::ICOSAHEDRON;
	glm::vec3 position{ 0.0f };
	glm::vec3 scale{ 1.0f };
	float rotation = 0.0f;
};

class Primitive {
public:

	Primitive(PrimitiveMesh mesh, PrimitiveEntity entity) {
		this->mesh = mesh;
		this->entity = entity;
	}
	~Primitive() {}

	PrimitiveMesh mesh;
	PrimitiveEntity entity;
};

template<size_t vertexArraySize, size_t indexArraySize>
struct PrimitiveData {
	std::array<GLfloat,vertexArraySize> vertex;
	unsigned int num_index;
	std::array<GLuint, indexArraySize> index;
};

struct Cube : PrimitiveData<48, 36> {
	Cube() {
		vertex = {
			// Position        Color
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f
		};
		num_index = 36;
		index = {
				// Top face
			    3, 2, 6,
			    6, 7, 3,
			    // Bottom face
			    0, 1, 5,
			    5, 4, 0,
			    // Left face
			    0, 4, 7,
			    7, 3, 0,
			    // Right face
			    1, 5, 6,
			    6, 2, 1,
			    // Back face
			    0, 1, 2,
			    2, 3, 0,
			    // Front face
			    4, 5, 6,
			    6, 7, 4,
		};
	}
};

struct Icosahedron : PrimitiveData<72, 60> {
	Icosahedron() {
		vertex = {
			//  Position                        Color
			0.0f, 1.0f, 0.0f,               1.0f, 0.0f, 0.0f,
			-0.276f, 0.447f, 0.851f,        0.0f, 1.0f, 0.0f,
			0.724f, 0.447f, 0.526f,         0.0f, 0.0f, 1.0f,
			0.724f, 0.447f, -0.526f,       1.0f, 0.0f, 0.0f,
			-0.276f, 0.447f, -0.851f,       0.0f, 1.0f, 1.0f,
			-0.894f, 0.447f, 0.0f,          1.0f, 1.0f, 0.0f,
			0.276f, -0.447f, 0.851f,        1.0f, 0.0f, 1.0f,
			0.894f, -0.447f, 0.0f,          1.0f, 1.0f, 1.0f,
			0.276f, -0.447f, -0.851f,        1.0f, 0.0f, 0.0f,
			-0.724f, -0.447f, -0.526f,       0.0f, 1.0f, 0.0f,
			-0.724f, -0.447f, 0.526f,        0.0f, 0.0f, 1.0f,
			0.0f, -1.0f, 0.0f,              1.0f, 0.0f, 0.0f
		};
		num_index = 60;
		index = {
		//top 5 faces
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,1,
		//middle 10 faces
		1,6,2,
		2,6,7,
		3,2,7,
		3,7,8,
		4,3,8,
		4,8,9,
		5,4,9,
		5,9,10,
		1,5,10,
		1,10,6,
		//bottom 5 faces
		11,6,7,
		11,7,8,
		11,8,9,
		11,9,10,
		11,10,6
		};
	}
};
struct Pyramid : PrimitiveData<30, 18> {
	Pyramid() {
		vertex = {
			//  Position                        Color
			0.0f, 1.0f, 0.0f,               1.0f, 0.0f, 0.0f,
			-2.0f, -1.0f, 0.0f,				0.0f, 1.0f, 0.0f,
			0.0f, -1.0f, 2.0f,				0.0f, 0.0f, 1.0f,
			2.0f, -1.0f, 0.0f,				1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, -2.0f,				0.0f, 1.0f, 1.0f
		};
		num_index = 18;
		index = {
			//top faces
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,1,
			//base
			1,2,3,
			1,3,4
		};
	}
};