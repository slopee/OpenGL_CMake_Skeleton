#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "system/Input.h"
#include "GLFW/glfw3.h"
#include <iostream>

static const float INCREMENT_ANGLE = 5;

using namespace glm;

Camera::Camera() : 
	m_Distance(10),
	m_Theta(0),
	m_Phi(0)
{		
	Input::GetInstance().RegisterCallbackOnKeyPressed([&]()
	{
		m_Theta += INCREMENT_ANGLE;
		
	}, GLFW_KEY_A);

	Input::GetInstance().RegisterCallbackOnKeyPressed([&]()
	{
		m_Theta -= INCREMENT_ANGLE;
	}, GLFW_KEY_D);

	Input::GetInstance().RegisterCallbackOnKeyPressed([&]()
	{
		m_Phi += INCREMENT_ANGLE;
	}, GLFW_KEY_W);

	Input::GetInstance().RegisterCallbackOnKeyPressed([&]()
	{
		m_Phi -= INCREMENT_ANGLE;
	}, GLFW_KEY_S);
}

glm::mat4 Camera::GetProjectionMatrix(float height, float windowRatio) const
{
	return perspective(float(2.0*atan(height / 1920.f)), windowRatio, 0.1f, 100.f);
}

glm::mat4 Camera::GetViewMatrix() const
{	
	const auto cameraDistance = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -m_Distance));
	const auto phiRotation = glm::rotate(glm::mat4(), radians(m_Phi), vec3(1.0f, 0.0f, 0.0f));
	const auto thetaRotation = glm::rotate(glm::mat4(), radians(m_Theta), vec3(0.0f, 1.0f, 0.0f));

	return cameraDistance * phiRotation * thetaRotation;
}