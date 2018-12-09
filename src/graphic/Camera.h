#pragma once
#include <glm/mat4x3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

class Camera
{
public:
	Camera(float height, float windowRatio);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

	const glm::vec3& GetWorldPosition() const;
	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;
	
private:
	void CalculateProjectionMatrix(float height, float windowRatio);
	void CalculateViewMatrix();
	void RegisterInputEvents();

	Transform& GetCurrentActiveCamera();

private:	
	Transform m_DebugCameraTransform;
	Transform m_RenderCameraTransform;
	
	glm::vec3 m_RenderCameraWorldPosition;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
};
