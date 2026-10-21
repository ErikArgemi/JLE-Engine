#include <assimp/scene.h>
#include <assimp/postprocess.h>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "Texture.h"

// I'll keep these structs here for now, but i'll move it to another header after i gather some data
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

class Mesh {
public:
	Mesh() {};
	~Mesh();

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
	GLuint m_VAO = 0;
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