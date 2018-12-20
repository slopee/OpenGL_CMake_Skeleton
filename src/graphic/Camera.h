#pragma once
#include <glm/mat4x3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Transform.h"

class CameraDebugDrawer;

class Camera
{
	struct LookAtVectors
	{
		glm::vec3 position;
		glm::vec3 viewDirection;
		glm::vec3 up;

		LookAtVectors();
		glm::vec3 CalculateRight() const;
	};

public:
	Camera(float height, float windowRatio);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

	const glm::vec3& GetWorldPosition() const;
	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;

	const glm::mat4& GetRenderViewMatrix() const;
	const glm::mat4& GetRenderProjectionMatrix() const;
	
private:
	void CalculateProjectionMatrix(float height, float windowRatio);
	void CalculateViewMatrix();
	void RegisterInputEvents();
	LookAtVectors& GetCurrentActiveCameraLookAt();
	
private:	
	LookAtVectors m_DebugCameraLookAt;
	LookAtVectors m_RenderCameraLookAt;
	
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	friend CameraDebugDrawer;
};
