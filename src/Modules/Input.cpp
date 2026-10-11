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
    while (SDL_PollEvent(&event))
    {
    const bool* keyboard = SDL_GetKeyboardState(nullptr);
    const Uint32 mouse = SDL_GetMouseState(nullptr, nullptr);

    if (mouse & SDL_BUTTON_RMASK) {

        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
			float sensitivity = Engine::GetInstance().render->sensitivity;

            Engine::GetInstance().render->yaw += event.motion.xrel * sensitivity;
            Engine::GetInstance().render->pitch -= event.motion.yrel * sensitivity;

            Engine::GetInstance().render->pitch = glm::clamp(Engine::GetInstance().render->pitch, -89.0f, 89.0f);
        }

        if (keyboard[SDL_SCANCODE_W]) {
            Engine::GetInstance().render->position -= Engine::GetInstance().render->z_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_S]) {
            Engine::GetInstance().render->position += Engine::GetInstance().render->z_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_A]) {
            Engine::GetInstance().render->position -= Engine::GetInstance().render->x_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_D]) {
            Engine::GetInstance().render->position += Engine::GetInstance().render->x_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_Q]) {
            Engine::GetInstance().render->position -= Engine::GetInstance().render->y_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_E]) {
            Engine::GetInstance().render->position += Engine::GetInstance().render->y_axis * movementSpeed * Engine::GetInstance().dt;
        }

        if (keyboard[SDL_SCANCODE_LSHIFT]) {
            movementSpeed *= 2;
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
