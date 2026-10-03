#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

struct RenderSettings {
	uint16_t windowsSizeX;
	uint16_t windowsSizeY;

	RenderSettings(int x, int y) {
		windowsSizeX = x;
		windowsSizeY = y;
	}
};

struct EditorSettings {
	bool ImGuiActive;

	EditorSettings(bool active) {
		ImGuiActive = active;
	}
};

class JSON_FileReader {
public:
	JSON_FileReader() {};

	RenderSettings GetRenderSettings();
	EditorSettings GetEditorSettings();

private:
	json GetFile(const char* path);

};