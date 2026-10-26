#include "FileReader.h"

json FileReader::GetFile(const char* path) {
	std::ifstream file(path);
	json to_json;
	file >> to_json;
	if (to_json.empty()) {
		//log: the json file is not correct
		return json();
	}

	return to_json;
}

RenderSettings FileReader::GetRenderSettings()
{
	json file = GetFile("Editor/configFile/render_settings.json");

	int x = file["currentResolution"]["x"];
	int y = file["currentResolution"]["y"];

	return RenderSettings(x, y);
}

EditorSettings FileReader::GetEditorSettings()
{
	json file = GetFile("Editor/configFile/editor_settings.json");

	bool active = file["isImGuiActive"];

	return EditorSettings(active);
}

WindowSettings FileReader::GetWindowSettings() {
	json file = GetFile("Editor/configFile/window_settings.json");

	bool fullScreen = file["fullScreen"];

	return WindowSettings(fullScreen);
}