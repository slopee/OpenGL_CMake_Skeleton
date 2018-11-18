#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "system/Input.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "system/InputEvent.h"

static const float PAN_MULTIPLIER = 0.01f;
static const float ZOOM_DISTANCE = 0.5f;

using namespace glm;

//---------------------------------------------------------------------------------------------------------------------
Camera::Camera(float height, float windowRatio) :
	m_Translation(0.0f, 0.0f, -10.0f),
	m_Theta(0),
	m_Phi(0)
{
	CalculateProjectionMatrix(height, windowRatio);
	CalculateViewMatrix();

	Input::GetInstance().RegisterInputEvent<MouseScroll>([&](const MouseScroll& scroll)
	{
		if(scroll.direction == MouseScroll::DOWN)
		{
			m_Translation.z += ZOOM_DISTANCE;
		}
		else
		{
			m_Translation.z -= ZOOM_DISTANCE;
		}		
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<MouseRotation>([&](const MouseRotation& rotation)
	{
		if(rotation.xDelta != 0)
		{
			m_Theta += rotation.xDelta;
		}

		if (rotation.yDelta != 0)
		{
			m_Phi += rotation.yDelta;
		}
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<MousePan>([&](const MousePan& pan)
	{		
		m_Translation.x += pan.xDelta * PAN_MULTIPLIER;
		m_Translation.y += pan.yDelta * PAN_MULTIPLIER;
		CalculateViewMatrix();
	});
}

//---------------------------------------------------------------------------------------------------------------------
const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
const glm::mat4& Camera::GetViewMatrix() const
{	
	

	return m_ViewMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::CalculateProjectionMatrix(float height, float windowRatio)
{
	m_ProjectionMatrix = perspective(float(2.0*atan(height / 1920.f)), windowRatio, 0.1f, 100.f);
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	//const auto translation = -m_Translation.z;
	const auto cameraDistance = glm::translate(glm::mat4(), m_Translation);
	const auto phiRotation = glm::rotate(glm::mat4(), radians(m_Phi), vec3(1.0f, 0.0f, 0.0f));
	const auto thetaRotation = glm::rotate(glm::mat4(), radians(m_Theta), vec3(0.0f, 1.0f, 0.0f));

	m_ViewMatrix = cameraDistance * phiRotation * thetaRotation;
}
