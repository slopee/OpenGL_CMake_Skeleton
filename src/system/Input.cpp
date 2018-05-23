#include "Input.h"
#include <GLFW/glfw3.h>

Input& Input::GetInstance()
{
	static Input instance;
	return instance;
}

void Input::OnSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		GetInstance().OnKeyPressed(key);
	}
}

void Input::OnKeyPressed(int key)
{
	auto searchKey = m_CallbackMap.find(key);
	if (searchKey != m_CallbackMap.end())
	{
		for(auto& callback : searchKey->second)
		{
			callback();
		}
	}
}

void Input::RegisterCallbackOnKeyPressed(std::function<void()> func, int key)
{
	auto searchKey = m_CallbackMap.find(key);
	if(searchKey != m_CallbackMap.end())
	{
		auto keyVector = searchKey->second;
		keyVector.push_back(func);
	}
	else
	{
		m_CallbackMap.insert({ key, std::vector<std::function<void()>> {func} });
	}
}
