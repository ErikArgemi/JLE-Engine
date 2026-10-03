#include <iostream>
#include <sstream>
#include <iomanip>

#include "Engine.h"
//Modules includes
#include "Modules/Windows.h"
#include "Modules/Render.h"
#include "Modules/Input.h"



Engine::Engine() {
	//Modules
	windows = std::make_shared<Windows>();
	render = std::make_shared<Render>();
	input = std::make_shared<Input>();

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
	if (result) {
		result = PreUpdate();
	}
	if (result) {
		result = DoUpdate();
	}
	if (result) {
		result = PostUpdate();
	}
	return result;
}

bool Engine::CleanUp() {
	bool result = true;
	for (const auto& module : moduleList) {
		result = module->CleanUp();
		if (!result) {
			break;
		}
	}
	return result;
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