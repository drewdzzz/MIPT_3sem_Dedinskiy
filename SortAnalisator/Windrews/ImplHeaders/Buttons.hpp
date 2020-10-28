#pragma once

class KeyButton: public AbstractWindow {
protected:
	int key;
	void (*func)(void) = nullptr;
public:
	void draw() override {}
	void callback(const WindowStat& status) override {
		if (status.key.key == key && status.key.action == GLFW_PRESS)
			func();
	}

	KeyButton(int key, void (*func)(void)):
	key (key),
	func (func) {}
};

class Button: public AbstractWindow {
protected:
	Color picked_color;
	Color active_color;
	Color color;
	const char* text;
	void (Button::*draw_method)() = &Button::draw_inactive;
public:
	Octangle shape;

	Button(const Color color, const Color picked_color, const Color active_color, const char* text, const Point points[4]): 
	shape (points, color),
	picked_color(picked_color),
	active_color(active_color),
	text(text)
	{}

	void draw_inactive() {
		shape.draw();
		draw_text_impl (text, strlen(text), shape.coordinates[0].x, shape.coordinates[0].y, GLUT_BITMAP_HELVETICA_18, colors::BLACK);
	}

	void draw_picked() {
		shape.draw();
		draw_text_impl(text, strlen(text), shape.coordinates[0].x, shape.coordinates[0].y, GLUT_BITMAP_HELVETICA_18, colors::BLACK);
	}

	void draw_active() {
		shape.draw();
		draw_text_impl(text, strlen(text), shape.coordinates[0].x, shape.coordinates[0].y, GLUT_BITMAP_HELVETICA_18, colors::BLACK);
	}

	virtual void draw() override {
		(this->*draw_method)();
	}

	virtual void callback(const WindowStat& status) override {

	}

};