#pragma once
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

	glm::mat4 GetTransformMatrix() const;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = {1.0f, 1.0f, 1.0f};
};