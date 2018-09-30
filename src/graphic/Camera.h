#pragma once
#include <glm/mat4x3.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	Camera(float height, float windowRatio);

	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;

private:
	void CalculateProjectionMatrix(float height, float windowRatio);
	void CalculateViewMatrix();

private:	
	glm::vec3 m_Translation;
	float m_Theta;
	float m_Phi;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
};
