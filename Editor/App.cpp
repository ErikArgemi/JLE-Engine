#include <iostream>
#include <sstream>
#include <iomanip>

#include "App.h"
#include "Log.h"
//Editor modules include
#include "Modules/Editor.h"
#include "Modules/ModelLoader.h"
#include "Modules/TextureLoader.h"

//Modules includes
#include "../Engine/Modules/Windows.h"
#include "../Engine/Modules/Render.h"
#include "../Engine/Modules/Input.h"
#include "../Engine/Modules/Scene.h"
#include "../Engine/Modules/Camera.h"

App::App() {
	std::cout << "adding modules-----------" << std::endl;

	//Editor Modules
	editor = std::make_shared<Editor>();
	modelLoader = std::make_shared<ModelLoader>();
	textureLoader = std::make_shared<TextureLoader>();

	//Modules
	windows = std::make_shared<Windows>();
	input = std::make_shared<Input>();
	render = std::make_shared<Render>();
	scene = std::make_shared<Scene>();
	camera = std::make_shared<Camera>();

	//add the modules in the list
	//App Modules
	AddModule(std::static_pointer_cast<Module>(windows));
	AddModule(std::static_pointer_cast<Module>(input));
	AddModule(std::static_pointer_cast<Module>(camera));

	//Scene
	AddModule(std::static_pointer_cast<Module>(scene));
		//Render always last
	AddModule(std::static_pointer_cast<Module>(render));
	AddModule(std::static_pointer_cast<Module>(editor));
	AddModule(std::static_pointer_cast<Module>(modelLoader));
	AddModule(std::static_pointer_cast<Module>(textureLoader));

}

App& App::GetInstance() {
	static App instance; // Guaranteed to be destroyed and instantiated on first use
	return instance;
}

void App::AddModule(std::shared_ptr<Module> module) {
	module->Init();
	moduleList.push_back(module);
}

bool App::Awake() {
	LOG("ENGINE AWAKE----------");
	bool result = true;
	for (const auto& module : moduleList) {
		//module->LoadParameters(configFile.child("config").child(module.get()->name.c_str())); from the json, this is an example code using xml it prob won't work with nlohmann
		result = module->Awake();
		if (!result) {
			break;
		}
	}
	return result;
}

bool App::Start() {
	LOG("ENGINE START----------HELLO WORLD");
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->Start();
		if (!result) {
			break;
		}
	}
	return result;
}

bool App::Update() {
	bool result = true;
	PrepareUpdate();

	if (result) {
		result = PreUpdate();
	}
	if (result) {
		result = DoUpdate();
	}
	if (result) {
		result = PostUpdate();
	}

	FinishUpdate();
	return result;
}

bool App::CleanUp() {
	LOG("ENGINE CLEAN UP----------");
	bool result = true;
	moduleList.reverse();
	for (const auto& module : moduleList) {
		result = module->CleanUp();
		if (!result) {
			break;
		}
	}
	log.~Log();
	return result;
}
void App::PrepareUpdate() {
	SDL_GetCurrentTime(&currentTime);
	//FPS Update
	fps = 1.0f / dt;
	if (ms_log.size() > 59) {
		ms_log.erase(ms_log.begin());
	}
	ms_log.emplace_back(dt * 1000);

	if (fps_log.size() > 59) {
		fps_log.erase(fps_log.begin());
	}
	fps_log.emplace_back(fps);
}

void App::FinishUpdate() {
	prevTime = currentTime;
	SDL_GetCurrentTime(&currentTime);

	float current_dt = (currentTime - prevTime) / 1000000000.0f;
	
	float max_dt;
	if (max_fps != 0) {
		max_dt = 1000 / max_fps;
	}
	else max_dt = 0;

	if (current_dt < max_dt && max_dt != 0.0f) {
		SDL_Delay(max_dt - current_dt);
	}
	SDL_GetCurrentTime(&currentTime);
	dt = (currentTime - prevTime) / 1000000000.0f;
	prevTime = currentTime;
}

bool App::PreUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->PreUpdate();
		if (!result) {
			break;
		}
	}
	return result;
}

bool App::DoUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->Update();
		if (!result) {
			break;
		}
	}
	return result;
}

bool App::PostUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->PostUpdate();
		if (!result) {
			break;
		}
	}
	return result;
}