#ifndef ANALISATOR_CONSTANTS_INCLUDE
#define ANALISATOR_CONSTANTS_INCLUDE


struct Point{
	double x;
	double y;

	Point() = default;
	Point(double x, double y): x(x), y(y) {
	}	

	void set() const {
		glVertex2d(x, y);
	}
};

namespace AnConst{

	const char QUICKSORT_BUTTON_TEXT[]  = "PRESS Q TO TEST QUICKSORT";
	const char BUBBLESORT_BUTTON_TEXT[] = "PRESS B TO TEST BUBBLESORT";
	const char F_BUTTON_TEXT[]          = "PRESS F TO PAY RESPECT";
	const char COPY_NUM[]               = "Copies";
	const char COMP_NUM[]               = "Comporations";
	const char EXIT_DECLARATION[]       = "PRESS ESC TO EXIT";

	const char ARRAY_SIZE[] = "Array size";

	const int MAX_COPY_QUANTITY = 65000;
	const int MAX_COMP_QUANTITY = 1000000000;

	const int BUTTON_WIDTH = 100;
	const int BUTTON_HEIGHT = 50;


	const int SORT_QUANTITY = 3;

	const int MEASURE_FREQ = 10;
	const int MEASURE_NUM  = 50;

	const Point LEFT_COPY_OCTANGLE[4] = {
		{-0.95, 0.95},
		{-0.05, 0.95},
		{-0.05, -0.25},
		{-0.95, -0.25}
	};

	const Point LEFT_VERTICAL_ARROW[4] = {
		{-0.9, -0.2},
		{-0.9, 0.9},
		{-0.91, 0.86},
		{-0.89, 0.86},
	};

	const Point LEFT_HORIZONTAL_ARROW[4] = {
		{-0.9, -0.2},
		{-0.1, -0.2},
		{-0.124, -0.216},
		{-0.124, -0.184}
	};

	const Point RIGHT_COMP_OCTANGLE[4] = {
		{0.05, 0.95},
		{0.95, 0.95},
		{0.95, -0.25},
		{0.05, -0.25}
	};

	const Point RIGHT_VERTICAL_ARROW[4] = {
		{0.1, -0.2},
		{0.1, 0.9},
		{0.09, 0.86},
		{0.11, 0.86}
	};

	const Point RIGHT_HORIZONTAL_ARROW[4] = {
		{0.1, -0.2},
		{0.9, -0.2},
		{0.876, -0.216},
		{0.876, -0.184}
	};
}


#endif