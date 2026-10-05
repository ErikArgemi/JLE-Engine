#pragma once

#include <string>
class Module {
public:
	//Start with false as maybe the module is activated later
	Module() : active(false) {}

	//Called when initialise the module
	void Init() {
		active = true;
	}

	// Called before render is available
	virtual bool Awake() {
		return true;
	}

	// Called before the first frame
	virtual bool Start() {
		return true;
	}
	// Called each loop iteration before the main update
	virtual bool PreUpdate() {
		return true;
	}

	// Called each loop iteration
	virtual bool Update() { //I could make it so it passes dt from frame to frame, but i'm too lazy to do the math rn
		return true;
	}

	// Called each loop iteration after the main update 
	virtual bool PostUpdate() {
		return true;
	}

	// Called before quitting
	virtual bool CleanUp() {
		return true;
	}

public:
	std::string name;
	bool active;
};