#include "Camera.h"
#include "debug/Axis.h"
#include "system/Input.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "system/InputEvent.h"
#include "debug/CameraDebugDrawer.h"
#include <glm\gtx\transform.hpp>

static const float PAN_MULTIPLIER = 0.01f;
static const float ZOOM_DISTANCE = 0.5f;
static const float NEAR_DIST = 0.1f;
static const float FAR_DIST = 100.0f;
static const float ROTATION_MULTIPIER = 0.01f;

using namespace glm;

static float fov = 45.0f;
static float ratio = 0.5f;
static bool isDebugCameraOn = false;
static Axis* debugAxis;
static CameraDebugDrawer* cameraDebugDrawer = nullptr;

//---------------------------------------------------------------------------------------------------------------------
Camera::LookAtVectors::LookAtVectors()
	: position(0.0f, 0.0f, -15.0f), viewDirection(0.0f, 0.0f, 1.0f), up(0.0f, 1.0f, 0.0f)
{
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 Camera::LookAtVectors::CalculateRight() const
{
	return glm::normalize(glm::cross(viewDirection, up));
}

//---------------------------------------------------------------------------------------------------------------------
Camera::Camera(float height, float windowRatio)	
{
	/*
	m_DebugCameraTransform.position.z = -15.0f;
	m_DebugCameraTransform.rotation.y = 90.0f;

	m_RenderCameraTransform.position.z = -31.0f;
	*/
	//m_RenderCameraTransform.position.y = 10.0f;
	//m_RenderCameraTransform.position.x = -25.0f;
	//m_RenderCameraTransform.rotation.x = -45.0f;
	//m_RenderCameraTransform.rotation.z = -65.0f;

	debugAxis = new Axis();	

	CalculateProjectionMatrix(height, windowRatio);
	CalculateViewMatrix();

	cameraDebugDrawer = new CameraDebugDrawer(m_RenderCameraLookAt, fov, NEAR_DIST, FAR_DIST, ratio);

	RegisterInputEvents();
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::Draw(float time, const glm::mat4& projection, const glm::mat4& view)
{	
	if(isDebugCameraOn)
	{				
		cameraDebugDrawer->Draw(time, projection, view);
		//debugAxis->Draw(time, projection, m_RenderCameraTransform.rotation);
	}
}

//---------------------------------------------------------------------------------------------------------------------
const glm::vec3& Camera::GetWorldPosition() const
{
	return m_RenderCameraLookAt.position;
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
	fov = float(2.0*atan(height / 1920.f));
	ratio = windowRatio;
	m_ProjectionMatrix = perspective(fov, windowRatio, NEAR_DIST, FAR_DIST);

	if(cameraDebugDrawer != nullptr && (cameraDebugDrawer != nullptr))
	{
		cameraDebugDrawer->Update(m_RenderCameraLookAt, fov, NEAR_DIST, FAR_DIST, ratio);
	}	
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	auto lookAt = GetCurrentActiveCameraLookAt();
	m_ViewMatrix = glm::lookAt(lookAt.position, lookAt.position + lookAt.viewDirection, lookAt.up);

	if (!isDebugCameraOn && (cameraDebugDrawer != nullptr))
	{	
		cameraDebugDrawer->Update(m_RenderCameraLookAt, fov, NEAR_DIST, FAR_DIST, ratio);
	}	
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::RegisterInputEvents()
{
	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		if (key.pressed)
		{
			isDebugCameraOn = !isDebugCameraOn;
		}
		CalculateViewMatrix();
	}, GLFW_KEY_D);

	Input::GetInstance().RegisterInputEvent<MouseScroll>([&](const MouseScroll& scroll)
	{		
		auto& cameraLookAt = GetCurrentActiveCameraLookAt();
		if (scroll.direction == MouseScroll::DOWN)
		{
			cameraLookAt.position -= ZOOM_DISTANCE * cameraLookAt.viewDirection;
		}
		else
		{
			cameraLookAt.position += ZOOM_DISTANCE * cameraLookAt.viewDirection;
		}
		CalculateViewMatrix();		
	});

	Input::GetInstance().RegisterInputEvent<MouseRotation>([&](const MouseRotation& rotation)
	{
		auto& cameraLookAt = GetCurrentActiveCameraLookAt();

		glm::mat4 xRotation;
		glm::mat4 yRotation;

		if (rotation.xDelta != 0)
		{
			xRotation = glm::rotate(static_cast<float>(-rotation.xDelta * ROTATION_MULTIPIER), cameraLookAt.up);
		}

		if (rotation.yDelta != 0)
		{
			const auto right = cameraLookAt.CalculateRight();
			yRotation = glm::rotate(static_cast<float>(-rotation.yDelta * ROTATION_MULTIPIER), right);
			cameraLookAt.up = glm::mat3(yRotation) * cameraLookAt.up;
		}

		cameraLookAt.viewDirection = glm::mat3(xRotation * yRotation) * cameraLookAt.viewDirection;
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<MousePan>([&](const MousePan& pan)
	{		
		auto& cameraLookAt = GetCurrentActiveCameraLookAt();
		cameraLookAt.position += static_cast<float>((pan.xDelta * PAN_MULTIPLIER)) * cameraLookAt.CalculateRight();
		cameraLookAt.position += static_cast<float>((-pan.yDelta * PAN_MULTIPLIER)) * cameraLookAt.up;
		CalculateViewMatrix();
		
	});

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		/*
		m_RenderCameraTransform.rotation.x = 0.0f;
		m_RenderCameraTransform.rotation.y = 0.0f;
		*/
		CalculateViewMatrix();
	}, GLFW_KEY_0);

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		/*
		m_RenderCameraTransform.rotation.x = -90.0f;
		m_RenderCameraTransform.rotation.y = 0.0f;
		*/
		CalculateViewMatrix();
	}, GLFW_KEY_1);

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		/*
		m_RenderCameraTransform.rotation.y = -90.0f;
		*/
		CalculateViewMatrix();
	}, GLFW_KEY_2);
}

//---------------------------------------------------------------------------------------------------------------------
Camera::LookAtVectors& Camera::GetCurrentActiveCameraLookAt()
{
	if(isDebugCameraOn)
	{
		return m_DebugCameraLookAt;
	}

	return m_RenderCameraLookAt;
}
