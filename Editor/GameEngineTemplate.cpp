#include <iostream>
//#include "src/App.h"
//#include "src/Log.h"
#include "App.h"
#include "Log.h"
#include <filesystem>

int main() {
	std::cout << std::filesystem::current_path() << '\n';

	std::cout << "engine starting----------"<< std::endl;
	App::AppState state = App::AppState::CREATE;
	int result = EXIT_FAILURE;
	std::cout << "engine create----------" << std::endl;
	while (state != App::AppState::EXIT) {
		switch (state) {
		case App::AppState::CREATE:
			std::cout << "engine awake----------" << std::endl;
			state = App::AppState::AWAKE;
			break;

		case App::AppState::AWAKE:
			std::cout << "module awake----------" << std::endl;
			if (App::GetInstance().Awake() == true) {
				state = App::AppState::START;
			}
			else {
				std::cout << "module awake fail" << std::endl;
				state = App::AppState::FAIL;
			}
			break;

		case App::AppState::START:
			std::cout << "module start----------" << std::endl;
			if (App::GetInstance().Start() == true) {
				state = App::AppState::UPDATE;
			}
			else {
				std::cout << "module start fail" << std::endl;
				state = App::AppState::FAIL;
			}
			break;

		case App::AppState::UPDATE:
			if (App::GetInstance().Update() == false) {
				state = App::AppState::CLEAN;
			}
			break;

		case App::AppState::CLEAN:
			std::cout << "module clean----------" << std::endl;
			if (App::GetInstance().CleanUp() == true) {
				result = EXIT_SUCCESS;
				state = App::AppState::EXIT;
			}
			else {
				std::cout << "module clean fail" << std::endl;
				state = App::AppState::FAIL;
			}
			break;

		case App::AppState::FAIL:
			std::cout << "fail---------------" << std::endl;
			result = EXIT_FAILURE;
			state = App::AppState::EXIT;
			break;
		}
	}
	return result;
}
