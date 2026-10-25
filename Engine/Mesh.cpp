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
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define COLOR_TEXTURE_UNIT GL_TEXTURE0

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2

Mesh::~Mesh() {
	//Clear();
}

void Mesh::Clear() {
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
	if (m_buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);
	}
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void Mesh::Render() {
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		unsigned int materialIndex = m_Meshes[i].materialIndex;

		if (materialIndex < m_Textures.size() && m_Textures[materialIndex]) {
			m_Textures[materialIndex]->Bind(COLOR_TEXTURE_UNIT);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Meshes[i].numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Meshes[i].baseIndex),
			m_Meshes[i].baseVertex
		);
	}
	glBindVertexArray(0);
}

bool Mesh::ChangeTexture(unsigned int materialIndex, const std::string& fileName) {
	if (materialIndex >= m_Textures.size()) {
		return false;
	}
	//Check if there is no material
	if (m_Textures[materialIndex] == nullptr) {
		m_Textures[materialIndex] = new Texture(fileName);
		if (!m_Textures[materialIndex]->Load()) {
			std::cerr << "Error loading texture: " << fileName.c_str() << std::endl;
			delete m_Textures[materialIndex];
			m_Textures[materialIndex] = nullptr;
			return false;
		}
		return true;
	}
	// change material
	return m_Textures[materialIndex]->LoadFromFile(fileName);
}