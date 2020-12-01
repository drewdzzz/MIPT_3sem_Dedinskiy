#ifndef DRAW_TOOLS_INCLUDE
#define DRAW_TOOLS_INCLUDE

class AbstractWindow {
public:
	viewPortState viewChange;
	virtual void draw(const viewPortState& state) = 0;
	virtual void callback(const WindowStat& status, const viewPortState& state) = 0;
	virtual void move(const Point& moveVec, const viewPortState& state) = 0;
	virtual bool changeViewPort() = 0;
	viewPortState getViewPortChange() {
		return viewChange;
	}

	AbstractWindow():
	viewChange(0, 0, 0, 0) {}
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

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		topLeft += moveVec;
		bottomRight += moveVec;
	}

	virtual void draw (const viewPortState& state) override {

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

	virtual bool changeViewPort() {
		return false;
	}
};

template <int N>
class Polygon: public AbstractWindow {
	Point coordinates[N];
public:
	Color color;

	Polygon () = delete;

	Polygon (const Point points[N], Color color): color (color) {
		for (int i = 0; i < N; ++i)
			coordinates[i] = points[i];
	}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		for (int i = 0; i < N; ++i)
			coordinates[i] += moveVec;
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw (const viewPortState& state) override {
		color.set();

		glBegin (GL_POLYGON);
		for (int i = 0; i < 4; ++i)
			coordinates[i].set();
		glEnd();
		glFlush();	
	}

	virtual bool changeViewPort() {
		return false;
	}
};

class Octangle: public AbstractWindow {
public:
	Point topLeft;
	Point bottomRight;
	Color color;
	Octangle (const Point points[2], Color color): color (color) {
		if (points[0].x >= points[1].x) {
			bottomRight = points[0];
			topLeft = points[1];
		}
		else {
			bottomRight = points[1];
			topLeft = points[0];
		}
	}

	Octangle (const Point& topLeft, const Point& bottomRight, Color color):
	topLeft (topLeft),
	bottomRight (bottomRight),
	color (color) {}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		topLeft += moveVec;
		bottomRight += moveVec;
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw (const viewPortState& state) override {
		color.set();

		Point topRight = {bottomRight.x, topLeft.y};
		Point bottomLeft = {topLeft.x, bottomRight.y};

		glBegin (GL_QUADS);
		topLeft.set();
		topRight.set();
		bottomRight.set();
		bottomLeft.set();
		glEnd();
		glFlush();	
	}

	virtual bool changeViewPort() {
		return false;
	}

};

class Arrow: public AbstractWindow {
	static const int POINT_NUM = 4;
	Point coordinates[POINT_NUM];
public:
	Color color;

	Arrow (const Point points[4], Color color): color (color) {
		for (int i = 0; i < 4; ++i)
			coordinates[i] = points[i];
	}

	virtual void move (const Point& moveVec, const viewPortState& state) override {
		for (int i = 0; i < POINT_NUM; ++i)
			coordinates[i] += moveVec;
	}

	virtual void callback (const WindowStat& status, const viewPortState& state) override {}

	virtual void draw(const viewPortState& state) override {
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

	virtual bool changeViewPort() {
		return false;
	}
};

class UnderScreen: public AbstractWindow {
	Point bottomLeft;
	Color color;
	Octangle background;
	int width;
	int height;

public:

	UnderScreen(const Point& bottomLeft, int width, int height, const Color& color):
	bottomLeft(bottomLeft),
	color(color),
	width(width),
	height(height),
	background(Point(-1, 1), Point(1, -1), color) {
		viewChange = viewPortState(bottomLeft, width, height); 	
	}
	
	virtual void callback(const WindowStat& status, const viewPortState& state) {};
	virtual void move(const Point& moveVec, const viewPortState& state) {
		bottomLeft += Point(moveVec.x * state.width, moveVec.y * state.height);
	}

	virtual void draw(const viewPortState& state) {
		glViewport(int(bottomLeft.x), int(bottomLeft.y), width, height);
		background.draw(state);
	}

	virtual bool changeViewPort() {
		return true;
	}
};
#endif