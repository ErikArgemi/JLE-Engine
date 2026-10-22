#include <iostream>
//#include "src/Engine.h"
//#include "src/Log.h"
#include "Engine.h"
#include "Log.h"
#include <filesystem>

int main() {
	std::cout << std::filesystem::current_path() << '\n';

	std::cout << "engine starting----------"<< std::endl;
	Engine::EngineState state = Engine::EngineState::CREATE;
	int result = EXIT_FAILURE;
	std::cout << "engine create----------" << std::endl;
	while (state != Engine::EngineState::EXIT) {
		switch (state) {
		case Engine::EngineState::CREATE:
			std::cout << "engine awake----------" << std::endl;
			state = Engine::EngineState::AWAKE;
			break;

		case Engine::EngineState::AWAKE:
			std::cout << "module awake----------" << std::endl;
			if (Engine::GetInstance().Awake() == true) {
				state = Engine::EngineState::START;
			}
			else {
				std::cout << "module awake fail" << std::endl;
				state = Engine::EngineState::FAIL;
			}
			break;

		case Engine::EngineState::START:
			std::cout << "module start----------" << std::endl;
			if (Engine::GetInstance().Start() == true) {
				state = Engine::EngineState::UPDATE;
			}
			else {
				std::cout << "module start fail" << std::endl;
				state = Engine::EngineState::FAIL;
			}
			break;

		case Engine::EngineState::UPDATE:
			if (Engine::GetInstance().Update() == false) {
				state = Engine::EngineState::CLEAN;
			}
			break;

		case Engine::EngineState::CLEAN:
			std::cout << "module clean----------" << std::endl;
			if (Engine::GetInstance().CleanUp() == true) {
				result = EXIT_SUCCESS;
				state = Engine::EngineState::EXIT;
			}
			else {
				std::cout << "module clean fail" << std::endl;
				state = Engine::EngineState::FAIL;
			}
			break;

		case Engine::EngineState::FAIL:
			std::cout << "fail---------------" << std::endl;
			result = EXIT_FAILURE;
			state = Engine::EngineState::EXIT;
			break;
		}
	}
	return result;
}
