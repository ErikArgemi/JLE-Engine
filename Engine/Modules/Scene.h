#pragma once
#include "../Module.h"
#include <iostream>
#include <vector>
#include "glm/glm.hpp"

class GameObject;
class Primitive;

class Scene : public Module {
public:
	Scene();
	~Scene();

	bool Awake();
	bool Start();
	bool Update();

	std::vector<GameObject*> GetGameObjects();
	GameObject* GetRootGameObject() { return rootGameObject.get(); }

	void AddGameObject(std::unique_ptr<GameObject> gameObject);

	GameObject* GetSelectedGameObject() { return selectedGameObject; }
	void SetSelectedGameObject(GameObject* newSelectedGameObject) { selectedGameObject = newSelectedGameObject; }

	std::unique_ptr<GameObject> CreatePrimitive(Primitive& primitive, glm::vec3 position, std::string name = "Primitive");

private:
	std::unique_ptr<GameObject> rootGameObject;
	GameObject* selectedGameObject = nullptr;
};