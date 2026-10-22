#include "../Component.h"
#include <glad/glad.h>
//include assimp
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Texture.h"

struct Vector3f {
	union {
		struct {
			float x, y, z;
		};
		struct {
			float data[3];
		};
	};
	Vector3f() : Vector3f(0, 0, 0) {}
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector2f {
	union {
		struct {
			float x, y;
		};
		struct {
			float data[2];
		};
	};
	Vector2f() : Vector2f(0, 0) {}
	Vector2f(float x, float y) : x(x), y(y) {}
};

class MeshComponent : public Component { //TODO: Add the assimp for loading meshes
public:
	MeshComponent() { type = ComponentType::MESH; }
	MeshComponent(GameObject* owner) { this->owner = owner; type = ComponentType::MESH; }
	~MeshComponent() { }

	void Enable() { active = true; }
	void Update();
	void Disable() { active = false; }

	void SetData(const MeshComponent& meshComponent); //TODO: think if making a MeshData is better to avoid initializing a MeshComponent

	//Assimp load mesh
	bool LoadMesh(const std::string& filename);

	void Render(); // i believe this has to be in render module

	//WorldTrans& GetWorldTransform(){ return m_worldTransform; }

	bool ChangeTexture(unsigned int materialIndex, const std::string& fileName);
private:
	void Clear();

	bool InitFromScene(const aiScene* pScene, const std::string& filename);

	void CountVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices);

	void ReserveSpace(unsigned int numVertices, unsigned int numIndices);

	void InitAllMeshes(const aiScene* pScene);

	void InitSingleMesh(const aiMesh* paiMesh);

	bool InitMaterials(const aiScene* pScene, const std::string& filename);

	void PopulateBuffers(); // i believe this has to be in render module

public:
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	GLsizei n_index = 0;
	
private:

	#define INVALID_MATERIAL 0xFFFFFFFF

	enum Buffer_Type {
		INDEX_BUFFER	= 0,
		POS_VB			= 1,
		TEXCOORD_VB		= 2,
		NORMAL_VB		= 3,
		WVP_MAT_VB		= 4,
		WORLD_MAT_VB	= 5,
		NUM_BUFFERS		= 6
	};

	//WorldTrans m_worldTransform;
	GLuint m_buffers[NUM_BUFFERS] = { 0 };

	struct MeshEntry {
		MeshEntry() {
			numIndices = 0;
			baseVertex = 0;
			baseIndex = 0;
			materialIndex = INVALID_MATERIAL;
		}
		unsigned int numIndices;
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int materialIndex;
	};

	std::vector<MeshEntry> m_Meshes;
	std::vector<Texture*> m_Textures;

	std::vector<Vector3f> m_Positions;
	std::vector<Vector3f> m_Normals;
	std::vector<Vector2f> m_TextCoords;
	std::vector<unsigned int> m_Indices;
};