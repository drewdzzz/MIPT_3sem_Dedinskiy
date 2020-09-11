#ifndef ANALISATOR_CONSTANTS_INCLUDE
#define ANALISATOR_CONSTANTS_INCLUDE

namespace AnConst{

	const char* QUICK_SORT_BUTTON_TEXTURE = "./textures/buttons/quicksort.jpg";

	const int BUTTON_WIDTH = 100;
	const int BUTTON_HEIGHT = 50;


	const int SORT_QUANTITY = 3;

	const int MEASURE_FREQ = 5;
	const int MEASURE_NUM  = 5;

	const double LEFT_OCTANGLE[8] = {
	-0.95, 0.95,
	-0.05, 0.95,
	-0.05, -0.25,
	-0.95, -0.25
	};

	const double LEFT_VERTICAL_ARROW[8] = {
		-0.9, -0.2,
		-0.9, 0.9,
		-0.91, 0.86,
		-0.89, 0.86,
	};

	const double LEFT_HORIZONTAL_ARROW[8] = {
		-0.9, -0.2,
		-0.1, -0.2,
		-0.124, -0.216,
		-0.124, -0.184
	};

	const double RIGHT_OCTANGLE[8] = {
		0.05, 0.95,
		0.95, 0.95,
		0.95, -0.25,
		0.05, -0.25
	};

	const double RIGHT_VERTICAL_ARROW[8] = {
		0.1, -0.2,
		0.1, 0.9,
		0.09, 0.86,
		0.11, 0.86
	};

	const double RIGHT_HORIZONTAL_ARROW[8] = {
		0.1, -0.2,
		0.9, -0.2,
		0.876, -0.216,
		0.876, -0.184
	};
}


#endif