#include<cstdlib>
#include<iostream>
#include<string>
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

Button QSORT_BUTTON({1.0, 0.0, 0.0}, {1.0, 0.451, 0.451}, {0.65, 0.0, 0.0}, AnConst::QUICKSORT_BUTTON_TEXT, AnConst::QUICKSORT_BUTTON_COORDS);

Button BSORT_BUTTON({0.0, 1.0, 0.0}, {0.451, 1.0, 0.451}, {0.0, 0.65, 0.0}, AnConst::BUBBLESORT_BUTTON_TEXT, AnConst::BUBBLESORT_BUTTON_COORDS);

void GetCursorPos(GLFWwindow* window, double* x, double* y) {
	glfwGetCursorPos(window, x, y);
	*x = 2 * *x / AnConst::SCREEN_WIDTH - 1.0;
	*y = -2 * *y / AnConst::SCREEN_HEIGHT + 1.0;

}

void poll_button(GLFWwindow* window, const Button& button, void (*callback)(IntCount*, IntCount*, MyIntLess), Results* result_array) {
	double x = 0.0;
	double y = 0.0;
	GetCursorPos(window, &x, &y);

	if (x <= button.coordinates[1].x && x >= button.coordinates[0].x && y <= button.coordinates[1].y && y >= button.coordinates[2].y) {
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			button.draw_active();
			sort_test(callback, result_array);
		}
		else
			button.draw_picked();
	}
	else
		button.draw();
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

	return window;
}

void main_loop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		colors::BACKGROUND.set_as_bg();

		draw_octangle(AnConst::LEFT_COPY_OCTANGLE, {1.0, 1.0, 1.0});
		draw_arrow(AnConst::LEFT_VERTICAL_ARROW, {0.0, 0.0, 0.0});
		draw_arrow(AnConst::LEFT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0});

		draw_octangle(AnConst::RIGHT_COMP_OCTANGLE, {1.0, 1.0, 1.0});
		draw_arrow(AnConst::RIGHT_VERTICAL_ARROW, {0.0, 0.0, 0.0});
		draw_arrow(AnConst::RIGHT_HORIZONTAL_ARROW, {0.0, 0.0, 0.0});

		draw_graphic(TestRes::QUICKSORT, colors::QUICKSORT);
		draw_graphic(TestRes::BUBBLESORT, colors::BUBBLESORT);

		// draw_text_and_measures(colors::WHITE, colors::BLACK);

		poll_button(window, QSORT_BUTTON, QuickSort, TestRes::QUICKSORT);
		poll_button(window, BSORT_BUTTON, BubbleSort, TestRes::BUBBLESORT);

		glfwPollEvents();

		glfwSwapBuffers(window);

	}
}

int main(int argc, char** argv) {
	srand(time(NULL));

	GLFWwindow* window = initCreateContextWindow(argc, argv);
	if (!window)
		return -1;

	glfwSetKeyCallback(window, callbacks::key_check);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);

	glfwSetCursor(window, cursor);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

	main_loop(window);

	glfwTerminate();

	return 0;
}
