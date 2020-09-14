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
#include"Callbacks.hpp"

GLFWwindow* initCreateContextWindow(int argc, char** argv) {
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

	glutInit(&argc, argv);

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(1280, 720, "Graphics", NULL, NULL);

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

		glfwWaitEvents();

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

		draw_text_and_measures(colors::WHITE, colors::BLACK);

		glfwSwapBuffers(window);

	}
}

int main(int argc, char** argv) {
	srand(time(NULL));

	GLFWwindow* window = initCreateContextWindow(argc, argv);
	if (!window)
		return -1;

	glfwSetKeyCallback(window, callbacks::key_check);

	main_loop(window);

	glfwTerminate();

	return 0;
}