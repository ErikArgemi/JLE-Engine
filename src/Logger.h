#pragma once
#include <vector>
#include <string>
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
class Log {
public:
	Log() {}
	~Log() {}

	//Add a message to the log
	void LOG(std::string message) {
		msgLog.push_back(message);
	}

	//Clean the log
	void Clear() {
		msgLog.clear();
	}

	//Draw the console window where the log is displayed
	void Log::DrawConsole() {
		//Console window
		ImGui::Begin("Console", nullptr);
		for (const auto& a : msgLog) {
			ImGui::Text(a.c_str());
		}
		ImGui::End();
	}

	//Get the messages from the log
	const std::vector<std::string>& GetMessages() const {return msgLog;	}
private:
	inline static std::vector<std::string> msgLog;
};