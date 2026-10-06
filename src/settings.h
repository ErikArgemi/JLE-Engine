#pragma once
struct RenderSettings {
	uint16_t windowsSizeX;
	uint16_t windowsSizeY;

	RenderSettings() {}

	RenderSettings(int x, int y) {
		windowsSizeX = x;
		windowsSizeY = y;
	}
};

struct EditorSettings {
	bool ImGuiActive;

	EditorSettings() {}

	EditorSettings(bool active) {
		ImGuiActive = active;
	}
};