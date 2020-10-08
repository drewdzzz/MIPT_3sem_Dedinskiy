#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE

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

	Point copy_start_point(AnConst::LEFT_VERTICAL_ARROW[0].x, AnConst::LEFT_VERTICAL_ARROW[0].y);
	Point comp_start_point(AnConst::RIGHT_VERTICAL_ARROW[0].x, AnConst::RIGHT_VERTICAL_ARROW[0].y);

	double vertical_length   = AnConst::LEFT_VERTICAL_ARROW[1].y - AnConst::LEFT_VERTICAL_ARROW[0].y;
	double horizontal_length = AnConst::LEFT_HORIZONTAL_ARROW[1].x - AnConst::LEFT_HORIZONTAL_ARROW[0].x;
	int max_size = AnConst::MEASURE_FREQ * AnConst::MEASURE_NUM;

	Point current;

	color.set();

	glBegin(GL_LINE_STRIP);

	glVertex2d(copy_start_point.x, copy_start_point.y);

	for (int i = 1; i <= AnConst::MEASURE_NUM; ++i) {
		current.x = copy_start_point.x + horizontal_length * double(i) * double(AnConst::MEASURE_FREQ) / max_size;
		current.y = copy_start_point.y + vertical_length * double(measurements[i].copies) / AnConst::MAX_COPY_QUANTITY; 
		if (current.y <= copy_start_point.y + vertical_length)
			glVertex2d(current.x, current.y);
		else	
			break;
	}

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex2d(comp_start_point.x, comp_start_point.y);

	for (int i = 1; i <= AnConst::MEASURE_NUM; ++i) {
		current.x = comp_start_point.x + horizontal_length * double(i) * double(AnConst::MEASURE_FREQ) / max_size;
		current.y = comp_start_point.y + vertical_length * double(measurements[i].comps) / AnConst::MAX_COPY_QUANTITY; 
		if (current.y <= copy_start_point.y + vertical_length)
			glVertex2d(current.x, current.y);
		else
			break;
	}	

	glEnd();
	glFlush();
}

void draw_octangle(const Point points[4], Color color) {
	color.set();

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; ++i)
		points[i].set();
	glEnd();
	glFlush();
}

void draw_arrow(const Point points[4], Color color) {
	color.set();

	glBegin(GL_LINES);
	for (int i = 0; i < 2; ++i)
		points[i].set();
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i = 1; i < 4; ++i)
		points[i].set();
	
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