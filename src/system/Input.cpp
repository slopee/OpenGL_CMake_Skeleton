#include "Input.h"
#include <GLFW/glfw3.h>
#include "InputEvent.h"
#include "InputRecognizers/InputRecognizerRotation.h"
#include "InputRecognizers/InputRecognizerPan.h"

#define CUSTOM_INPUT_EVENTS	GLFW_KEY_LAST + 1

//---------------------------------------------------------------------------------------------------------------------
template void Input::RegisterInputEvent<KeyEvent>(std::function<void(const KeyEvent&)> func, int glfwInputValue);
template void Input::RegisterInputEvent<MouseButtonEvent>(std::function<void(const MouseButtonEvent&)> func, int glfwInputValue);
template void Input::RegisterInputEvent<MouseScroll>(std::function<void(const MouseScroll&)> func, int glfwInputValue);
template void Input::RegisterInputEvent<MouseRotation>(std::function<void(const MouseRotation&)> func, int glfwInputValue);
template void Input::RegisterInputEvent<MouseMoved>(std::function<void(const MouseMoved&)> func, int glfwInputValue);
template void Input::RegisterInputEvent<MousePan>(std::function<void(const MousePan&)> func, int glfwInputValue);

//---------------------------------------------------------------------------------------------------------------------
Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}

//---------------------------------------------------------------------------------------------------------------------
void Input::RegisterToGlfwCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, &Input::OnSetKeyCallback);
	glfwSetScrollCallback(window, &Input::OnMouseScrollCallback);
	glfwSetMouseButtonCallback(window, &Input::OnMouseButtonCallback);
	glfwSetCursorPosCallback(window, &Input::OnMouseMovedCallback);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void Input::RegisterInputEvent(std::function<void(const T&)> func, int glfwInputValue)
{
	static_assert(std::is_base_of<BaseInputEvent, T>::value, "T must derive from InputEvent");

	auto eventWrapper = std::make_unique<EventWrapper<T>>();
	eventWrapper->fn = func;

	auto callbackKey = glfwInputValue;
	if (callbackKey == -1)
	{
		callbackKey = CUSTOM_INPUT_EVENTS + T::ID;
	}

	RegisterCallback(std::move(eventWrapper), callbackKey);
}

//---------------------------------------------------------------------------------------------------------------------
Input::Input()
{
	m_InputRecognizers.push_back(std::make_unique<InputRecognizerRotation>());
	m_InputRecognizers.push_back(std::make_unique<InputRecognizerPan>());
}


//---------------------------------------------------------------------------------------------------------------------
Input::~Input()
{
	for (auto& item : m_CallbackMap)
	{
		item.second.clear();
	}

	m_CallbackMap.clear();
	m_InputRecognizers.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnSetKeyCallback(GLFWwindow* /* window */, int key, int /* scancode */, int action, int /* mods */)
{
	if (action == GLFW_PRESS)
	{
		GetInstance().OnKeyPressed(key);
	}
	else if (action == GLFW_RELEASE)
	{
		GetInstance().OnKeyReleased(key);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseScrollCallback(GLFWwindow* /* window */, double /* xOffset */, double yOffset)
{
	if (yOffset != 0)
	{
		GetInstance().OnMouseWheelScrolledVertically(yOffset);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int /* mods */)
{
	double xPos;
	double yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	if (action == GLFW_PRESS)
	{
		GetInstance().OnMouseButtonPressed(button, xPos, yPos);
	}
	else
	{
		GetInstance().OnMouseButtonReleased(button, xPos, yPos);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseMovedCallback(GLFWwindow* /* window */, double xPos, double yPos)
{
	GetInstance().OnMouseMoved(xPos, yPos);
}

//---------------------------------------------------------------------------------------------------------------------
void Input::RegisterCallback(std::unique_ptr<BaseWrapper> eventWrapper, int mapKey)
{
	auto searchKey = m_CallbackMap.find(mapKey);
	if (searchKey != m_CallbackMap.end())
	{
		auto keyVector = &searchKey->second;
		keyVector->push_back(std::move(eventWrapper));
	}
	else
	{
		auto vector = std::vector<std::unique_ptr<BaseWrapper>>();
		vector.push_back(std::move(eventWrapper));
		m_CallbackMap.insert({ mapKey, std::move(vector) });
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::TriggerCallback(const BaseInputEvent& event, int mapKey)
{
	auto searchKey = m_CallbackMap.find(mapKey);
	if (searchKey != m_CallbackMap.end())
	{
		for (auto& callback : searchKey->second)
		{
			callback->operator()(&event);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::TriggerInputRecognizer(std::function<void(InputRecognizer&)> eventTrigger)
{
	for (auto& inputRecognizer : m_InputRecognizers)
	{
		eventTrigger(*inputRecognizer);

		if (inputRecognizer->IsEventRecognized())
		{
			const auto inputEvent = inputRecognizer->GetInputEvent();
			TriggerCallback(*inputEvent, CUSTOM_INPUT_EVENTS + inputEvent->GetEventId());
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnKeyPressed(int key)
{
	const auto inputEvent = KeyEvent::KeyPressedEvent(key);
	TriggerCallback(inputEvent, key);

	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnKeyPressed(key);
	});
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnKeyReleased(int key)
{
	const auto inputEvent = KeyEvent::KeyReleasedEvent(key);
	TriggerCallback(inputEvent, key);
	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnKeyReleased(key);
	});
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseButtonPressed(int button, double xPos, double yPos)
{
	const auto inputEvent = MouseButtonEvent::ButtonPressedEvent(button, xPos, yPos);
	TriggerCallback(inputEvent, button);
	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnMouseButtonPressed(button, xPos, yPos);
	});
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseButtonReleased(int button, double xPos, double yPos)
{
	const auto inputEvent = MouseButtonEvent::ButtonReleasedEvent(button, xPos, yPos);
	TriggerCallback(inputEvent, button);
	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnMouseButtonReleased(button, xPos, yPos);
	});
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseMoved(double xPos, double yPos)
{
	const auto inputEvent = MouseMoved(xPos, yPos);
	TriggerCallback(inputEvent, CUSTOM_INPUT_EVENTS + BaseInputEvent::MouseMoved);
	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnMouseMoved(xPos, yPos);
	});
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseWheelScrolledVertically(double offset)
{
	auto direction = MouseScroll::UP;
	if (offset == -1)
	{
		direction = MouseScroll::DOWN;
	}

	const auto inputEvent = MouseScroll(direction);

	TriggerCallback(inputEvent, CUSTOM_INPUT_EVENTS + BaseInputEvent::MouseScroll);
	TriggerInputRecognizer([=](InputRecognizer& inputRecognizer)
	{
		inputRecognizer.OnMouseVerticalScroll(direction);
	});
}
