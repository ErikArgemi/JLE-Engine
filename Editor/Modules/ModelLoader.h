#pragma once
#include "../../Engine/Module.h"
#include "../../Engine/Mesh.h"
class Mesh;
class ModelLoader : public Module {
public:
	Mesh LoadMesh(const std::string& filename);

	Mesh LoadPrimitive(const std::vector<float>& vertices, const std::vector<int>& indices);
private:
	bool InitFromScene(Mesh& mesh, const aiScene* pScene, const std::string& filename);

	void CountVerticesAndIndices(Mesh& mesh, const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices);

	void ReserveSpace(Mesh& mesh, unsigned int numVertices, unsigned int numIndices);

	void InitAllMeshes(Mesh& mesh, const aiScene* pScene);

	void InitSingleMesh(Mesh& mesh, const aiMesh* paiMesh);

	bool InitMaterials(Mesh& mesh, const aiScene* pScene, const std::string& filename);

	void PopulateBuffers(Mesh& mesh);
};