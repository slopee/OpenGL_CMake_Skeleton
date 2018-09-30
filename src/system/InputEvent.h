#pragma once

struct BaseInputEvent
{
	enum Type
	{
		KeyEvent,
		MouseButtonEvent,
		MouseScroll,
		MouseRotation,
		MouseMoved,
		MousePan,
		Invalid
	};

	virtual Type GetEventId() const = 0;
	virtual ~BaseInputEvent() = default;
};

//---------------------------------------------------------------------------------------------------------------------
template<typename T, BaseInputEvent::Type _Id>
struct InputEvent : BaseInputEvent
{	
	static constexpr BaseInputEvent::Type ID = _Id;

	BaseInputEvent::Type GetEventId() const override
	{
		return _Id;
	}
};

//---------------------------------------------------------------------------------------------------------------------
struct KeyEvent : InputEvent<KeyEvent, BaseInputEvent::KeyEvent>
{
	bool pressed;
	bool released;
	int key;
	
	static KeyEvent KeyPressedEvent(int key)
	{
		return KeyEvent(true, false, key);
	}

	static KeyEvent KeyReleasedEvent(int key)
	{
		return KeyEvent(false, true, key);
	}


private:
	KeyEvent(bool pressed, bool released, int key)
		: pressed(pressed),
		  released(released),
		  key(key)
	{
	}
};

//---------------------------------------------------------------------------------------------------------------------
struct MouseButtonEvent : InputEvent<MouseButtonEvent, BaseInputEvent::MouseButtonEvent>
{
	bool pressed;
	bool released;
	int button;
	double xPos;
	double yPos;

	static MouseButtonEvent ButtonPressedEvent(int button, double xPos, double yPos)
	{
		return MouseButtonEvent(true, false, button, xPos, yPos);
	}

	static MouseButtonEvent ButtonReleasedEvent(int button, double xPos, double yPos)
	{
		return MouseButtonEvent(false, true, button, xPos, yPos);
	}
	
private:
	MouseButtonEvent(bool pressed, bool released, int button, double xPos, double yPos)
		: pressed(pressed),
		released(released),
		button(button),
		xPos(xPos),
		yPos(yPos)
	{
	}
};

//---------------------------------------------------------------------------------------------------------------------
struct MouseScroll : InputEvent<MouseButtonEvent, BaseInputEvent::MouseScroll>
{
	enum Direction
	{
		UP,
		DOWN
	};

	Direction direction;
	MouseScroll(Direction d) : direction(d) {}
};

//---------------------------------------------------------------------------------------------------------------------
struct MouseRotation : InputEvent<MouseRotation, BaseInputEvent::MouseRotation>
{
	double xDelta;
	double yDelta;

	MouseRotation() : xDelta(0), yDelta(0) {}

	MouseRotation(double delta, double delta1)
		: xDelta(delta),
		  yDelta(delta1)
	{
	}
};

//---------------------------------------------------------------------------------------------------------------------
struct MouseMoved : InputEvent<MouseMoved, BaseInputEvent::MouseMoved>
{
	double xPos;
	double yPos;
	
	MouseMoved(double pos, double pos1)
		: xPos(pos),
		  yPos(pos1)
	{
	}
};

//---------------------------------------------------------------------------------------------------------------------
struct MousePan : InputEvent<MouseMoved, BaseInputEvent::MousePan>
{
	double xDelta;
	double yDelta;

	MousePan() : xDelta(0), yDelta(0) {}

	MousePan(double xDelta, double yDelta)
		: xDelta(xDelta),
		yDelta(yDelta)
	{
	}
};