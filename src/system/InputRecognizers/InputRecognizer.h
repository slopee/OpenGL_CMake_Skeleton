#pragma once
#include "system/InputEvent.h"

struct BaseInputEvent;

class InputRecognizer
{
public:
	virtual ~InputRecognizer() = default;
	virtual bool IsEventRecognized() = 0;
	virtual const BaseInputEvent* GetInputEvent() const = 0;

	virtual void OnKeyPressed(int key);
	virtual void OnKeyReleased(int key);
	virtual void OnMouseVerticalScroll(MouseScroll::Direction direction);
	virtual void OnMouseButtonPressed(int button, double xPos, double yPos);
	virtual void OnMouseButtonReleased(int button, double xPos, double yPos);
	virtual void OnMouseMoved(double xPos, double yPos);
};
