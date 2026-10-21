#include "TransformComponent.h"

void TransformComponent::Update() {
    //update model matrix
    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);
    modelMat = glm::rotate(modelMat, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::scale(modelMat, scale);
}