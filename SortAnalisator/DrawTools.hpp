#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE


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


#endif