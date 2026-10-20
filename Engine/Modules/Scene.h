#include "../Module.h"
#include <vector>

class GameObject;

class Scene : public Module {
public:
	Scene();
	~Scene();

	bool Awake();
	bool Update();

	std::vector<GameObject*> GetGameObjects();

private:
	GameObject* rootGameObject = nullptr;
};