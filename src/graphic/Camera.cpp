#include "Camera.h"
#include "debug/Axis.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "system/Input.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "system/InputEvent.h"
#include "debug/CameraDebugDrawer.h"

static const float PAN_MULTIPLIER = 0.01f;
static const float ZOOM_DISTANCE = 0.5f;
static const float NEAR_DIST = 0.1f;
static const float FAR_DIST = 100.0f;

using namespace glm;

static float fov = 45.0f;
static float ratio = 0.5f;
static bool isDebugCameraOn = false;
static Axis* debugAxis;
static CameraDebugDrawer* cameraDebugDrawer = nullptr;

//---------------------------------------------------------------------------------------------------------------------
Camera::Camera(float height, float windowRatio)	
{
	m_DebugCameraTransform.position.z = -30.0f;
	m_DebugCameraTransform.rotation.y = 90.0f;
	m_RenderCameraTransform.position.z = 0.0f;

	debugAxis = new Axis();	

	CalculateProjectionMatrix(height, windowRatio);
	CalculateViewMatrix();

	cameraDebugDrawer = new CameraDebugDrawer(m_RenderCameraTransform, fov, NEAR_DIST, FAR_DIST, ratio);

	RegisterInputEvents();
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::Draw(float time, const glm::mat4& projection, const glm::mat4& view)
{
	//if(isDebugCameraOn)
	{
		debugAxis->Draw(time, projection, m_RenderCameraTransform.rotation);
		cameraDebugDrawer->Draw(time, projection, view);
	}
}

//---------------------------------------------------------------------------------------------------------------------
const glm::mat4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
const glm::mat4& Camera::GetViewMatrix() const
{	
	if(!isDebugCameraOn)
	{
		return m_ViewMatrix;
	}
	return m_ViewMatrix;
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::CalculateProjectionMatrix(float height, float windowRatio)
{
	fov = float(2.0*atan(height / 1920.f));
	ratio = windowRatio;
	m_ProjectionMatrix = perspective(fov, windowRatio, NEAR_DIST, FAR_DIST);

	if(cameraDebugDrawer != nullptr)
	{
		cameraDebugDrawer->Update(m_RenderCameraTransform, fov, NEAR_DIST, FAR_DIST, ratio);
	}	
}

//---------------------------------------------------------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_ViewMatrix = GetCurrentActiveCamera().GetTransformMatrix();	

	if (!isDebugCameraOn)
	{
		m_CameraDirection = glm::vec3(m_ViewMatrix[0][2], m_ViewMatrix[1][2], m_ViewMatrix[2][2]);
		m_CameraUp = glm::vec3(m_ViewMatrix[0][1], m_ViewMatrix[1][1], m_ViewMatrix[2][1]);

		if (cameraDebugDrawer != nullptr)
		{
			cameraDebugDrawer->Update(m_RenderCameraTransform, fov, NEAR_DIST, FAR_DIST, ratio);
		}
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
		auto& camera = GetCurrentActiveCamera();
		if (scroll.direction == MouseScroll::DOWN)
		{
			camera.position.z += ZOOM_DISTANCE;
		}
		else
		{
			camera.position.z -= ZOOM_DISTANCE;
		}
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<MouseRotation>([&](const MouseRotation& rotation)
	{
		auto& camera = GetCurrentActiveCamera();
		if (rotation.xDelta != 0)
		{
			camera.rotation.y += rotation.xDelta;
		}

		if (rotation.yDelta != 0)
		{
			camera.rotation.x += rotation.yDelta;
		}
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<MousePan>([&](const MousePan& pan)
	{
		auto& camera = GetCurrentActiveCamera();
		camera.position.x += pan.xDelta * PAN_MULTIPLIER;
		camera.position.y += pan.yDelta * PAN_MULTIPLIER;
		CalculateViewMatrix();
	});

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		m_RenderCameraTransform.rotation.x = 0.0f;
		m_RenderCameraTransform.rotation.y = 0.0f;
		CalculateViewMatrix();
	}, GLFW_KEY_0);

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		m_RenderCameraTransform.rotation.x = -90.0f;
		m_RenderCameraTransform.rotation.y = 0.0f;
		CalculateViewMatrix();
	}, GLFW_KEY_1);

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		m_RenderCameraTransform.rotation.y = -90.0f;
		CalculateViewMatrix();
	}, GLFW_KEY_2);
}

//---------------------------------------------------------------------------------------------------------------------
Transform& Camera::GetCurrentActiveCamera()
{
	if(isDebugCameraOn)
	{
		return m_DebugCameraTransform;
	}

	return m_RenderCameraTransform;
}
