#pragma once
#include <iostream>

class GameObject;

enum class ComponentType{
	MESH = 0,
	TRANSFORM,
	MATERIAL
};

class Component {
public:
	Component() {};
	Component(GameObject* owner) { this->owner = owner; }
	~Component() {}

	virtual void Enable() = 0;
	virtual void Update() = 0;
	virtual void Disable() = 0;

	ComponentType type;
	bool active = true;
	GameObject* owner = nullptr;
};