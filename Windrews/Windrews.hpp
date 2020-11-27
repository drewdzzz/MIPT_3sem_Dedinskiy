#pragma once

#include <queue>
#include <map>
#include <functional>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

struct Point{
	double x;
	double y;

	Point() = default;
	Point(const Point& other) = default;
	Point(Point&& other) = default;
	Point& operator=(const Point& other) = default;
	Point& operator=(Point&&) = default;

	Point(double x, double y): x(x), y(y) {
	}	

	void set() const {
		glVertex2d(x, y);
	}

	Point& operator+=(const Point& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	} 
};

struct keyAction {
	int key = 0;
	int action = 0;
};

enum KEY_STATES {
	PRESS = GLFW_PRESS,
	RELEASE = GLFW_RELEASE
};

class WindowStat {
public:
	Point mousePos;
	keyAction key;
};

#include "include/DrawTools.hpp"
#include "include/TextTools.hpp"
#include "include/Buttons.hpp"

class EventManager {
	GLFWwindow* window;
	int screen_heigth;
	int screen_width;

	Point getMousePos(GLFWwindow* window, int screen_heigth, int screen_width) {
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		mousePos.x = 2 * x / screen_width - 1.0;
		mousePos.y = -2 * y / screen_heigth + 1.0;	
		return mousePos;
	}

	static void getKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
		keys.push({key, action});
	}

	static void getMouseKeys(GLFWwindow* window, int key, int action, int mods) {
		keys.push({key, action});
	}

public:

	EventManager(GLFWwindow* window, int screen_heigth, int screen_width):
	window (window),
	screen_heigth (screen_heigth),
	screen_width (screen_width) {
		glfwSetKeyCallback(window, getKeys);
		glfwSetMouseButtonCallback(window, getMouseKeys);
	}

	Point mousePos;
	static std::queue<keyAction> keys;

	WindowStat getEvent() {
		WindowStat status;
		if (!keys.empty()){
			status.key = keys.front();
			keys.pop();
		}
		status.mousePos = getMousePos(window, screen_heigth, screen_width);
		return status;
	}
};

std::queue<keyAction> EventManager::keys;


class WindowsTree;

struct bad_main_window {
	WindowsTree* actual_main_window;
	WindowsTree* current_main_window;

	bad_main_window (WindowsTree* actual_main_window, WindowsTree* current_main_window): 
	actual_main_window (actual_main_window), 
	current_main_window(current_main_window) {}	
};

class WindowNode {
public:
	WindowsTree* main_window;
	AbstractWindow* window;
	std::vector<WindowNode*> UnderWindows;

	WindowNode(AbstractWindow* new_window, WindowsTree* new_main_window): 
	window (new_window), 
	main_window (new_main_window) {};

	void move(const Point& moveVec) {
		window->move(moveVec);
		for (auto w:UnderWindows)
			w->move(moveVec);
	}

	void draw() {
		window->draw ();
		for (auto w: UnderWindows)
			w->draw ();
	}

	void pollEvents(const WindowStat& status) {
		window->callback(status);
		for (auto w: UnderWindows)
			w->pollEvents(status);
	}
};

class WindowsTree {
public:
	Point startPoint;
	Color bg_color;
	GLFWwindow* main_window;
	std::vector<WindowNode*> UnderWindows;
	int screen_height;
	int screen_width;

	WindowsTree (GLFWwindow* window, Color color, int screen_height, int screen_width): 
	main_window (window), 
	bg_color (color),
	screen_height (screen_height),
	screen_width (screen_width),
	startPoint (0, 0) {}

	~WindowsTree () = default;
	WindowsTree (const WindowsTree& other) = delete;
	WindowsTree (WindowsTree&& other) = delete;
	WindowsTree& operator= (const WindowsTree& other) = delete;
	WindowsTree& operator= (WindowsTree&& other) = delete;

	WindowNode* make_underwindow (AbstractWindow* new_window, WindowNode* node) {
		WindowNode* res = nullptr;
		
		if (node == nullptr)
			UnderWindows.push_back (res = new WindowNode (new_window, this));
		else {
			if (node->main_window != this)
				throw bad_main_window (node->main_window, this);
			else
				node->UnderWindows.push_back (res = new WindowNode (new_window, this));
		}

		return res;
	}

	void draw () {
		bg_color.set_as_bg ();
		for (auto& w: UnderWindows)
			w->draw ();
		glfwSwapBuffers(main_window);
	}

	void move(const Point& moveVec) {
		startPoint += moveVec;
		for (auto w: UnderWindows)
			w->move(moveVec);
	}
};

class Windrew: public WindowsTree {
private:
	EventManager events;
public:
	Windrew(GLFWwindow* window, Color color, int heigth, int width): 
	WindowsTree(window, color, heigth, width), 
	events(window, heigth, width) {}
	
	~Windrew () = default;
	Windrew (const WindowsTree& other) = delete;
	Windrew (WindowsTree&& other) = delete;
	Windrew& operator= (const WindowsTree& other) = delete;
	Windrew& operator= (WindowsTree&& other) = delete;

	bool shouldClose() {
		return glfwWindowShouldClose(main_window);
	}

	void pollEvents() {
		glfwPollEvents();
		WindowStat status = events.getEvent();
		for (auto w: UnderWindows)
			w->pollEvents(status);
	}

	Point getStartPoint() {
		return startPoint;
	}
};

GLFWwindow* initCreateContextWindow(int argc, char** argv, int heigth, int width) {
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

	glutInit(&argc, argv);

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(width, heigth, "Graphics", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	return window;
}

Windrew* windrewsInit(int argc, char** argv, Color color, int heigth, int width) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = initCreateContextWindow(argc, argv, heigth, width);

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	glfwSetCursor(window, cursor);

	if (window == nullptr)
		return nullptr;

	return new Windrew (window, color, heigth, width);
}

void windrewsTerminate() {
	glfwTerminate();
}