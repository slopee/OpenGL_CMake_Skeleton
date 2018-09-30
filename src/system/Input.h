#pragma once
#include <functional>

class GLFWwindow;
class InputRecognizer;
struct BaseInputEvent;

class Input
{
public:
	static Input& GetInstance();
	static void RegisterToGlfwCallbacks(GLFWwindow* window);

	template<typename  T>
	void RegisterInputEvent(std::function<void(const T&)> func, int glfwInputValue = -1);

private:
	struct BaseWrapper {
		virtual ~BaseWrapper() = default;
		virtual void operator()(const BaseInputEvent*) = 0;
	};

	template<typename T = BaseInputEvent>
	struct EventWrapper : BaseWrapper {
		std::function<void(T)> fn;

		virtual ~EventWrapper() = default;

		void operator()(const BaseInputEvent* e) override
		{
			fn(*static_cast<const T*>(e));
		}
	};

private:
	Input();
	~Input();

	static void OnSetKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnMouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void OnMouseMovedCallback(GLFWwindow* window, double xPos, double yPos);

	void RegisterCallback(std::unique_ptr<BaseWrapper> eventWrapper, int mapKey);
	void TriggerCallback(const BaseInputEvent& event, int mapKey);
	void TriggerInputRecognizer(std::function<void(InputRecognizer&)> eventTrigger);

	void OnKeyPressed(int key);
	void OnKeyReleased(int key);
	void OnMouseButtonPressed(int button, double xPos, double yPos);
	void OnMouseButtonReleased(int button, double xPos, double yPos);
	void OnMouseMoved(double xPos, double yPos);
	void OnMouseWheelScrolledVertically(double offset);

private:
	std::unordered_map<int, std::vector<std::unique_ptr<BaseWrapper>>> m_CallbackMap;
	std::vector<std::unique_ptr<InputRecognizer>> m_InputRecognizers;
};
