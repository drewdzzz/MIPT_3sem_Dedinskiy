#pragma once

#include <queue>
#include <map>
#include <stack>
#include <functional>
#define GLEW_STATIC
#include <GL/glew.h>		
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include "shaders/Shader.h"

static const char* TEXTURE_VERTEX_SHADER_PATH = "shaders/textures.vs";
static const char* TEXTURE_FRAG_SHADER_PATH = "shaders/textures.frag";
static const char* PRIMITIVE_VERTEX_SHADER_PATH = "shaders/primitive.vs";
static const char* PRIMITIVE_FRAG_SHADER_PATH = "shaders/primitive.frag";

struct Shaders{
	Shader textureShader;
	Shader primitiveShader;

	Shaders(const char* textureVs, const char* textureFrag, const char* primVs, const char* primFrag):
	textureShader(textureVs, textureFrag),
	primitiveShader(primVs, primFrag)
	{}
};

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

	Point operator*(double k) const {
		Point ans;
		ans.x = k*x;
		ans.y = k*y;
		return ans;
	}
};

struct viewPortState {
	int x;
	int y;
	int width;
	int height;

	viewPortState(int x, int y, int width, int height):
	x(x),
	y(y),
	width(width),
	height(height)
	{}

	viewPortState(const Point& point, int width, int height):
	x(point.x),
	y(point.y),
	width(width),
	height(height)
	{}
};

void windrewsViewPort(const viewPortState& state) {
	glViewport(state.x, state.y, state.width, state.height);
}

void windrewsViewPort(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

// Point leftBottomToMiddle(const Point& point, int screen_width, int screen_height) {

// }

// Point middleToLeftBottom(const Point& point, int screen_width, int screen_height) {
// 	Point new_point;
// 	new_point.x = (point.x + 1) * screen_width / 2;
// 	new_point.y = (point.y + 1) * screen_height / 2;
// 	return new_point;
// }

Point mouseToMiddle(const Point& point, viewPortState state) {
	Point ans;
	ans.x = 2 * point.x / state.width - 1.0;
	ans.y = 2 * point.y / state.height - 1.0;
	return ans;
}


struct keyAction {
	int key = -1;
	int action = -1;
};

enum KEY_STATES {
	PRESS = GLFW_PRESS,
	RELEASE = GLFW_RELEASE
};

class WindowStat {
	int screen_height;
	Point mousePos;
	keyAction key;
public:

	Point getMousePos(const viewPortState& state) const {
		Point res = mousePos;
		res.y = screen_height - res.y;
		res.x -= state.x;
		res.y -= state.y;
		return mouseToMiddle(res, state);
	}

	keyAction getKeyAction() const {
		return key;
	}

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

	Point getMousePos(GLFWwindow* window, int screen_heigth, int screen_width) {
		double x;
		double y;
		glfwGetCursorPos(window, &mousePos.x, &mousePos.y);
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
		status.screen_height = screen_heigth;
		status.mousePos = getMousePos(window, screen_heigth, screen_width);
		return status;
	}
};

std::queue<keyAction> EventManager::keys;


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

	WindowNode* make_underwindow (AbstractWindow* new_window, WindowNode* node);

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

	Windrew(GLFWwindow* window, Color color, int heigth, int width): 
	WindowsTree(window, color, heigth, width), 
	events(window, heigth, width),
	shaders(TEXTURE_VERTEX_SHADER_PATH, TEXTURE_FRAG_SHADER_PATH, PRIMITIVE_VERTEX_SHADER_PATH, PRIMITIVE_FRAG_SHADER_PATH) {
		viewPorts.emplace(0, 0, width, heigth);
	}
	
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

	void draw ();
};

GLFWwindow* initCreateContextWindow(int argc, char** argv, int heigth, int width) {
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	glutInit(&argc, argv);

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(width, heigth, "Graphics", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}

	return window;
}

Windrew* windrewsInit(int argc, char** argv, Color color, int heigth, int width) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = initCreateContextWindow(argc, argv, heigth, width);
	if (window == nullptr)
		return nullptr;

	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	glfwSetCursor(window, cursor);

	return new Windrew (window, color, heigth, width);
}

void windrewsTerminate() {
	glfwTerminate();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////WINDOWS TREE IMPLEMENTATION///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
WindowsTree::WindowsTree (GLFWwindow* window, Color color, int screen_height, int screen_width): 
	main_window (window), 
	bg_color (color),
	screen_height (screen_height),
	screen_width (screen_width),
	startPoint (0, 0) {}

	WindowsTree::~WindowsTree () = default;

	WindowNode*  WindowsTree::make_underwindow (AbstractWindow* new_window, WindowNode* node) {
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

	void Windrew::draw () {
		glViewport(0, 0, screen_width, screen_height);
		bg_color.set_as_bg ();
		for (auto& w: UnderWindows)
			w->draw (shaders);
		glfwSwapBuffers(main_window);
	}

	void WindowsTree::move(const Point& moveVec) {
		startPoint += moveVec;
		for (auto w: UnderWindows)
			w->move(moveVec);
	}

	void WindowsTree::addViewPort(const viewPortState& state) {
		viewPorts.push(state);
	}

	viewPortState WindowsTree::getViewPort() {
		return viewPorts.top();
	}

	void WindowsTree::rmViewPort() {
		viewPorts.pop();
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////WINDOW NODE IMPLEMENTATION///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WindowNode::WindowNode(AbstractWindow* new_window, WindowsTree* new_main_window): 
	window (new_window), 
	main_window (new_main_window) {};
	
	void WindowNode::moveUnderWindows(const Point& moveVec) {
		for (auto w:UnderWindows)
			w->move(moveVec);
	}

	void WindowNode::move(const Point& moveVec) {
		window->move(moveVec, main_window->getViewPort());
		if (!window->changeViewPort())
			moveUnderWindows(moveVec);
	}

	void WindowNode::draw(const Shaders& shaders) {
		if (window->changeViewPort()) {
			main_window->addViewPort(window->getViewPortChange());
		}

		auto state = main_window->getViewPort();
		window->draw(state, shaders);
		for (auto w: UnderWindows)
			w->draw(shaders);

		if (window->changeViewPort()) {
			main_window->rmViewPort();
			windrewsViewPort(main_window->getViewPort());
		}
	}

	void WindowNode::pollEvents(const WindowStat& status) {
		if (window->changeViewPort()) {
			main_window->addViewPort(window->getViewPortChange());
		}

		auto state = main_window->getViewPort();
		window->callback(status, state);
		for (auto w: UnderWindows)
			w->pollEvents(status);

		if (window->changeViewPort()) {
			main_window->rmViewPort();
			windrewsViewPort(main_window->getViewPort());
		}
	}