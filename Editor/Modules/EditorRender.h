#pragma once

#include "../../Engine/Module.h"
#include <SDL3/SDL.h>

class EditorRender : public Module {
public:
    EditorRender();
    ~EditorRender();

    // This one must be called after render is created, due to glcontext
    bool Awake();

    // Called before quitting
    bool CleanUp();
};
