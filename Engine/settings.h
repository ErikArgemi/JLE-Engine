#pragma once
struct RenderSettings {
	uint16_t windowsSizeX;
	uint16_t windowsSizeY;

	RenderSettings() {
		windowsSizeX = 1920;
		windowsSizeY = 1080;
	}

	RenderSettings(int x, int y) {
		windowsSizeX = x;
		windowsSizeY = y;
	}
};

struct EditorSettings {
	bool ImGuiActive;

	EditorSettings() {
		ImGuiActive = true;
	}

	EditorSettings(bool active) {
		ImGuiActive = active;
	}
};

struct WindowSettings {
	bool fullScreen;

	WindowSettings() {
		fullScreen = true;
	}

	WindowSettings(bool fullScreen) {
		this->fullScreen = fullScreen;
	}
};