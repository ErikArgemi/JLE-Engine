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