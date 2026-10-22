#include "MeshComponent.h"
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

void MeshComponent::Update() {

}

void MeshComponent::SetData(const MeshComponent& meshComponent) {
	m_VAO = meshComponent.m_VAO;
	m_VBO = meshComponent.m_VBO;
	m_EBO = meshComponent.m_EBO;
	n_index = meshComponent.n_index;
}
void MeshComponent::Clear() {
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

bool MeshComponent::LoadMesh(const std::string& filename) {
	Clear();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_buffers), m_buffers);

	bool ret = false;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);

	if (pScene) {
		ret = InitFromScene(pScene, filename);
	}
	else {
		std::cout << "Error parsing " << filename.c_str() << ": " << importer.GetErrorString() << std::endl;
	}

	glBindVertexArray(0);

	return ret;
}

bool MeshComponent::InitFromScene(const aiScene* pScene, const std::string& filename) {
	m_Meshes.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	CountVerticesAndIndices(pScene, numVertices, numIndices);

	ReserveSpace(numVertices, numIndices);

	InitAllMeshes(pScene);

	if (!InitMaterials(pScene, filename)) {
		return false;
	}
	PopulateBuffers();

	return true;
}

void MeshComponent::CountVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices) {
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		m_Meshes[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Meshes[i].numIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Meshes[i].baseVertex = numVertices;
		m_Meshes[i].baseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_Meshes[i].numIndices;
	}
}

void MeshComponent::ReserveSpace(unsigned int numVertices, unsigned int numIndices) {
	m_Positions.reserve(numVertices);
	m_Normals.reserve(numVertices);
	m_TextCoords.reserve(numVertices);
	m_Indices.reserve(numIndices);
}

void MeshComponent::InitAllMeshes(const aiScene* pScene) {
	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitSingleMesh(paiMesh);
	}
}

void MeshComponent::InitSingleMesh(const aiMesh* paiMesh) {
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	//populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D& pPos = paiMesh->mVertices[i];
		const aiVector3D& pNormal = paiMesh->mNormals[i];
		const aiVector3D& pTextCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;

		m_Positions.push_back(Vector3f(pPos.x, pPos.y, pPos.z));
		m_Normals.push_back(Vector3f(pNormal.x, pNormal.y, pNormal.z));
		m_TextCoords.push_back(Vector2f(pTextCoord.x, pTextCoord.y));
	}

	//populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		m_Indices.push_back(face.mIndices[0]);
		m_Indices.push_back(face.mIndices[1]);
		m_Indices.push_back(face.mIndices[2]);
	}
}

bool MeshComponent::InitMaterials(const aiScene* pScene, const std::string& filename) {
	//Extract the directory part from the file name
	std::string::size_type slashIndex = filename.find_last_of("/");
	std::string dir;

	if (slashIndex == std::string::npos) {
		dir = ".";
	}
	else if (slashIndex == 0) {
		dir = "/";
	}
	else {
		dir = filename.substr(0, slashIndex);
	}

	bool ret = true;

	//Init materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(path.C_Str());
				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}
				std::string fullPath = dir + "/" + p;
				//Load texture
				m_Textures[i] = new Texture(fullPath.c_str());
				if (!m_Textures[i]->Load()) {
					std::cerr << "Error loading texture: " << fullPath << std::endl;
					delete m_Textures[i];
					m_Textures[i] = NULL;
					ret = false;
				}
				else {
					std::cerr << "Loaded texture: " << fullPath << std::endl;
				}
			}
		}
	}
	return ret;
}

void MeshComponent::PopulateBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_TextCoords[0]) * m_TextCoords.size(), &m_TextCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
}

void MeshComponent::Render() {
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Meshes.size(); i++) {
		unsigned int materialIndex = m_Meshes[i].materialIndex;

		assert(materialIndex < m_Textures.size());

		if (m_Textures[materialIndex]) {
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

bool MeshComponent::ChangeTexture(unsigned int materialIndex, const std::string& fileName) {
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