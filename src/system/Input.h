#pragma once
#include <functional>

class GLFWwindow;

class Input
{
public:
	struct InputEvent
	{		
	};

	struct MouseScroll : InputEvent
	{
		enum Direction
		{
			UP,
			DOWN
		};

		Direction direction;
		MouseScroll(Direction d) : direction(d) {}
	};

public:
	static Input& GetInstance();
	static void RegisterToGlfwCallbacks(GLFWwindow* window);

	void RegisterCallbackOnKeyPressed(std::function<void(const InputEvent&)> func, int key);
	void RegisterOnMouseWheel(std::function<void(const MouseScroll&)> func);

private:	
	struct BaseWrapper {
		virtual void operator()(const InputEvent const*) = 0;
		virtual ~BaseWrapper() = default;
	};

	template<typename T = InputEvent>
	struct EventWrapper : BaseWrapper {
		std::function<void(T)> fn;
		virtual void operator()(const InputEvent const* e) {
			fn(*static_cast<const T*>(e));
		}
		virtual ~EventWrapper() = default;
	};

private:
	Input() = default;
	~Input();

	static void OnSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	void RegisterCallback(std::unique_ptr<BaseWrapper> eventWrapper, int mapKey);
	void OnKeyPressed(int key);	
	void OnMouseWheelScrolledVertically(double offset);

private:
	std::unordered_map<int, std::vector<std::unique_ptr<BaseWrapper>>> m_CallbackMap;
};
