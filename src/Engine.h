#pragma once

#include <memory>
#include <list>
#include "Module.h"

// Modules
class Render;
class Windows;
class Input;


class Engine
{
public:
	// Public method to get the instance of the Singleton
	static Engine& GetInstance();

	//	Called to add the modules of the engine
	void AddModule(std::shared_ptr<Module> module);

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

private:

	// Private constructor to prevent instantiation
	// Constructor
	Engine();

	// Delete copy constructor and assignment operator to prevent copying
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	std::list<std::shared_ptr<Module>> moduleList;

public:
	//Manage states of the engine
	enum EngineState {
		CREATE = 1,
		AWAKE, //set up libraries if needed
		START,
		UPDATE,
		CLEAN,
		FAIL,
		EXIT
	};

	//Modules
	std::shared_ptr<Render> render;
	std::shared_ptr<Windows> windows;
	std::shared_ptr<Input> input;

private:
	//add the variable of the json reader from general configuration here
	//below there's an example code using xml it prob won't work with nlohmann
	/*void LoadConfig()
	{
		pugi::xml_parse_result result = configFile.load_file("config.xml");
		if (result)
		{
			LOG("config.xml parsed without errors");
		}
		else
		{
			LOG("Error loading config.xml: %s", result.description());
		}

	}*/
};