#include "../Component.h"
#include <glad/glad.h>
//include assimp

class MeshComponent : public Component { //TODO: Add the assimp for loading meshes
public:
	MeshComponent() {}
	MeshComponent(GameObject* owner) { this->owner = owner; }
	~MeshComponent() {}

	void Enable() { active = true; }
	void Update();
	void Disable() { active = false; }

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLsizei n_index = 0;
};