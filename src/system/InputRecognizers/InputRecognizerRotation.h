#pragma once
#include "InputRecognizer.h"

class InputRecognizerRotation : public InputRecognizer
{
public:
	InputRecognizerRotation() = default;

	bool IsEventRecognized() override;
	const BaseInputEvent* GetInputEvent() const override;

	void OnKeyPressed(int key) override;
	void OnKeyReleased(int key) override;
	void OnMouseButtonPressed(int button, double xPos, double yPos) override;
	void OnMouseButtonReleased(int button, double xPos, double yPos) override;
	void OnMouseMoved(double xPos, double yPos) override;

private:
	MouseRotation m_MouseRotation;
	bool m_AltPressed = false;
	bool m_LeftButtonPressed = false;
	double m_LastXPosition = 0.0f;
	double m_LastYPosition = 0.0f;
};
