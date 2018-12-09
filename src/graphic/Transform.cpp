#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Transform::GetTransformMatrix() const
{
	const auto translationMatrix = GetTranslationMatrix();
	const auto scaleMatrix = GetScaleMatrix();
	const auto rotationMatrix = GetRotationMatrix();
	
	return translationMatrix * rotationMatrix * scaleMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Transform::GetInverseTransformMatrix() const
{
	const auto translationMatrix = glm::inverse(GetTranslationMatrix());
	const auto scaleMatrix = glm::inverse(GetScaleMatrix());
	const auto rotationMatrix = glm::inverse(GetRotationMatrix());

	return  scaleMatrix * rotationMatrix * translationMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Transform::GetRotationMatrix() const
{
	const glm::quat qPitch = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1, 0, 0));
	const glm::quat qYaw = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0, 1, 0));
	const glm::quat qRoll = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0, 0, 1));

	const glm::quat rotQuat = qYaw * qPitch * qRoll;

	return toMat4(rotQuat);
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Transform::GetTranslationMatrix() const
{
	return glm::translate(glm::mat4(), position);
}

//---------------------------------------------------------------------------------------------------------------------
glm::mat4 Transform::GetScaleMatrix() const
{
	return glm::scale(glm::mat4(), scale);
}
