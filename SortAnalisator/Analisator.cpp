#include<cstdlib>
#include<iostream>
#include<string>
#include<queue>
#include<vector>
#include<GLFW/glfw3.h>
#include<GL/glut.h>
#include"Sort.hpp"
#include"IntCount.h"
#include"AnalisatorConstants.h"
#include"MeasureTools.hpp"
#include"DrawTools.hpp"
#include"TextTools.hpp"
#include"Buttons.hpp"
#include"Callbacks.hpp"

class WindowsTree;

struct bad_main_window {
	WindowsTree* actual_main_window;
	WindowsTree* current_main_window;

	bad_main_window (WindowsTree* actual_main_window, WindowsTree* current_main_window): 
	actual_main_window (actual_main_window), 
	current_main_window(current_main_window) {}	
};

class EventManager {
private:
	std::queue<AbstractWindow*> draw_que;
public:
	void PollEvents() {
		AbstractWindow* curr_window = nullptr;
		while (!draw_que.empty()) {
			curr_window = draw_que.back();
			draw_que.pop();
			curr_window->draw();
		}
	}
} MANAGER;

class WindowNode {
public:
	WindowsTree* main_window;
	AbstractWindow* window;
	std::vector<WindowNode*> UnderWindows;

	WindowNode (AbstractWindow* new_window, WindowsTree* new_main_window): window (new_window), main_window (new_main_window) {};

	void draw () {
		window->draw ();
		for (auto w: UnderWindows)
			w->draw ();
	}
};

class WindowsTree {
public:
	Color bg_color;
	GLFWwindow* main_window;
	std::vector<WindowNode*> UnderWindows;

	WindowsTree (GLFWwindow* window, Color color): main_window (window), bg_color (color) {}
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
		for (auto w: UnderWindows)
			w->draw ();
	}
} *MAIN_WINDOW;

Button QSORT_BUTTON({1.0, 0.0, 0.0}, {1.0, 0.451, 0.451}, {0.65, 0.0, 0.0}, AnConst::QUICKSORT_BUTTON_TEXT, AnConst::QUICKSORT_BUTTON_COORDS);

Button BSORT_BUTTON({0.0, 1.0, 0.0}, {0.451, 1.0, 0.451}, {0.0, 0.65, 0.0}, AnConst::BUBBLESORT_BUTTON_TEXT, AnConst::BUBBLESORT_BUTTON_COORDS);

void GetCursorPos(GLFWwindow* window, double* x, double* y) {
	glfwGetCursorPos(window, x, y);
	*x = 2 * *x / AnConst::SCREEN_WIDTH - 1.0;
	*y = -2 * *y / AnConst::SCREEN_HEIGHT + 1.0;

}

GLFWwindow* initCreateContextWindow(int argc, char** argv) {
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

	glutInit(&argc, argv);

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(AnConst::SCREEN_WIDTH, AnConst::SCREEN_HEIGHT, "Graphics", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	MAIN_WINDOW = new WindowsTree (window, colors::BACKGROUND);

	return window;
}

void main_loop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {

		MAIN_WINDOW->draw ();

		glfwPollEvents();

		glfwSwapBuffers(window);

	}
}

int main(int argc, char** argv) {
	srand(time(NULL));

	GLFWwindow* window = initCreateContextWindow(argc, argv);
	if (!window)
		return -1;

	// glfwSetKeyCallback(window, callbacks::key_check);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	sort_test(QuickSort, TestRes::QUICKSORT_COPIES, TestRes::QUICKSORT_COMPS);
	sort_test(BubbleSort, TestRes::BUBBLESORT_COPIES, TestRes::BUBBLESORT_COMPS);

	glfwSetCursor(window, cursor);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

	WindowNode* left_octangle = nullptr;
	WindowNode* right_octangle = nullptr;

	left_octangle = MAIN_WINDOW->make_underwindow (new Octangle (AnConst::LEFT_COPY_OCTANGLE, {1.0, 1.0, 1.0}), nullptr);
	MAIN_WINDOW->make_underwindow (new Arrow (AnConst::LEFT_VERTICAL_ARROW, {0.0, 0.0, 0.0}), left_octangle);
	MAIN_WINDOW->make_underwindow (new Arrow (AnConst::LEFT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0}), left_octangle);

	right_octangle = MAIN_WINDOW->make_underwindow (new Octangle (AnConst::RIGHT_COMP_OCTANGLE, {1.0, 1.0, 1.0}), nullptr);
	MAIN_WINDOW->make_underwindow (new Arrow (AnConst::RIGHT_VERTICAL_ARROW, {0.0, 0.0, 0.0}), right_octangle);
	MAIN_WINDOW->make_underwindow (new Arrow (AnConst::RIGHT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0}), right_octangle);

	MAIN_WINDOW->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COPIES),
	left_octangle);

	MAIN_WINDOW->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COPIES),
	left_octangle);

	MAIN_WINDOW->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COMPS),
	right_octangle);

	MAIN_WINDOW->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COMPS),
	right_octangle);

		// draw_text_and_measures(colors::WHITE, colors::BLACK);

		// poll_button(window, QSORT_BUTTON, QuickSort, TestRes::QUICKSORT);
		// poll_button(window, BSORT_BUTTON, BubbleSort, TestRes::BUBBLESORT);

		// MANAGER.PollEvents();

	main_loop(window);

	glfwTerminate();

	return 0;
}
