#include "Scene.h"
#include "../Modules/Scene.h"
#include "../GameObject.h"
#include <iostream>
#include "../../Engine/Components/MeshComponent.h"
#include "../../Engine/Components/TransformComponent.h"
#include "../../Engine/Components/MaterialComponent.h"
#include "../../Editor/App.h"
#include "Render.h"

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

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject)
{
	if (!rootGameObject) {
		rootGameObject = std::move(gameObject);
		return;
	}
	rootGameObject->children.emplace_back(std::move(gameObject));
}

std::unique_ptr<GameObject> Scene::CreatePrimitive(Primitive& primitive, glm::vec3 position, std::string name) {
	auto newGameObject = std::make_unique<GameObject>();
	newGameObject->name = name;
	newGameObject->CreateComponents({ ComponentType::TRANSFORM, ComponentType::MESH, ComponentType::MATERIAL });

	auto* transform = static_cast<TransformComponent*>(newGameObject->GetComponent(ComponentType::TRANSFORM));
	auto* mesh = static_cast<MeshComponent*>(newGameObject->GetComponent(ComponentType::MESH));
	auto* material = static_cast<MaterialComponent*>(newGameObject->GetComponent(ComponentType::MATERIAL));

	transform->position = position;

	mesh->mesh.LoadPrimitive(primitive.vertex, primitive.index); //function to make the mesh of primitives. This is because they are not leaded from the FBX so they need to have their VAO binded.
	material->LoadTexture(primitive.texturePath); // same with textures, as they come in the FBX for the normal GameObjects
	return newGameObject;
}

bool Scene::Awake() {
	//parse scene jsons to load GameObjects
	return true;
}

bool Scene::Start() {

	rootGameObject = std::make_unique<GameObject>();
	rootGameObject->CreateComponents({ ComponentType::MESH, ComponentType::TRANSFORM, ComponentType::MATERIAL });

	auto* meshComponent = static_cast<MeshComponent*>(rootGameObject->GetComponent(ComponentType::MESH));
	auto* transformComponent = static_cast<TransformComponent*>(rootGameObject->GetComponent(ComponentType::TRANSFORM));
	auto* materialComponent = static_cast<MaterialComponent*>(rootGameObject->GetComponent(ComponentType::MATERIAL));

	meshComponent->mesh.LoadMesh("Data/BakerHouse.fbx");
	meshComponent->SetData(*meshComponent);
	transformComponent->position = { 0,0,0 };
	materialComponent->LoadTexture("Data/Baker_house.png");

	return true;
}

bool Scene::Update() {
	if(rootGameObject) rootGameObject->Update();
	return true;
}