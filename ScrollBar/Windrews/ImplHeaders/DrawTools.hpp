#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE

class AbstractWindow {
public:
	virtual void draw() = 0;
	virtual void callback(const WindowStat& status) = 0;
	virtual void move(const Point& moveVec) = 0;
};

struct Color {
	double red;
	double green;
	double blue;

	Color() = default;
	Color(double red, double green, double blue): red(red), green(green), blue(blue) {}
	Color(const Color& other) = default;

	void set() const {
		glColor3d(red, green, blue);
	}

	void set_as_bg() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(red, green, blue, 0);
	}
};

template<typename MeasureType, int MeasureNum, int MeasureFreq, MeasureType MaxResult>
class Graphic: public AbstractWindow {
	MeasureType* MEASURES;
	Point topLeft;
	Point bottomRight;
	Color color;

public:
	Graphic () = delete;
	Graphic (Point topLeft, Point bottomRight, Color color, MeasureType* measures):
	topLeft (topLeft),
	bottomRight (bottomRight),
	color (color),
	MEASURES (measures) {}

	virtual void callback (const WindowStat& status) override {}

	virtual void move (const Point& moveVec) override {
		topLeft += moveVec;
		bottomRight += moveVec;
	}

	virtual void draw () override {

		Point start_point(topLeft.x, bottomRight.y);

		double vertical_length   = topLeft.y - bottomRight.y;
		double horizontal_length = bottomRight.x - topLeft.x;
		int max_size = MeasureNum * MeasureFreq;

		Point current;

		color.set();

		glBegin(GL_LINE_STRIP);

		start_point.set();

		for (int i = 1; i <= MeasureNum; ++i) {
			current.x = start_point.x + horizontal_length * double(i) * double(MeasureFreq) / max_size;
			current.y = start_point.y + vertical_length * double(MEASURES[i]) / MaxResult; 
			if (current.y > topLeft.y) {
				glEnd();
				glBegin(GL_LINE_STRIP);
				continue;
			}
			current.set();
		}

		glEnd();
		glFlush();
	}
};

template <int N>
class Polygon: public AbstractWindow {
public:
	Point coordinates[N];
	Color color;

public:

	Polygon () = delete;

	Polygon (const Point points[N], Color color): color (color) {
		for (int i = 0; i < N; ++i)
			coordinates[i] = points[i];
	}

	virtual void move (const Point& moveVec) override {
		for (int i = 0; i < N; ++i)
			coordinates[i] += moveVec;
	}

	virtual void callback (const WindowStat& status) override {}

	virtual void draw () override {
		color.set();

		glBegin (GL_POLYGON);
		for (int i = 0; i < 4; ++i)
			coordinates[i].set();
		glEnd();
		glFlush();	
	}
};

using Octangle = Polygon<4>;


class Arrow: public AbstractWindow {
	static const int POINT_NUM = 4;
	Point coordinates[POINT_NUM];
	Color color;

public:

	Arrow (const Point points[4], Color color): color (color) {
		for (int i = 0; i < 4; ++i)
			coordinates[i] = points[i];
	}

	virtual void move (const Point& moveVec) override {
		for (int i = 0; i < POINT_NUM; ++i)
			coordinates[i] += moveVec;
	}

	virtual void callback (const WindowStat& status) override {}

	virtual void draw () override {
		color.set();

		glBegin(GL_LINES);
		for (int i = 0; i < 2; ++i)
			coordinates[i].set();
		glEnd();

		glBegin(GL_TRIANGLES);
		for (int i = 1; i < 4; ++i)
			coordinates[i].set();
		
		glEnd();
		glFlush();
	}
};

namespace colors{
	const Color BLACK(0.0, 0.0, 0.0);
	const Color WHITE(1.0, 1.0, 1.0);
	const Color QUICKSORT(1.0, 0.0, 0.0);
	const Color QUICKSORT_PICKED(0.75, 0.0, 0.0);
	const Color QUICKSORT_ACTIVE(0.4, 0.0, 0.0);
	const Color BUBBLESORT(0.0, 1.0, 0.0);
	const Color BUBBLESORT_PICKED(0.0, 0.75, 0.0);
	const Color BUBBLESORT_ACTIVE(0.0, 0.4, 0.0);
	const Color BACKGROUND(0.04, 0.255, 0.29);
}

#endif