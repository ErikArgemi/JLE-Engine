#pragma once
#include "../Component.h"
#include <glad/glad.h>
#include "IL/il.h"

class MaterialComponent : public Component {
public:
	MaterialComponent() {}
	MaterialComponent(GameObject* owner) { this->owner = owner; }
	~MaterialComponent() {}
	
	void Enable() { active = true; }
	void Update() {}
	void Disable() { active = false; }

	void LoadTexture(const char* path);

	ILint width = 1;
	ILint height = 1;
	GLuint id = 0;
	
private:
	void CreateDefaultTexture();

};