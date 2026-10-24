#pragma once
#include <iostream>
#include "../Engine/Component.h"
#include <vector>

class GameObject {
public:
	GameObject();
	GameObject(std::string name) { this->name = name; }
	~GameObject();

	void CreateComponents(const std::vector<ComponentType>& newComponents);

	void Update();
	
public:
	Component* GetComponent(ComponentType type);
	void GetAllGameObjects(std::vector<GameObject*>& vec); //call only with the rootGameObject

	bool HasChildren() { return !children.empty(); }

	std::string name = "GameObject";
	bool active = true;

	std::vector<std::unique_ptr<Component>> components;

	std::vector<std::unique_ptr<GameObject>> children;
	GameObject* parent = nullptr;

private:
	Component& CreateComponent(ComponentType type);
};