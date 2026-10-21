#include "../Modules/Scene.h"
#include "../GameObject.h"
#include <iostream>
#include "../../Engine/Components/MeshComponent.h"
#include "../../Engine/Components/TransformComponent.h"
#include "../../Engine/Components/MaterialComponent.h"
#include "../../Editor/Engine.h"
#include "../../Engine/Modules/Render.h"

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
	CubeData cube;

	rootGameObject = std::make_unique<GameObject>();
	rootGameObject->CreateComponent(ComponentType::MESH);
	rootGameObject->CreateComponent(ComponentType::TRANSFORM);
	rootGameObject->CreateComponent(ComponentType::MATERIAL);

	auto* mesh = static_cast<MeshComponent*>(rootGameObject->GetComponent(ComponentType::MESH));
	auto* transform = static_cast<TransformComponent*>(rootGameObject->GetComponent(ComponentType::TRANSFORM));
	auto* material = static_cast<MaterialComponent*>(rootGameObject->GetComponent(ComponentType::MATERIAL));

	transform->position = { 0,0,0 };
	material->LoadTexture(cube.texturePath);
	MeshComponent newMesh = Engine::GetInstance().render->CreateMesh(cube.vertex.data(), cube.vertex.size() * sizeof(float), cube.index.data(), cube.num_index * sizeof(int));
	mesh->VAO = newMesh.VAO;
	mesh->VBO = newMesh.VBO;
	mesh->EBO = newMesh.EBO;
	mesh->n_index = newMesh.n_index;

	return true;
}

bool Scene::Update() {
	if(rootGameObject) rootGameObject->Update();
	return true;
}