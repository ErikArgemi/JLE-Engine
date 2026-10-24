#pragma once

#include <memory>
#include <list>
#include "../Module.h"
#include "Logger.h"
#include <SDL3/SDL.h>

// Editor Modules
class Editor;
class ModelLoader;
// Modules
class Render;
class Windows;
class Input;
class Scene;
class Camera;


class App
{
public:
	// Public method to get the instance of the Singleton
	static App& GetInstance();

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
	App();

	// Delete copy constructor and assignment operator to prevent copying
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	//Functions mainly for the time
	//Used to calculate dt,fps...
	void PrepareUpdate();
	//Used to calculate dt,fps...
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	std::list<std::shared_ptr<Module>> moduleList;

public:
	//Manage states of the engine
	enum AppState {
		CREATE = 1,
		AWAKE, //set up libraries if needed
		START,
		UPDATE,
		CLEAN,
		FAIL,
		EXIT
	};

	//Logger
	Log log;

	//Editor Modules
	std::shared_ptr<Editor> editor;
	std::shared_ptr<ModelLoader> modelLoader;

	//Modules
	std::shared_ptr<Render> render;
	std::shared_ptr<Windows> windows;
	std::shared_ptr<Input> input;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Camera> camera;

	//Time
	SDL_Time prevTime;
	SDL_Time currentTime;

	//FPS
	float dt = 1.0f;
	float fps = 60.0f;
	float max_fps = 60.0f;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
private:
};