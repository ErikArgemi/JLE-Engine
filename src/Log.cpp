#include "Log.h"
#include <iostream>
#include <cstdarg>
#include <cstdio>
#include <string>

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

void Logger::Log(const char file[], int line, const char* format, ...)
{
    static char tmpString1[4096];
    static va_list ap;

    // Construct the string from variable arguments
    va_start(ap, format);
    vsnprintf(tmpString1, 4096, format, ap);
    va_end(ap);

    // Construct the final log message
    std::string logMessage = std::string("\n") + file + "(" + std::to_string(line) + ") : " + tmpString1;

    // Print the formatted string to the standard error stream
    std::cerr << logMessage << std::endl;

    msgLog.push_back(logMessage);
  
}

void Logger::Clear() {
	msgLog.clear();
}

void Logger::DrawConsole() {
    //Console window
    ImGui::Begin("Console", nullptr);
    for (const auto& a : msgLog) {
        ImGui::Text(a.c_str());
    }
    ImGui::End();
}