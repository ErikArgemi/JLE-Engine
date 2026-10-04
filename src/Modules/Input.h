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
class Input : public Module {
public:
	Input();
	~Input();

	// Called each loop iteration
	bool PreUpdate();

private:
public:
	SDL_Event event;
	// SDL KEYS
	int numKeys;
private:
};