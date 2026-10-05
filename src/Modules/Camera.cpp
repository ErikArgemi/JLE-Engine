#include "Camera.h"
#include "../Engine.h"
#include "Windows.h"

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
    return;
}
