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
			arr[i] = rand() % 100000;
		IntCount::NullCounters();
		
		sort_func(arr, arr + size);
		
		
		result_array[i] = IntCount::getCounters();
		delete[] arr;
	}

}

void draw_graphic(Results measurements[AnConst::MEASURE_NUM], double red, double green, double blue){

	if (measurements[0].comps == 0)
		return;

	Point copy_start_point(AnConst::LEFT_VERTICAL_ARROW[0], AnConst::LEFT_VERTICAL_ARROW[1]);
	Point comp_start_point(AnConst::RIGHT_VERTICAL_ARROW[0], AnConst::RIGHT_VERTICAL_ARROW[1]);

	double vertical_length   = AnConst::LEFT_VERTICAL_ARROW[3] - AnConst::LEFT_VERTICAL_ARROW[1];
	double horizontal_length = AnConst::LEFT_HORIZONTAL_ARROW[2] - AnConst::LEFT_HORIZONTAL_ARROW[0];
	int max_size = AnConst::MEASURE_FREQ * AnConst::MEASURE_NUM;

	Point current;

	glColor3d(red, green, blue);

	glBegin(GL_LINE_STRIP);

	glVertex2d(copy_start_point.x, copy_start_point.y);

	for (int i = 1; i <= AnConst::MEASURE_NUM; ++i) {
		current.x = copy_start_point.x + horizontal_length * double(i) * double(AnConst::MEASURE_FREQ) / max_size;
		current.y = copy_start_point.y + vertical_length * double(measurements[i].copies) / AnConst::MAX_COPY_QUANTITY; 
		glVertex2d(current.x, current.y);
	}

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex2d(comp_start_point.x, comp_start_point.y);

	for (int i = 1; i <= AnConst::MEASURE_NUM; ++i) {
		current.x = comp_start_point.x + horizontal_length * double(i) * double(AnConst::MEASURE_FREQ) / max_size;
		current.y = comp_start_point.y + vertical_length * double(measurements[i].comps) / AnConst::MAX_COPY_QUANTITY; 
		glVertex2d(current.x, current.y);
	}	

	glEnd();
	glFlush();
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
	srand(time(NULL));
	GLFWwindow* window = createContextWindow();
	if (!window)
		return -1;

	sort_test(BubbleSort, BUBBLESORT_RESULTS);
	sort_test(QuickSort, QUICKSORT_RESULTS);


	while (!glfwWindowShouldClose(window))
	{


		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.75, 1.0, 0);

		draw_octangle(AnConst::LEFT_COPY_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::LEFT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::LEFT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		draw_octangle(AnConst::RIGHT_COMP_OCTANGLE, 1.0, 1.0, 1.0);
		draw_arrow(AnConst::RIGHT_VERTICAL_ARROW, 0.0, 0.0, 0.0);
		draw_arrow(AnConst::RIGHT_HORIZONTAL_ARROW, 0.0, 0.0, 0.0);

		draw_graphic(QUICKSORT_RESULTS, 1.0, 0.0, 0.0);
		draw_graphic(BUBBLESORT_RESULTS, 0.0, 1.0, 0.0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	
	return 0;
}