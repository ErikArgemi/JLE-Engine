#include "Mesh.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define COLOR_TEXTURE_UNIT GL_TEXTURE0

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

Mesh::~Mesh() {
	//Clear();
}

void Mesh::Clear() {
	if (m_buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
	}
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void Mesh::PopulateNormalsBuffers() {
	if (m_triangleNormals.empty()) return;

	std::vector<glm::vec3> vertices;
	for (const auto& triangleNormal : m_triangleNormals) {
		vertices.emplace_back(triangleNormal.position);
		vertices.emplace_back(triangleNormal.position + triangleNormal.normal * triangleNormal.length);
	}

	if (m_NormalsVAO == 0) {
		glGenVertexArrays(1, &m_NormalsVAO);
		glGenBuffers(1, &m_NormalsVBO);
	}

	glBindVertexArray(m_NormalsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}