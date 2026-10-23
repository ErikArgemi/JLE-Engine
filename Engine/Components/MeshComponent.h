#include "../Component.h"
#include <glad/glad.h>
#include "../Mesh.h"

class MeshComponent : public Component { //TODO: Add the assimp for loading meshes
public:
	MeshComponent(bool isPrimitive) { type = ComponentType::MESH; mesh.isPrimitive = isPrimitive; }
	MeshComponent(GameObject* owner, bool isPrimitive) {
		this->owner = owner; 
		type = ComponentType::MESH; 
		mesh.isPrimitive = isPrimitive;
	}

	~MeshComponent() { }

	void Enable() { active = true; }
	void Update();
	void Disable() { active = false; }

	void CleanUp() override { mesh.Clear(); }

	void SetData(const MeshComponent& meshComponent);

	GLsizei n_index = 0;
	//Loading Meshes and textures
	Mesh mesh;
};