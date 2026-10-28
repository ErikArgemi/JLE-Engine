#include "Input.h"
#include "../../Editor/App.h"
#include "../../Editor/Log.h"
#include "Render.h"
#include "Scene.h"
#include "../GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Component.h"
#include <iostream>

Input::Input() : Module() {
	name = "input";
}
Input::~Input() {

}

bool Input::PreUpdate() {

    Camera& camera = *App::GetInstance().camera;
    Scene& scene = *App::GetInstance().scene;
	float deltaTime = App::GetInstance().dt;
    const Uint32 mouse = SDL_GetMouseState(nullptr, nullptr);

    while (SDL_PollEvent(&event))
    {
        currentEvents.push_back(event);
        ImGui_ImplSDL3_ProcessEvent(&event);

        //Camera look
        if (event.type == SDL_EVENT_MOUSE_MOTION && mouse & SDL_BUTTON_RMASK){
            if (SDL_GetModState() & SDL_KMOD_ALT) { 
                if (!camera.isOrbiting) {
                    camera.isOrbiting = true;
                    camera.obitDistasnce = -glm::distance(camera.position, camera.orbitCentre);
                }
            }
            else{
                camera.isOrbiting = false;
            }

            camera.yaw += event.motion.xrel * camera.sensitivity;
            camera.pitch -= event.motion.yrel * camera.sensitivity;
            //clamps camera to prevent the user from looking so far up or down the pitch the camera turns upside down
            camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
            //clamps orbit distance to prevent the user from going way past the object
            camera.obitDistasnce = glm::clamp(camera.obitDistasnce, -150.0f, -1.0f);
        }

        //Camera scroll zoom?
        if (event.type == SDL_EVENT_MOUSE_WHEEL && mouse & SDL_BUTTON_RMASK){
            if (camera.isOrbiting) {
                camera.obitDistasnce += event.wheel.y * 2.0f;
            }
            else{
                camera.position += camera.direction * event.wheel.y * 2.0f;
            }
        }
    }
    
    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    if (mouse & SDL_BUTTON_RMASK) {
        glm::vec3 forward = camera.direction; //gets the forward direction of the camera

        float speed = camera.movementSpeed * deltaTime;

        //Movment speed boost when Lshift is held
        if (keyboard[SDL_SCANCODE_LSHIFT]) {
            speed = speed * 2;
        }

        if (glm::length(forward) > 0.0f) {
            forward = glm::normalize(forward);
        }

        //gets the perpendicular vector to the forward direction, which is the right direction of the camera
        glm::vec3 right = glm::normalize(glm::cross(forward, camera.y_axis));

        //Camera movement
        if (keyboard[SDL_SCANCODE_W]) {
            camera.position += forward * speed;
        }

        if (keyboard[SDL_SCANCODE_S]) {
            camera.position -= forward * speed;
        }

        if (keyboard[SDL_SCANCODE_A]) {
            camera.position -= right * speed;
        }

        if (keyboard[SDL_SCANCODE_D]) {
            camera.position += right * speed;
        }

        if (keyboard[SDL_SCANCODE_Q]) {
            camera.position -= camera.y_axis * speed;
        }

        if (keyboard[SDL_SCANCODE_E]) {
            camera.position += camera.y_axis * speed;
        }

    }

    //Focus on selected object when F is pressed
    if (keyboard[SDL_SCANCODE_F]&& !(scene.GetSelectedGameObject() == nullptr)) {
        camera.position = static_cast<TransformComponent*>(scene.GetSelectedGameObject()->GetComponent(ComponentType::TRANSFORM))->position;
    }

    return true;
}

bool Input::PostUpdate() {
    currentEvents.clear();
    return true;
}