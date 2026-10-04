#include <iostream>
#include "src/Engine.h"
#include "src/Log.h"

int main() {
	//LOG("Engine starting ...");
	Engine::EngineState state = Engine::EngineState::CREATE;
	int result = EXIT_FAILURE;

	while (state != Engine::EngineState::EXIT) {
		switch (state) {
		case Engine::EngineState::CREATE:
			//LOG("CREATE");
			state = Engine::EngineState::AWAKE;
			break;

		case Engine::EngineState::AWAKE:
			//LOG("AWAKEN MODULES");
			if (Engine::GetInstance().Awake() == true) {
				state = Engine::EngineState::START;
			}
			else {
				//LOG("ERROR: Awake failed");
				state = Engine::EngineState::FAIL;
			}
			break;

		case Engine::EngineState::START:
			//LOG("START MODULES");
			if (Engine::GetInstance().Start() == true) {
				state = Engine::EngineState::UPDATE;
				//LOG("UPDATE MODULES");
			}
			else {
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
			//LOG("CLEANUP MODULES");
			if (Engine::GetInstance().CleanUp() == true) {
				result = EXIT_SUCCESS;
				state = Engine::EngineState::EXIT;
			}
			else {
				state = Engine::EngineState::FAIL;
			}
			break;

		case Engine::EngineState::FAIL:
			//LOG("Exiting with errors");
			result = EXIT_FAILURE;
			state = Engine::EngineState::EXIT;
			break;
		}
	}

	//LOG("THE END");

	return result;
}
