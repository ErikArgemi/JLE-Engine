#include "../Module.h"
#include <iostream>
#include <vector>

class GameObject;

class Scene : public Module {
public:
	Scene();
	~Scene();

	bool Awake();
	bool Start();
	bool Update();

	std::vector<GameObject*> GetGameObjects();
	GameObject* GetRootGameObject() { return rootGameObject.get(); }

	//TODO: Create AddGameObject

private:
	std::unique_ptr<GameObject> rootGameObject;
};