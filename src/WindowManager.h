#include <stdio.h>
#include "imgui.h"
#include "imgui_internal.h"

class WindowManager { // will have to inherit from a module/manager class 
public:
	WindowManager() {}
	~WindowManager() {}
	
	void AddWindow(std::string newWindow) {
		for (const auto& window : windows) {
			if (window == newWindow) return;
		}
		windows.emplace_back(newWindow);
	}

	void ResizeWindows(ImVec2 windowSize) {
		currentWindowSize = windowSize;
		for (auto& window : windows) {
			RescaleWindow(window);
		}
		prevWindowsSize = currentWindowSize;
	}

private:
	void RescaleWindow(std::string window) {
		ImGuiWindow* win = ImGui::FindWindowByName(window.c_str());
		if (!win) return;

		float scaleX = (float)currentWindowSize.x / (float)prevWindowsSize.x;
		float scaleY = (float)currentWindowSize.y / (float)prevWindowsSize.y;

		ImVec2 newPos(win->Pos.x * scaleX, win->Pos.y * scaleY);
		ImVec2 newSize(win->Size.x * scaleX, win->Size.y * scaleY);

		ImGui::SetWindowPos(window.c_str(), newPos, ImGuiCond_Always);
		ImGui::SetWindowSize(window.c_str(), newSize, ImGuiCond_Always);
	}

	std::vector<std::string> windows;
	ImVec2 currentWindowSize;
	ImVec2 prevWindowsSize{ 1920, 1080 };
};