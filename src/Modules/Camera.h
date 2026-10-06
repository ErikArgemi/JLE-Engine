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
	
private:
	glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 13.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 view = glm::lookAt(glm::vec3(190.0f, 190.0f, 190.0f),
		glm::vec3(190.0f, 20.0f, 20.0f),
		glm::vec3(20.0f, 20.0f, 190.0f));
};