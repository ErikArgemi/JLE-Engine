#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Components/MaterialComponent.h"
#include "../Engine/Components/MeshComponent.h"
#include "../Engine/Components/TransformComponent.h"

// ImGui
#include "imgui.h"

#define COLOR_TEXTURE_UNIT GL_TEXTURE0

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

glm::vec3 GameObject::GetPosition(){

	auto* transform = static_cast<TransformComponent*>(GetComponent(ComponentType::TRANSFORM));

	if (transform) {
		return transform->position;
	}

	return glm::vec3(0.0f);
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

void GameObject::Render() {

	auto* meshComponent = static_cast<MeshComponent*>(GetComponent(ComponentType::MESH));
	if (!meshComponent) return;
	auto* materialComponent = static_cast<MaterialComponent*>(GetComponent(ComponentType::MATERIAL));
	if (!materialComponent) return;

	glBindVertexArray(meshComponent->mesh.m_VAO);

	for (unsigned int i = 0; i < meshComponent->mesh.m_Meshes.size(); i++) {
		unsigned int materialIndex = meshComponent->mesh.m_Meshes[i].materialIndex;

		if (materialIndex < materialComponent->m_Textures.size() && materialComponent->m_Textures[materialIndex]) {
			materialComponent->m_Textures[materialIndex]->Bind(COLOR_TEXTURE_UNIT);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			meshComponent->mesh.m_Meshes[i].numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * meshComponent->mesh.m_Meshes[i].baseIndex),
			meshComponent->mesh.m_Meshes[i].baseVertex
		);

	}

	glBindVertexArray(0);
}

void GameObject::RenderWireframe() { //TOO: Make it work when also rendering the mesh
	auto* meshComponent = static_cast<MeshComponent*>(GetComponent(ComponentType::MESH));
	if (!meshComponent) return;
	auto* materialComponent = static_cast<MaterialComponent*>(GetComponent(ComponentType::MATERIAL));
	if (!materialComponent) return;

	glBindVertexArray(meshComponent->mesh.m_VAO);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (unsigned int i = 0; i < meshComponent->mesh.m_Meshes.size(); i++) {
		unsigned int materialIndex = meshComponent->mesh.m_Meshes[i].materialIndex;

		if (materialIndex < materialComponent->m_Textures.size() && materialComponent->m_Textures[materialIndex]) {
			materialComponent->m_Textures[materialIndex]->Bind(COLOR_TEXTURE_UNIT);
		}


		glDrawElementsBaseVertex(GL_TRIANGLES,
			meshComponent->mesh.m_Meshes[i].numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * meshComponent->mesh.m_Meshes[i].baseIndex),
			meshComponent->mesh.m_Meshes[i].baseVertex
		);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthFunc(GL_LESS);
	glBindVertexArray(0);
}

GameObject::FaceNormal GameObject::GetNormal(const glm::vec3& A, const glm::vec3 B, const glm::vec3 C) { //TODO: move the FaceNormal code to mesh and load the FaceNormal vector after creating the mesh
	//calculate normal
	glm::vec3 vector1 = B - A;
	glm::vec3 vector2 = C - A;

	glm::vec3 normal = glm::cross(vector1, vector2);
	glm::vec3 normal = glm::normalize(normal);

	//calculate incenter
	float a = glm::distance(B, C);
	float b = glm::distance(A, C);
	float c = glm::distance(A, B);

	if (a + b + c == 0.0f) return FaceNormal({ 0,0,0 }, { 0,0,0 }, 1);

	glm::vec3 position = (a * A + b * B + c * C) / a + b + c;

	return FaceNormal(normal, position, 1);
}

void GameObject::DisplayNormalsPerTriangle()
{
	auto* meshComponent = static_cast<MeshComponent*>(GetComponent(ComponentType::MESH));
	if (!meshComponent) return;

	std::vector<FaceNormal> triangleNormals;

	const auto vertices = meshComponent->mesh.m_Positions;
	for (size_t i = 0; i < vertices.size(); ++i) {
		if (i > vertices.size()) break;
		triangleNormals.emplace_back(GetNormal(vertices[i], vertices[i+1], vertices[i+2]));
	}
}