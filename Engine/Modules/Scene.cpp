#include "../Modules/Scene.h"
#include "../GameObject.h"
#include <iostream>

Scene::Scene() {
	name = "Scene";
}

Scene::~Scene() {}

std::vector<GameObject*> Scene::GetGameObjects()
{
	if (!rootGameObject) {
		
		return std::vector<GameObject*>();
	}
	std::vector<GameObject*> ret;
	rootGameObject->GetAllGameObjects(ret);
	return ret;
}

bool Scene::Awake() {
	//parse scene jsons to load GameObjects
	return true;
}

bool Scene::Update() {
	if(rootGameObject) rootGameObject->Update();
	return true;
}