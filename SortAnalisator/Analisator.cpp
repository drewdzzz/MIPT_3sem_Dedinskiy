#include<cstdlib>
#include<iostream>
#include<GLFW/glfw3.h>
#include<SOIL/SOIL.h>
#include"Sort.hpp"
#include"IntCount.h"
#include"AnalisatorConstants.h"

Results BUBBLESORT_RESULTS[AnConst::MEASURE_NUM];
Results  QUICKSORT_RESULTS[AnConst::MEASURE_NUM];


void sort_test(void (*sort_func)(IntCount*, IntCount*), Results* result_array) {
	for (int size = AnConst::MEASURE_FREQ, i = 0; i <= AnConst::MEASURE_NUM; size += AnConst::MEASURE_FREQ, ++i) {
		IntCount* arr = new IntCount[size];
		for (int i = 0; i < size; ++i)
			arr[i] = rand() % 10000;
		IntCount::NullCounters();
		
		sort_func(arr, arr + size);
		
		
		result_array[i] = IntCount::getCounters();
		delete[] arr;
	}

}

/*Results makeTest(size_t n, void (*ptrSort)(IntCopyCompCount*, IntCopyCompCount*, MyIntLess)) {
	IntCopyCompCount* arr = new IntCopyCompCount[n];
	for (int i = 0; i < n; ++i)
		arr[i].value = rand();
	IntCopyCompCount::NullCounters();
	ptrSort(arr, arr + n, MyIntLess());
	delete[] arr;
	return IntCopyCompCount::getCounters();
}*/


void draw_octangle(const double points[8], double red, double green, double blue) {
	glColor3d(red, green, blue);

	glBegin(GL_QUADS);
	for (int i = 0; i < 8; i += 2)
		glVertex2d(points[i], points[i + 1]);
	glEnd();
	glFlush();
}

void draw_arrow(const double points[8], double red, double green, double blue) {
	glColor3d(red, green, blue);

	glBegin(GL_LINES);
	for (int i = 0; i < 4; i += 2)
		glVertex2d(points[i], points[i + 1]);
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i = 2; i < 8; i += 2)
		glVertex2d(points[i], points[i + 1]);
	glEnd();

	glFlush();
}

GLFWwindow* createContextWindow() {
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

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

int main()
{
	GLFWwindow* window = createContextWindow();
	if (!window)
		return -1;


	while (!glfwWindowShouldClose(window))
	{


		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.75, 1.0, 0);

		draw_octangle(AnConst::LEFT_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::LEFT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::LEFT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		draw_octangle(AnConst::RIGHT_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::RIGHT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::RIGHT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	
	return 0;
}