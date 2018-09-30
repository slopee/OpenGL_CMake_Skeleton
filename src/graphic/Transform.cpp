#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

glm::mat4 Transform::GetTransformMatrix() const
{
	auto translationMatrix = glm::translate(glm::mat4(), position);
	auto scaleMatrix = glm::scale(glm::mat4(), scale);
	
	glm::quat qPitch = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
	glm::quat qRoll = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));

	glm::quat rotQuat = qYaw *qPitch * qRoll;
		
	return translationMatrix * toMat4(rotQuat) * scaleMatrix;
}


