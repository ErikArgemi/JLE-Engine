#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "../Module.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <SDL3/SDL.h>

#include "../settings.h"

class Windows : public Module {
public:
	Windows();

	~Windows();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

private:


public:
    SDL_Window* window = NULL;

    // Window Resolution
    const int SCREEN_WIDTH = 1920;
    const int SCREEN_HEIGHT = 1080;

private:

};