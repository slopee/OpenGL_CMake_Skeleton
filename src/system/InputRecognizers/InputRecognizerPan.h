#pragma once
#include "InputRecognizer.h"

class InputRecognizerPan : public InputRecognizer
{
public:
	InputRecognizerPan() = default;

	bool IsEventRecognized() override;
	const BaseInputEvent* GetInputEvent() const override;

	void OnMouseButtonPressed(int button, double xPos, double yPos) override;
	void OnMouseButtonReleased(int button, double xPos, double yPos) override;
	void OnMouseMoved(double xPos, double yPos) override;

private:
	MousePan m_MousePan;
	bool m_RightButtonPressed = false;
	double m_LastXPosition = 0.0f;
	double m_LastYPosition = 0.0f;
};
