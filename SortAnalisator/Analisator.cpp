#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "../Windrews/Windrews.hpp"
#include "Sort.hpp"
#include "IntCount.h"
#include "AnalisatorConstants.h"
#include "MeasureTools.hpp"

const Point tr[] = {
	{1.0, 1.0},
	{0.0, 0.0},
	{1.0, 0.5}	
};

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

	WindowNode* left_octangle = nullptr;
	WindowNode* right_octangle = nullptr;

	left_octangle = window->make_underwindow<Picture> (nullptr, AnConst::LEFT_COPY_OCTANGLE, "textures/texture.jpg");
	window->make_underwindow<Arrow> (left_octangle, AnConst::LEFT_VERTICAL_ARROW, Color(0.0, 0.0, 0.0));
	window->make_underwindow<Arrow> (left_octangle, AnConst::LEFT_HORIZONTAL_ARROW, Color(0.0, 0.0, 0.0));

	right_octangle = window->make_underwindow<Octangle> (nullptr, AnConst::RIGHT_COMP_OCTANGLE, Color(1.0, 1.0, 1.0));
	window->make_underwindow<Arrow> (right_octangle, AnConst::RIGHT_VERTICAL_ARROW, Color(0.0, 0.0, 0.0));
	window->make_underwindow<Arrow> (right_octangle, AnConst::RIGHT_HORIZONTAL_ARROW, Color(0.0, 0.0, 0.0));

	// window->make_underwindow(new Polygon<3>(tr, {0.5, 0.4, 0.7}), nullptr);

	// window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	// (AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COPIES),
	// left_octangle);

	// window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	// (AnConst::LEFT_VERTICAL_ARROW[1], AnConst::LEFT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COPIES),
	// left_octangle);

	// window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	// (AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::BUBBLESORT, TestRes::BUBBLESORT_COMPS),
	// right_octangle);

	// window->make_underwindow (new Graphic<int, AnConst::MEASURE_NUM, AnConst::MEASURE_FREQ, AnConst::MAX_COPY_QUANTITY> 
	// (AnConst::RIGHT_VERTICAL_ARROW[1], AnConst::RIGHT_HORIZONTAL_ARROW[1], colors::QUICKSORT, TestRes::QUICKSORT_COMPS),
	// right_octangle);

	window->make_underwindow<PrimitiveButton> (
		nullptr,
			colors::QUICKSORT, 
		    colors::QUICKSORT_PICKED, 
		    colors::QUICKSORT_ACTIVE, 
		    AnConst::QUICKSORT_BUTTON_COORDS,
			test_qsort
		);

	window->make_underwindow<PrimitiveButton> (
		nullptr,
			colors::BUBBLESORT, 
		    colors::BUBBLESORT_PICKED, 
			colors::BUBBLESORT_ACTIVE, 
			AnConst::BUBBLESORT_BUTTON_COORDS,
			test_bsort
		);

	window->make_underwindow<KeyButton> (
		nullptr,
			GLFW_KEY_Q, 
			test_qsort
		);

	window->make_underwindow<KeyButton> (
		nullptr,
			GLFW_KEY_B, 
			test_bsort
		);

	window->make_underwindow<KeyButton>(
		nullptr,
			GLFW_KEY_DOWN, [window](){window->move({0.0, 0.2});});
    window->make_underwindow<KeyButton>(
		nullptr,
			GLFW_KEY_UP, [window](){window->move({0.0, -0.2});});


	main_loop(window);

	windrewsTerminate();

	return 0;
}
