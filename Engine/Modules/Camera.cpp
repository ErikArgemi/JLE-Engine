#include "Camera.h"
#include "../../Editor/App.h"
#include "Windows.h"
#include "Render.h"

Camera::Camera() : Module() {
    name = "camera";
}

Camera::~Camera() {
}

bool Camera::Update() {
    // View Matrix
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    viewMatrix = glm::lookAt(position, position + direction, y_axis);
    // Model Matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), y_axis);

    return true;
}

void Camera::UpdateCameraPosition() {
    return;
}