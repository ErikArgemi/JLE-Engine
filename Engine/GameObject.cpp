#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Components/MaterialComponent.h"
#include "../Engine/Components/MeshComponent.h"
#include "../Engine/Components/TransformComponent.h"

// ImGui
#include "imgui.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::CreateComponents(const std::vector<ComponentType>& newComponents)
{
	for (const auto& component : newComponents) {
		this->CreateComponent(component);
	}
}

Component& GameObject::CreateComponent(ComponentType type) {
	std::unique_ptr<Component> component;

	switch(type) {
		case ComponentType::MESH:
		{
			component = std::make_unique<MeshComponent>(this);
			break;
		}
		case ComponentType::MATERIAL:
		{
			component = std::make_unique<MaterialComponent>(this);
			break;
		}
		case ComponentType::TRANSFORM:
		{
			component = std::make_unique<TransformComponent>(this);
			break;
		}
	}

	Component* ret = component.get();
	components.emplace_back(std::move(component));
	return *ret;
}

Component* GameObject::GetComponent(ComponentType type) {
	for (auto& component : components) {
		if (component->type == type) {
			return component.get();
		}
	}

	return nullptr;
}

void GameObject::Update() {
	if (components.size() != 0) {
		for (auto& component : components) {
			if (component) component->Update();
		}
	}
	if (children.size() != 0) {
		for (auto& child : children) {
			if (child) child->Update();
		}
	}
}

void GameObject::GetAllGameObjects(std::vector<GameObject*>& vec) {
	if (children.size() != 0) {
		vec.push_back(this);
		for (auto& child : children) {
			child->GetAllGameObjects(vec);
		}
	}
	else {
		vec.emplace_back(this);
		return;
	}
}