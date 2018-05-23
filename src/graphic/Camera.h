#pragma once
#include <glm/mat4x3.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera
{
public:
	Camera();

	glm::mat4 GetProjectionMatrix(float height, float windowRatio) const;
	glm::mat4 GetViewMatrix() const;
	
private:	
	float m_Distance;
	float m_Theta;
	float m_Phi;
};
