#include "MeshComponent.h"
#include <iostream>

void MeshComponent::Update() {

}

void MeshComponent::SetData(const MeshComponent& meshComponent) {
	mesh.m_VAO = meshComponent.mesh.m_VAO;
	mesh.m_VBO = meshComponent.mesh.m_VBO;
	mesh.m_EBO = meshComponent.mesh.m_EBO;
	n_index = meshComponent.n_index;
}