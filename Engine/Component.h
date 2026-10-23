#pragma once
#include <iostream>

class GameObject;

enum class ComponentType{
	NONE = 0,
	MESH,
	TRANSFORM,
	MATERIAL,
};

class Component {
public:
	Component() {  }
	Component(GameObject* owner) { this->owner = owner; }
	~Component() {}

	virtual void Enable() = 0;
	virtual void Update() = 0;
	virtual void Disable() = 0;

	virtual void CleanUp() = 0;

	ComponentType type = ComponentType::NONE;
	bool active = true;
	GameObject* owner = nullptr;
};