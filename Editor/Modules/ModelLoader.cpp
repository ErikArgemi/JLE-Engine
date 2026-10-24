#include "ModelLoader.h"
#include "../../Engine/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

Mesh ModelLoader::LoadMesh(const std::string& filename) {
	Mesh newMesh;
	glGenVertexArrays(1, &newMesh.m_VAO);

	glGenBuffers(1, &newMesh.m_VBO);

	glGenBuffers(1, &newMesh.m_EBO);

	glBindVertexArray(newMesh.m_VAO);

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(newMesh.m_buffers), newMesh.m_buffers);

	bool ret = false;
	Assimp::Importer importer;

	//search for model and texture
	const aiScene* pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);

	if (pScene) {
		ret = InitFromScene(newMesh, pScene, filename);
	}
	else {
		std::cout << "Error parsing " << filename.c_str() << ": " << importer.GetErrorString() << std::endl;
	}
	if (!ret) {
		std::cout << "Failed to init the model" << std::endl;
	}
	glBindVertexArray(0);

	return newMesh;
}
Mesh ModelLoader::LoadPrimitive(const std::vector<float>& vertices, const std::vector<int>& indices) {
	Mesh newMesh;

	glGenVertexArrays(1, &newMesh.m_VAO);
	glGenBuffers(1, &newMesh.m_VBO);
	glGenBuffers(1, &newMesh.m_EBO);

	glBindVertexArray(newMesh.m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, newMesh.m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	MeshEntry entry;
	entry.numIndices = (unsigned int)indices.size();
	entry.baseVertex = 0;
	entry.baseIndex = 0;
	entry.materialIndex = 0;
	newMesh.m_Meshes.assign(1, entry);
	return newMesh;
}
bool ModelLoader::InitFromScene(Mesh& mesh, const aiScene* pScene, const std::string& filename) {
	mesh.m_Meshes.resize(pScene->mNumMeshes);
	mesh.m_Textures.resize(pScene->mNumMaterials);

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	CountVerticesAndIndices(mesh, pScene, numVertices, numIndices);

	ReserveSpace(mesh, numVertices, numIndices);

	InitAllMeshes(mesh, pScene);

	if (!InitMaterials(mesh, pScene, filename)) {
		return false;
	}
	PopulateBuffers(mesh);

	return true;
}

void ModelLoader::CountVerticesAndIndices(Mesh& mesh, const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices) {
	for (unsigned int i = 0; i < mesh.m_Meshes.size(); i++) {
		mesh.m_Meshes[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
		mesh.m_Meshes[i].numIndices = pScene->mMeshes[i]->mNumFaces * 3;
		mesh.m_Meshes[i].baseVertex = numVertices;
		mesh.m_Meshes[i].baseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += mesh.m_Meshes[i].numIndices;
	}
}

void ModelLoader::ReserveSpace(Mesh& mesh, unsigned int numVertices, unsigned int numIndices) {
	mesh.m_Positions.reserve(numVertices);
	mesh.m_Normals.reserve(numVertices);
	mesh.m_TextCoords.reserve(numVertices);
	mesh.m_Indices.reserve(numIndices);
}

void ModelLoader::InitAllMeshes(Mesh& mesh, const aiScene* pScene) {
	for (unsigned int i = 0; i < mesh.m_Meshes.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitSingleMesh(mesh, paiMesh);
	}
}

void ModelLoader::InitSingleMesh(Mesh& mesh, const aiMesh* paiMesh) {
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	//populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D& pPos = paiMesh->mVertices[i];
		const aiVector3D& pNormal = paiMesh->mNormals[i];
		const aiVector3D& pTextCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;

		mesh.m_Positions.push_back(Vector3f(pPos.x, pPos.y, pPos.z));
		mesh.m_Normals.push_back(Vector3f(pNormal.x, pNormal.y, pNormal.z));
		mesh.m_TextCoords.push_back(Vector2f(pTextCoord.x, pTextCoord.y));
	}

	//populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		mesh.m_Indices.push_back(face.mIndices[0]);
		mesh.m_Indices.push_back(face.mIndices[1]);
		mesh.m_Indices.push_back(face.mIndices[2]);
	}
}

bool ModelLoader::InitMaterials(Mesh& mesh, const aiScene* pScene, const std::string& filename) {
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

		mesh.m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(path.C_Str());
				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}
				std::string fullPath = dir + "/" + p;
				//Load texture
				mesh.m_Textures[i] = new Texture(fullPath.c_str());
				if (!mesh.m_Textures[i]->Load()) {
					std::cerr << "Error loading texture: " << fullPath << std::endl;
					delete mesh.m_Textures[i];
					mesh.m_Textures[i] = NULL;
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

void ModelLoader::PopulateBuffers(Mesh& mesh) {
	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_buffers[mesh.Buffer_Type::POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.m_Positions[0]) * mesh.m_Positions.size(), mesh.m_Positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDisableVertexAttribArray(1);
	glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_buffers[mesh.Buffer_Type::TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.m_TextCoords[0]) * mesh.m_TextCoords.size(), mesh.m_TextCoords.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_buffers[mesh.Buffer_Type::INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.m_Indices[0]) * mesh.m_Indices.size(), mesh.m_Indices.data(), GL_STATIC_DRAW);
}