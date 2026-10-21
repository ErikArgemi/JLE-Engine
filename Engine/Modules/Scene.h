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

private:
	std::unique_ptr<GameObject> rootGameObject;
};


struct CubeData {
	std::vector<float> vertex = {
		// Front (z = +0.5)
		-0.5f,-0.5f, 0.5f, 1,1,1, 0,0,   0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
		 0.5f, 0.5f, 0.5f, 1,1,1, 1,1,  -0.5f, 0.5f, 0.5f, 1,1,1, 0,1,
		 // Back (z = -0.5)
		  0.5f,-0.5f,-0.5f, 1,1,1, 0,0,  -0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
		 -0.5f, 0.5f,-0.5f, 1,1,1, 1,1,   0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
		 // Left (x = -0.5)
		 -0.5f,-0.5f,-0.5f, 1,1,1, 0,0,  -0.5f,-0.5f, 0.5f, 1,1,1, 1,0,
		 -0.5f, 0.5f, 0.5f, 1,1,1, 1,1,  -0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
		 // Right (x = +0.5)
		  0.5f,-0.5f, 0.5f, 1,1,1, 0,0,   0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
		  0.5f, 0.5f,-0.5f, 1,1,1, 1,1,   0.5f, 0.5f, 0.5f, 1,1,1, 0,1,
		  // Top (y = +0.5)
		  -0.5f, 0.5f, 0.5f, 1,1,1, 0,0,   0.5f, 0.5f, 0.5f, 1,1,1, 1,0,
		   0.5f, 0.5f,-0.5f, 1,1,1, 1,1,  -0.5f, 0.5f,-0.5f, 1,1,1, 0,1,
		   // Bottom (y = -0.5)
		   -0.5f,-0.5f,-0.5f, 1,1,1, 0,0,   0.5f,-0.5f,-0.5f, 1,1,1, 1,0,
			0.5f,-0.5f, 0.5f, 1,1,1, 1,1,  -0.5f,-0.5f, 0.5f, 1,1,1, 0,1,
	};
	unsigned int num_index = 36;
	std::vector<int> index = {
		 0, 1, 2,   0, 2, 3,     4, 5, 6,   4, 6, 7,
		 8, 9,10,   8,10,11,    12,13,14,  12,14,15,
		16,17,18,  16,18,19,    20,21,22,  20,22,23,
	};
	const char* texturePath = "Data/PrimitiveTextures/checkerboard.dds"; //TODO: Find the correct path
};