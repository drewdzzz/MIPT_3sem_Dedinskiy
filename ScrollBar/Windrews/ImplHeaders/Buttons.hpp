#pragma once

class ScrollBar: public AbstractWindow {
protected:
	Color picked_color;
	Color active_color;
	Color color;
	void (ScrollBar::*draw_method)() = &ScrollBar::draw_inactive;
public:
	Octangle shape;

	ScrollBar(const Color color, const Color picked_color, const Color active_color, const Point points[4]): 
	shape (points, color),
	picked_color(picked_color),
	active_color(active_color)
	{}

	void draw_inactive() {
		shape.color = color;
		shape.draw();
	}

	void draw_picked() {
		shape.color = picked_color;
		shape.draw();
	}

	void draw_active() {
		shape.color = active_color;
		shape.draw();
	}

	virtual void move (const Point& moveVec) override {
		shape.move(moveVec);
	}

	virtual void draw() override {
		(this->*draw_method)();
	}

	virtual void callback(const WindowStat& status) override {

	}
};

class KeyButton: public AbstractWindow {
protected:
	int key;
	std::function<void()> func;
public:
	virtual void move(const Point& moveVec) override {}
	virtual void draw() override {}
	virtual void callback(const WindowStat& status) override {
		if (status.key.key == key && status.key.action == GLFW_PRESS)
			func();
	}

	KeyButton(int key, std::function<void()> func):
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
		shape.color = color;
		shape.draw();
	}

	void draw_picked() {
		shape.color = picked_color;
		shape.draw();
	}

	void draw_active() {
		shape.color = active_color;
		shape.draw();
	}

	virtual void move (const Point& moveVec) override {
		shape.move(moveVec);
	}

	virtual void draw() override {
		(this->*draw_method)();
	}

	virtual void callback(const WindowStat& status) override {

	}

};