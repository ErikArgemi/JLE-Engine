#pragma once

#include "../Module.h"

#include <iostream>
#include <memory>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "ImGuizmo.h"

class Camera : public Module {
public:
	Camera();
	~Camera();

	// Called each loop iteration
	bool Update();

private:
	void UpdateCameraPosition();

public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);

	glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);

	const float sensitivity = 0.5f;
	float yaw = -90.0f;
	float pitch = 0.0f;

	glm::mat4 viewMatrix = glm::lookAt(position, position + z_axis, y_axis);
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix = 1.0f;
	float rotation = 0.0f;

private:
	
};