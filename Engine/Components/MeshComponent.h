#include "../Component.h"
#include <glad/glad.h>
//include assimp

class MeshComponent : public Component { //TODO: Add the assimp for loading meshes
public:
	MeshComponent() { type = ComponentType::MESH; }
	MeshComponent(GameObject* owner) { this->owner = owner; type = ComponentType::MESH; }
	~MeshComponent() {}

	void Enable() { active = true; }
	void Update();
	void Disable() { active = false; }

	void SetData(const MeshComponent& meshComponent);

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLsizei n_index = 0;
};