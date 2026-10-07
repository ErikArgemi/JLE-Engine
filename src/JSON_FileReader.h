#pragma once
#include "./Module.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include "./settings.h"

using json = nlohmann::json;

class JSON_FileReader {
public:
	JSON_FileReader() {};
	~JSON_FileReader() {}

	RenderSettings GetRenderSettings();
	EditorSettings GetEditorSettings();

private:
	json GetFile(const char* path);

};