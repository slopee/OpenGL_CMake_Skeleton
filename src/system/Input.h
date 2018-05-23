#pragma once
#include <functional>

class GLFWwindow;

class Input
{
public:
	static Input& GetInstance();
	static void OnSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);	
	void RegisterCallbackOnKeyPressed(std::function<void ()> func, int key);

private:
	void OnKeyPressed(int key);

private:
	std::unordered_map<int, std::vector<std::function<void()>>> m_CallbackMap;
};
