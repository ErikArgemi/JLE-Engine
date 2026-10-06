#include "Camera.h"
#include "../Engine.h"
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
    Engine::GetInstance().render->position;
    return;
}
