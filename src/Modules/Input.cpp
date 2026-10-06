#include "Input.h"
#include "../Engine.h"
#include "Windows.h"
#include "Render.h"

Input::Input() : Module() {
	name = "input";
}
Input::~Input() {

}

bool Input::PreUpdate() {
    while (SDL_PollEvent(&event))
    {
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

    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    if (keyboard[SDL_SCANCODE_W])
    {
		Engine::GetInstance().render->position += Engine::GetInstance().render->forward * 5.0f * Engine::GetInstance().dt;
    }

    if (keyboard[SDL_SCANCODE_S])
    {
        // S is being held
    }

    if (keyboard[SDL_SCANCODE_A])
    {
        // A is being held
    }

    if (keyboard[SDL_SCANCODE_D])
    {
        // D is being held
    }

    return true;
}
