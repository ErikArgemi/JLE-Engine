#include <iostream>
#include "src/Engine.h"
#include "src/Log.h"

int main() {
	std::cout << "engine starting----------"<< std::endl;
	//LOG("Engine starting ...");
	Engine::EngineState state = Engine::EngineState::CREATE;
	int result = EXIT_FAILURE;
	std::cout << "engine create----------" << std::endl;
	while (state != Engine::EngineState::EXIT) {
		switch (state) {
		case Engine::EngineState::CREATE:
			//LOG("CREATE");
			std::cout << "engine awake----------" << std::endl;
			state = Engine::EngineState::AWAKE;
			break;

		case Engine::EngineState::AWAKE:
			//LOG("AWAKEN MODULES");
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
			//LOG("START MODULES");
			if (Engine::GetInstance().Start() == true) {
				state = Engine::EngineState::UPDATE;
				//LOG("UPDATE MODULES");
			}
			else {
				std::cout << "module start fail" << std::endl;
				state = Engine::EngineState::FAIL;
				//LOG("ERROR: Start failed");
			}
			break;

		case Engine::EngineState::UPDATE:
			if (Engine::GetInstance().Update() == false) {
				state = Engine::EngineState::CLEAN;
			}
			break;

		case Engine::EngineState::CLEAN:
			std::cout << "module clean----------" << std::endl;
			//LOG("CLEANUP MODULES");
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
			//LOG("Exiting with errors");
			result = EXIT_FAILURE;
			state = Engine::EngineState::EXIT;
			break;
		}
	}
	//LOG("THE END");

	return result;
}
