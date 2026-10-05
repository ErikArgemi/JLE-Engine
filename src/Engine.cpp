#include <iostream>
#include <sstream>
#include <iomanip>

#include "Engine.h"
#include "Log.h"
//Modules includes
#include "Modules/Windows.h"
#include "Modules/Render.h"
#include "Modules/Input.h"



Engine::Engine() {
	std::cout << "adding modules-----------" << std::endl;
	//Modules
	windows = std::make_shared<Windows>();
	input = std::make_shared<Input>();
	render = std::make_shared<Render>();
	//add the modules in the list
	AddModule(std::static_pointer_cast<Module>(windows));
	AddModule(std::static_pointer_cast<Module>(input));
		//Render last
	AddModule(std::static_pointer_cast<Module>(render));
}

Engine& Engine::GetInstance() {
	static Engine instance; // Guaranteed to be destroyed and instantiated on first use
	return instance;
}

void Engine::AddModule(std::shared_ptr<Module> module) {
	module->Init();
	moduleList.push_back(module);
}

bool Engine::Awake() {
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

bool Engine::Start() {
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

bool Engine::Update() {
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

bool Engine::CleanUp() {
	LOG("ENGINE CLEAN UP----------");
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->CleanUp();
		if (!result) {
			break;
		}
	}
	log.~Log();
	return result;
}
void Engine::PrepareUpdate() {
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

void Engine::FinishUpdate() {
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

bool Engine::PreUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->PreUpdate();
		if (!result) {
			break;
		}
	}
	return result;
}

bool Engine::DoUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->Update();
		if (!result) {
			break;
		}
	}
	return result;
}

bool Engine::PostUpdate() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->PostUpdate();
		if (!result) {
			break;
		}
	}
	return result;
}