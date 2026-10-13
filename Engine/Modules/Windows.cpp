#include "Windows.h"
#include "../Engine.h"
#include "../Log.h"
#include "Render.h"
#include "JSON_FileReader.h"
#include "../PrimitiveManager.h"

#include <iostream>
#include <memory>

// OpenGL
#include <glad/glad.h>
#include <SDL3/SDL.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Additional C++ Headers
#include <vector>

Windows::Windows() : Module(){
	name = "windows";
}

Windows::~Windows() {

}

bool Windows::Awake() {
    // Init SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        LOG("Failed to init SDL");
        std::cout << "windows: failed init video" << std::endl;
        return false;
    }
    LOG("SDL initialized");

    // Setup Min/Major version for using OpenGL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    LOG("OpenGL set up version 4.6");
    // Set Core Profile Mode
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create OpenGL window using SDL
    window = SDL_CreateWindow("EnginishGL",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr)
    {
        std::cout << "failed to create window" << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}