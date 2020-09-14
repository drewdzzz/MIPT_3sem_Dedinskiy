#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE

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

struct Color {
	double red;
	double green;
	double blue;

	Color() = default;
	Color(double red, double green, double blue): red(red), green(green), blue(blue) {}
	Color (const Color& other) = default;

	void set() const {
		glColor3d(red, green, blue);
	}

	void set_as_bg() const {
		glClearColor(red, green, blue, 0);
	}
};

void draw_graphic(Results measurements[AnConst::MEASURE_NUM], Color color){

	if (measurements[0].comps == 0)
		return;

	Point copy_start_point(AnConst::LEFT_VERTICAL_ARROW[0], AnConst::LEFT_VERTICAL_ARROW[1]);
	Point comp_start_point(AnConst::RIGHT_VERTICAL_ARROW[0], AnConst::RIGHT_VERTICAL_ARROW[1]);

	double vertical_length   = AnConst::LEFT_VERTICAL_ARROW[3] - AnConst::LEFT_VERTICAL_ARROW[1];
	double horizontal_length = AnConst::LEFT_HORIZONTAL_ARROW[2] - AnConst::LEFT_HORIZONTAL_ARROW[0];
	int max_size = AnConst::MEASURE_FREQ * AnConst::MEASURE_NUM;

	Point current;

	color.set();

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

void draw_octangle(const double points[8], Color color) {
	color.set();

	glBegin(GL_QUADS);
	for (int i = 0; i < 8; i += 2)
		glVertex2d(points[i], points[i + 1]);
	glEnd();
	glFlush();
}

void draw_arrow(const double points[8], Color color) {
	color.set();

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

namespace colors{
	const Color BLACK(0.0, 0.0, 0.0);
	const Color WHITE(1.0, 1.0, 1.0);
	const Color QUICKSORT(1.0, 0.0, 0.0);
	const Color BUBBLESORT(0.0, 1.0, 0.0);
	const Color BACKGROUND(0.04, 0.255, 0.29);
}

#endif