#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "Windrews/Windrews.hpp"
#include "Sort.hpp"
#include "IntCount.h"
#include "AnalisatorConstants.h"
#include "MeasureTools.hpp"

void main_loop(Windrew* window) {
	while (!window->shouldClose()) {
		window->draw();
		window->pollEvents();
		glfwPollEvents();
	}
}

void test_qsort() {
	sort_test(QuickSort, TestRes::QUICKSORT_COPIES, TestRes::QUICKSORT_COMPS);
}

void test_bsort() {
	sort_test(BubbleSort, TestRes::BUBBLESORT_COPIES, TestRes::BUBBLESORT_COMPS);
}

int main(int argc, char** argv) {
	srand(time(NULL));

	Windrew* window = windrewsInit(argc, argv, colors::BACKGROUND, AnConst::SCREEN_HEIGHT, AnConst::SCREEN_WIDTH);
	if (window == nullptr)
		return -1;

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	glfwSetCursor(window->main_window, cursor);
	glfwSetInputMode(window->main_window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

	WindowNode* left_octangle = nullptr;
	WindowNode* right_octangle = nullptr;

	left_octangle = window->make_underwindow (new Octangle (AnConst::LEFT_COPY_OCTANGLE, {1.0, 1.0, 1.0}), nullptr);
	window->make_underwindow (new Arrow (AnConst::LEFT_VERTICAL_ARROW, {0.0, 0.0, 0.0}), left_octangle);
	window->make_underwindow (new Arrow (AnConst::LEFT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0}), left_octangle);

	right_octangle = window->make_underwindow (new Octangle (AnConst::RIGHT_COMP_OCTANGLE, {1.0, 1.0, 1.0}), nullptr);
	window->make_underwindow (new Arrow (AnConst::RIGHT_VERTICAL_ARROW, {0.0, 0.0, 0.0}), right_octangle);
	window->make_underwindow (new Arrow (AnConst::RIGHT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0}), right_octangle);

	window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COPIES),
	left_octangle);

	window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COPIES),
	left_octangle);

	window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COMPS),
	right_octangle);

	window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	(AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COMPS),
	right_octangle);

	// window->make_underwindow (
		// new Button(colors::QUICKSORT, colors::QUICKSORT_PICKED, colors::QUICKSORT_ACTIVE, AnConst::QUICKSORT_BUTTON_TEXT, AnConst::QUICKSORT_BUTTON_COORDS),
		// nullptr);

	// window->make_underwindow (
		// new Button(colors::BUBBLESORT, colors::BUBBLESORT_PICKED, colors::BUBBLESORT_ACTIVE, AnConst::BUBBLESORT_BUTTON_TEXT, AnConst::BUBBLESORT_BUTTON_COORDS),
		// nullptr);

	window->make_underwindow (
		 new KeyButton(GLFW_KEY_Q, test_qsort),
		nullptr);

	window->make_underwindow (
		 new KeyButton(GLFW_KEY_B, test_bsort),
		nullptr);

		// draw_text_and_measures(colors::WHITE, colors::BLACK);

		// poll_button(window, QSORT_BUTTON, QuickSort, TestRes::QUICKSORT);
		// poll_button(window, BSORT_BUTTON, BubbleSort, TestRes::BUBBLESORT);

		// MANAGER.PollEvents();

	main_loop(window);

	glfwTerminate();

	return 0;
}
