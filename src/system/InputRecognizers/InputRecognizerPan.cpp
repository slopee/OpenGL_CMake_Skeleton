#include "InputRecognizerPan.h"
#include "GLFW/glfw3.h"

//---------------------------------------------------------------------------------------------------------------------
bool InputRecognizerPan::IsEventRecognized()
{
	return m_RightButtonPressed;
}

//---------------------------------------------------------------------------------------------------------------------
const BaseInputEvent* InputRecognizerPan::GetInputEvent() const
{
	return &m_MousePan;
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerPan::OnMouseButtonPressed(int button, double xPos, double yPos)
{
	if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_RightButtonPressed = true;
		m_LastXPosition = xPos;
		m_LastYPosition = yPos;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerPan::OnMouseButtonReleased(int button, double xPos, double yPos)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		m_RightButtonPressed = false;
	}
}

//---------------------------------------------------------------------------------------------------------------------
void InputRecognizerPan::OnMouseMoved(double xPos, double yPos)
{
	if(m_RightButtonPressed)
	{
		m_MousePan.xDelta = (xPos - m_LastXPosition);
		m_MousePan.yDelta = (yPos - m_LastYPosition);

		m_LastXPosition = xPos;
		m_LastYPosition = yPos;
	}
}