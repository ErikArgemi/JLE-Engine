#include "Input.h"
#include "../Engine.h"
#include "Windows.h"
#include "Render.h"
#include <iostream>

Input::Input() : Module() {
	name = "input";
}
Input::~Input() {

}

bool Input::PreUpdate() {

    Render& render = *Engine::GetInstance().render;
	float deltaTime = Engine::GetInstance().dt;

    while (SDL_PollEvent(&event))
    {
    const bool* keyboard = SDL_GetKeyboardState(nullptr);
    const Uint32 mouse = SDL_GetMouseState(nullptr, nullptr);

    if (mouse & SDL_BUTTON_RMASK) {

        //Camera look
        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            render.yaw += event.motion.xrel * render.sensitivity;
            render.pitch -= event.motion.yrel * render.sensitivity;
            //clamps camera to prevent the user from looking so far up or down the pitch the camera turns upside down
            render.pitch = glm::clamp(render.pitch, -89.0f, 89.0f);
        }

		glm::vec3 forward = render.direction; //gets the forward direction of the camera

        float speed = movementSpeed * deltaTime;

		//Movment speed boost when Lshift is held
        if (keyboard[SDL_SCANCODE_LSHIFT]) {
            speed = (movementSpeed+speedBoost) * deltaTime;
        }

        if (glm::length(forward) > 0.0f) {
            forward = glm::normalize(forward);
        }

		//gets the perpendicular vector to the forward direction, which is the right direction of the camera
        glm::vec3 right = glm::normalize(glm::cross(forward, render.y_axis));

        //Camera movement
        if (keyboard[SDL_SCANCODE_W]) {
            render.position += forward * speed;
        }

        if (keyboard[SDL_SCANCODE_S]) {
            render.position -= forward * speed;
        }

        if (keyboard[SDL_SCANCODE_A]) {
            render.position -= right * speed;
        }

        if (keyboard[SDL_SCANCODE_D]) {
            render.position += right * speed;
        }

        if (keyboard[SDL_SCANCODE_Q]) {
            render.position -= render.y_axis * speed;
        }

        if (keyboard[SDL_SCANCODE_E]) {
            render.position += render.y_axis * speed;
        }

	}
    
    ImGui_ImplSDL3_ProcessEvent(&event);
    if (event.type == SDL_EVENT_QUIT)
    {
        return false;
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_UP
        && event.button.button == SDL_BUTTON_RIGHT)
    {
    }

    if ((event.type == SDL_EVENT_MOUSE_BUTTON_DOWN
        && event.button.button == SDL_BUTTON_RIGHT))
    {
    }

    if (event.type == SDL_EVENT_WINDOW_RESIZED)
    {
        int w, h;
        SDL_GetWindowSize(Engine::GetInstance().windows->window, &w, &h);
        Engine::GetInstance().windows->ResizeWindows(ImVec2(w, h));
    }
    }

    return true;
}
