#include "../Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TransformComponent : public Component {
public:
	TransformComponent() { type = ComponentType::TRANSFORM; }
	TransformComponent(GameObject* owner) { this->owner = owner; type = ComponentType::TRANSFORM;}
	~TransformComponent() {}

	void Enable() { active = true; }
	void Update();
	void Disable() { active = false; }

	void CleanUp() override {}

	glm::mat4 modelMat;

	glm::vec3 position{ 0.0f };
	glm::vec3 scale{ 1.0f };
	glm::vec3 rotation{ 0.0f };
};