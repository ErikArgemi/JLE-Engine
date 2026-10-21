#include <iostream>
#include "../Engine/Component.h"
#include <vector>

class GameObject {
public:
	GameObject();
	GameObject(std::string name) { this->name = name; }
	~GameObject();

	void Update();
	
	Component& CreateComponent(ComponentType type);
	Component* GetComponent(ComponentType type);
	void GetAllGameObjects(std::vector<GameObject*>& vec); //call only with the rootGameObject

	std::string name = "default";
	bool active = true;

	std::vector<std::unique_ptr<Component>> components;

	std::vector<std::unique_ptr<GameObject>> children;
	GameObject* parent;
};