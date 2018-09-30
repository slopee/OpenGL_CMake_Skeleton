#include "InputRecognizerRotation.h"
#include "GLFW/glfw3.h"

//---------------------------------------------------------------------------------------------------------------------
bool InputRecognizerRotation::IsEventRecognized()
{
	return m_AltPressed && m_LeftButtonPressed;
}

//---------------------------------------------------------------------------------------------------------------------
const BaseInputEvent* InputRecognizerRotation::GetInputEvent() const
{
	return &m_MouseRotation;
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerRotation::OnKeyPressed(int key)
{
	if(key == GLFW_KEY_LEFT_ALT)
	{
		m_AltPressed = true;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerRotation::OnKeyReleased(int key)
{
	if (key == GLFW_KEY_LEFT_ALT)
	{
		m_AltPressed = false;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerRotation::OnMouseButtonPressed(int button, double xPos, double yPos)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_LeftButtonPressed = true;
		m_LastXPosition = xPos;
		m_LastYPosition = yPos;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerRotation::OnMouseButtonReleased(int button, double xPos, double yPos)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		m_LeftButtonPressed = false;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerRotation::OnMouseMoved(double xPos, double yPos)
{
	if(m_AltPressed && m_LeftButtonPressed)
	{
		m_MouseRotation.xDelta = (xPos - m_LastXPosition);
		m_MouseRotation.yDelta = (yPos - m_LastYPosition);

		m_LastXPosition = xPos;
		m_LastYPosition = yPos;
	}
}