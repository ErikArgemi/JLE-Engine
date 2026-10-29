#pragma once

#include "../Module.h"

#include <iostream>
#include <memory>
#include <vector>

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

	// Called each loop iteration after the main update 
	bool PostUpdate();

	// Get the events fromt this loop
	const std::vector<SDL_Event>& GetEvents() const { return currentEvents; }
private:
public:
	bool isHoveringScene = false;
private:
	std::vector<SDL_Event> currentEvents;
	SDL_Event event;
	// SDL KEYS
	int numKeys;
};