#include "JSON_FileReader.h"

json JSON_FileReader::GetFile(const char* path) {
	std::ifstream file(path);
	json to_json;
	file >> to_json;
	if (to_json.empty()) {
		//log: the json file is not correct
		return json();
	}

	return to_json;
}

RenderSettings JSON_FileReader::GetRenderSettings()
{
	json file = GetFile("src/configFile/render_settings.json");

	int x = file["currentResolution"]["x"];
	int y = file["currentResolution"]["y"];

	return RenderSettings(x, y);
}

EditorSettings JSON_FileReader::GetEditorSettings()
{
	json file = GetFile("src/configFile/editor_settings.json");

	bool active = file["isImGuiActive"];

	return EditorSettings(active);
}