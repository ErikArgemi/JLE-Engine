#pragma once
#include "../Component.h"
#include <glad/glad.h>
#include "IL/il.h"

class MaterialComponent : public Component {
public:
	MaterialComponent() { type = ComponentType::MATERIAL; }
	MaterialComponent(GameObject* owner) { this->owner = owner; type = ComponentType::MATERIAL; }
	~MaterialComponent() {}
	
	void Enable() { active = true; }
	void Update() {}
	void Disable() { active = false; }

	void CleanUp() override {}

	void LoadTexture(const char* path);

	ILint width = 1;
	ILint height = 1;
	GLuint id = 0;
	
private:
	void CreateDefaultTexture();
};