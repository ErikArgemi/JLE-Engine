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
    
    UpdateCameraPosition();
    return true;
}

void Camera::UpdateCameraPosition() {

    radYaw = glm::radians(yaw);
    radPitch = glm::radians(pitch);

    if (isOrbiting) {
        UpdateOrbitPosition();
    }

    // View Matrix
    direction.x = cos(radYaw) * cos(radPitch);
    direction.y = sin(radPitch);
    direction.z = sin(radYaw) * cos(radPitch);

    viewMatrix = glm::lookAt(position, position + direction, y_axis);
    // Model Matrix
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), y_axis);

    return;
}

void Camera::UpdateOrbitPosition() {
    glm::vec3 offset;

    offset.x = obitDistasnce * cos(radPitch) * cos(radYaw);
    offset.y = obitDistasnce * sin(radPitch);
    offset.z = obitDistasnce * cos(radPitch) * sin(radYaw);

	position = orbitCentre + offset;

	direction = glm::normalize(orbitCentre - position);

	viewMatrix = glm::lookAt(position, orbitCentre, y_axis);

	return;
}