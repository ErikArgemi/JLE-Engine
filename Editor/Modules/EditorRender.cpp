#include "EditorRender.h"
#include "../Engine.h"
#include "../Log.h"
#include "../../Engine/Modules/Render.h"
#include "../../Engine/Modules/Windows.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "IL/il.h"

EditorRender::EditorRender() : Module() {
    name = "EditorRender";
}

EditorRender::~EditorRender() {

}

bool EditorRender::Awake() {
    SDL_Window* window = Engine::GetInstance().windows->window;
    SDL_GLContext glContext = Engine::GetInstance().render->GetGLContext();

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    LOG("ImGui created context");
    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init();

    return true;
}

bool EditorRender::CleanUp() {
    // Deinit ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    return true;
}