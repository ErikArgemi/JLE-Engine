#include "../Modules/Scene.h"
#include "../GameObject.h"
#include <iostream>
#include "../../Engine/Components/MeshComponent.h"
#include "../../Engine/Components/TransformComponent.h"
#include "../../Engine/Components/MaterialComponent.h"
#include "../../Editor/App.h"
#include "../../Engine/Modules/Render.h"

#include "../Primitives/Cube.h"

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

bool Scene::Start() {
	Cube cube;

	rootGameObject = std::make_unique<GameObject>();
	rootGameObject->CreateComponents({ ComponentType::MESH, ComponentType::TRANSFORM, ComponentType::MATERIAL });

	auto* mesh = static_cast<MeshComponent*>(rootGameObject->GetComponent(ComponentType::MESH));
	auto* transform = static_cast<TransformComponent*>(rootGameObject->GetComponent(ComponentType::TRANSFORM));
	auto* material = static_cast<MaterialComponent*>(rootGameObject->GetComponent(ComponentType::MATERIAL));

	mesh->SetData(Engine::GetInstance().render->CreateMesh(cube.vertex.data(), cube.vertex.size() * sizeof(float), cube.index.data(), cube.num_index));
	transform->position = { 0,0,0 };
	material->LoadTexture(cube.texturePath);

	return true;
}

bool Scene::Update() {
	if(rootGameObject) rootGameObject->Update();
	return true;
}