#include "MeshComponent.h"

void MeshComponent::Update() {

}

void MeshComponent::SetData(const MeshComponent& meshComponent) {
	VAO = meshComponent.VAO;
	VBO = meshComponent.VBO;
	EBO = meshComponent.EBO;
	n_index = meshComponent.n_index;
}