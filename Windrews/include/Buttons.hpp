#pragma once

class ScrollBar: public AbstractWindow {
protected:
	Color hover_color;
	Color active_color;
	Color color;
	void (ScrollBar::*draw_method)() = &ScrollBar::draw_inactive;

	bool mouseInside(const WindowStat& status){
		Point mousePos = status.mousePos;
		if (mousePos.x <= shape.bottomRight.x && mousePos.x >= shape.topLeft.x &&
		    mousePos.y <= shape.topLeft.y && mousePos.y >= shape.bottomRight.y)
			return true;
		else
			return false;
	}
	
public:
	Octangle shape;

	ScrollBar(const Color color, const Color hover_color, const Color active_color, const Point points[4]): 
	shape (points, color),
	hover_color(hover_color),
	active_color(active_color)
	{}

	void draw_inactive() {
		shape.color = color;
		shape.draw();
	}

	void draw_hover() {
		shape.color = hover_color;
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
	Color hover_color;
	Color active_color;
	Color color;
	const char* text;
	bool isClicked = false;
	void (Button::*draw_method)() = &Button::draw_inactive;
	std::function<void()> action;

	bool mouseInside(const WindowStat& status){
		Point mousePos = status.mousePos;
		if (mousePos.x <= shape.bottomRight.x && mousePos.x >= shape.topLeft.x &&
		    mousePos.y <= shape.topLeft.y && mousePos.y >= shape.bottomRight.y)
			return true;
		else
			return false;
	}

public:
	Octangle shape;

	Button(const Color color, const Color hover_color, const Color active_color, const char* text, const Point points[4], std::function<void()> action): 
	shape (points, color),
	color (color),
	hover_color(hover_color),
	active_color(active_color),
	text(text),
	action(action)
	{}

	void draw_inactive() {
		shape.color = color;
		shape.draw();
	}

	void draw_hover() {
		shape.color = hover_color;
		shape.draw();
	}

	void draw_active() {
		shape.color = active_color;
		shape.draw();
	}

	virtual void move(const Point& moveVec) override {
		shape.move(moveVec);
	}

	virtual void draw() override {
		(this->*draw_method)();
	}

	virtual void callback(const WindowStat& status) override {
		if (!mouseInside(status)) {
			draw_method = &Button::draw_inactive;
			return;
		}

		int key = status.key.key;
		int action = status.key.action;

		if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			isClicked = true;
			draw_method = &Button::draw_active;
		}

		if (mouseInside(status) && isClicked && (key == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE)) {
			isClicked = false;
			this->action();
		}

		if (mouseInside(status) && (key == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_RELEASE)) {
			draw_method = &Button::draw_hover;
		}
	}

};