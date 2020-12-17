#pragma once

#include <queue>
#include <map>
#include <stack>
#include <functional>
#include <type_traits>
#define GLEW_STATIC
#include <GL/glew.h>		
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include "shaders/Shader.h"

struct Shaders{
	Shader textureShader;
	Shader primitiveShader;

	Shaders(const char* textureVs, const char* textureFrag, const char* primVs, const char* primFrag);
};

struct bad_underwindow_type {};

struct Point{
	double x;
	double y;

	Point(double x, double y);
	Point();
	Point(const Point& other);
	Point(Point&& other) ;
	Point& operator=(const Point& other);
	Point& operator=(Point&&);

	bool operator<(const Point& other) const;
	Point& operator+=(const Point& other);
	Point operator*(double k) const;
};

struct viewPortState {
	int x;
	int y;
	int width;
	int height;

	viewPortState(int x, int y, int width, int height);
	viewPortState(const Point& point, int width, int height);
};

Point NORMAL_TO_PIXELS(const Point& other, const viewPortState& state);
Point PIXELS_TO_NORMAL(const Point& other, const viewPortState& state);

void windrewsViewPort(const viewPortState& state);

void windrewsViewPort(int x, int y, int width, int height);

Point mouseToMiddle(const Point& point, viewPortState state);


struct keyAction {
	int key = -1;
	int action = -1;
};

class WindowStat {
	int screen_height;
	Point mousePos;
	keyAction key;
public:

	Point getMousePos(const viewPortState& state) const;
	keyAction getKeyAction() const;

	friend class EventManager;
};

class WindowsTree;
class AbstractWindow;


class WindowNode {
public:
	WindowsTree* main_window;
	AbstractWindow* window;
	std::vector<WindowNode*> UnderWindows;

	WindowNode(AbstractWindow* new_window, WindowsTree* new_main_window);

	void moveUnderWindows(const Point& moveVec);

	void move(const Point& moveVec);

	void draw(const Shaders& shaders);

	void pollEvents(const WindowStat& status);
};

#include "include/DrawTools.hpp"
#include "include/TextTools.hpp"
#include "include/Buttons.hpp"

class EventManager {
	GLFWwindow* window;
	int screen_heigth;
	int screen_width;

	Point getMousePos(GLFWwindow* window, int screen_heigth, int screen_width);
	static void getKeys(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void getMouseKeys(GLFWwindow* window, int key, int action, int mods);

public:

	EventManager(GLFWwindow* window, int screen_heigth, int screen_width);

	Point mousePos;
	static std::queue<keyAction> keys;

	WindowStat getEvent();
};

class WindowNode;

class WindowsTree {
protected:
	std::stack<viewPortState> viewPorts;

public:
	Point startPoint;
	Color bg_color;
	GLFWwindow* main_window;
	std::vector<WindowNode*> UnderWindows;
	int screen_height;
	int screen_width;

	WindowsTree (GLFWwindow* window, Color color, int screen_height, int screen_width);
	~WindowsTree ();
	WindowsTree (const WindowsTree& other) = delete;
	WindowsTree (WindowsTree&& other) = delete;
	WindowsTree& operator= (const WindowsTree& other) = delete;
	WindowsTree& operator= (WindowsTree&& other) = delete;

	template<class WindowType, class... Args>
	WindowNode* make_underwindow (WindowNode* node, Args... args);

	void move(const Point& moveVec);

	void addViewPort(const viewPortState& state);
	viewPortState getViewPort();
	void rmViewPort();
};

struct bad_main_window {
	WindowsTree* actual_main_window;
	WindowsTree* current_main_window;

	bad_main_window (WindowsTree* actual_main_window, WindowsTree* current_main_window): 
	actual_main_window (actual_main_window), 
	current_main_window(current_main_window) {}	
};

class Windrew: public WindowsTree {
private:
	EventManager events;
public:
	Shaders shaders;

	Windrew(GLFWwindow* window, Color color, int heigth, int width);
	
	~Windrew ();
	Windrew (const WindowsTree& other) = delete;
	Windrew (WindowsTree&& other) = delete;
	Windrew& operator= (const WindowsTree& other) = delete;
	Windrew& operator= (WindowsTree&& other) = delete;

	bool shouldClose();

	void pollEvents();

	Point getStartPoint();

	void draw ();
};

GLFWwindow* initCreateContextWindow(int argc, char** argv, int heigth, int width);

Windrew* windrewsInit(int argc, char** argv, Color color, int heigth, int width);

void windrewsTerminate();


template<class WindowType, class... Args>
WindowNode* WindowsTree::make_underwindow (WindowNode* node, Args... args) {
	WindowNode* res = nullptr;
	if (!std::is_base_of<AbstractWindow, WindowType>::value) {
		throw bad_underwindow_type();
	}
	AbstractWindow* new_window = new WindowType(std::forward<Args>(args)...);
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