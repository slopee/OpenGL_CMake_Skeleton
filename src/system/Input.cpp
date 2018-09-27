#include "Input.h"
#include <GLFW/glfw3.h>

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
}

//---------------------------------------------------------------------------------------------------------------------
void Input::RegisterCallbackOnKeyPressed(std::function<void(const InputEvent&)> func, int key)
{
	auto eventWrapper = std::make_unique<EventWrapper<>>();
	eventWrapper->fn = func;

	RegisterCallback(std::move(eventWrapper), key);
}

//---------------------------------------------------------------------------------------------------------------------
void Input::RegisterOnMouseWheel(std::function<void(const MouseScroll&)> func)
{
	auto eventWrapper = std::make_unique<EventWrapper<MouseScroll>>();
	eventWrapper->fn = func;

	RegisterCallback(std::move(eventWrapper), GLFW_MOUSE_BUTTON_MIDDLE);
}

//---------------------------------------------------------------------------------------------------------------------
Input::~Input()
{
	for(auto& item : m_CallbackMap)
	{
		item.second.clear();
	}

	m_CallbackMap.clear();
}


//---------------------------------------------------------------------------------------------------------------------
void Input::OnSetKeyCallback(GLFWwindow* /* window */, int key, int /* scancode */ , int action, int /* mods */)
{
	if (action == GLFW_PRESS)
	{
		GetInstance().OnKeyPressed(key);
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseScrollCallback(GLFWwindow* /* window */, double /* xOffset */, double yOffset)
{
	if(yOffset != 0)
	{
		GetInstance().OnMouseWheelScrolledVertically(yOffset);
	}
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
void Input::OnKeyPressed(int key)
{
	auto searchKey = m_CallbackMap.find(key);
	const auto inputEvent = InputEvent();
	
	if (searchKey != m_CallbackMap.end())
	{
		for(auto& callback : searchKey->second)
		{
			callback->operator()(&inputEvent);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
void Input::OnMouseWheelScrolledVertically(double offset)
{
	auto searchKey = m_CallbackMap.find(GLFW_MOUSE_BUTTON_MIDDLE);

	auto direction = MouseScroll::UP;
	if(offset == -1)
	{
		direction = MouseScroll::DOWN;
	}

	const auto inputEvent = MouseScroll(direction);	
	if (searchKey != m_CallbackMap.end())
	{
		for (auto& callback : searchKey->second)
		{
			callback->operator()(&inputEvent);
		}
	}
}
