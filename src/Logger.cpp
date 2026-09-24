#include "Logger.h"
#include <iostream>
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

void Log::LOG(std::string message) {
	msgLog.push_back(message);
}

void Log::Clear() {
	msgLog.clear();
}

void Log::DrawConsole() {
    //Console window
    ImGui::Begin("Console", nullptr);
    for (const auto& a : msgLog) {
        ImGui::Text(a.c_str());
    }
    ImGui::End();
}