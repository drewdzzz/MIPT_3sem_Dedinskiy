#include<cstdlib>
#include<iostream>
#include<GLFW/glfw3.h>
#include<GL/glut.h>
// #include<GL/gl.h>
// #include<GL/glew.h>
// #include<SOIL/SOIL.h>
#include"Sort.hpp"
#include"IntCount.h"
#include"AnalisatorConstants.h"
#include"MeasureTools.hpp"
#include"DrawTools.hpp"


void key_check(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		sort_test(QuickSort, TestRes::QUICKSORT);

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
		sort_test(BubbleSort, TestRes::BUBBLESORT);	

}



GLFWwindow* initCreateContextWindow(int argc, char* argv[]) {
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

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.75, 1.0, 0);

		draw_octangle(AnConst::LEFT_COPY_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::LEFT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::LEFT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		draw_octangle(AnConst::RIGHT_COMP_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::RIGHT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::RIGHT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		draw_graphic(TestRes::QUICKSORT, 1.0, 0.0, 0.0);
		draw_graphic(TestRes::BUBBLESORT, 0.0, 1.0, 0.0);

		glfwSwapBuffers(window);

	}
}

int main(int args, char* argv[])
{
	srand(time(NULL));

	GLFWwindow* window = initCreateContextWindow(args, argv);
	if (!window)
		return -1;

	glfwSetKeyCallback(window, key_check);


	main_loop(window);


	glfwTerminate();
	
	return 0;
}