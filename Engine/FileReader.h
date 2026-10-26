#pragma once
#include "../Module.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include "../settings.h"

using json = nlohmann::json;

class FileReader {
public:
	FileReader() {};
	~FileReader() {}

	static RenderSettings GetRenderSettings();
	static EditorSettings GetEditorSettings();
	static WindowSettings GetWindowSettings();

private:
	static json GetFile(const char* path);

};