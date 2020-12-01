#pragma once

class ScrollBar: public AbstractWindow {
	Octangle background;
	Octangle shape;
	double scale;
	double startPos;

	bool mouseInside(const WindowStat& status, const viewPortState& state){
		Point mousePos = status.getMousePos(state);
		if (mousePos.x <= shape.bottomRight.x && mousePos.x >= shape.topLeft.x &&
		    mousePos.y <= shape.topLeft.y && mousePos.y >= shape.bottomRight.y)
			return true;
		else
			return false;
	}

public:
	ScrollBar() = delete;
	ScrollBar(const Point& topLeft, const Point& bottomRight, double scale, double startPos, Color color, Color backColor):
	background(topLeft, bottomRight, backColor),
	scale(scale),
	startPos(startPos),
	shape({0,0}, {0,0}, color)
	{
		Point barTopLeft;
		Point barBottomRight;
		barTopLeft.x = topLeft.x;
		barBottomRight.x = bottomRight.x;

		barTopLeft.y = bottomRight.y + (topLeft.y - bottomRight.y) * (scale + 1)/2;		
		barBottomRight.y = topLeft.y - (topLeft.y - bottomRight.y) * (scale + 1)/2;		

		barTopLeft.y += (topLeft.y - barTopLeft.y) * startPos;
		barBottomRight.y += (barBottomRight.y - bottomRight.y) * startPos;

		shape.bottomRight = barBottomRight;
		shape.topLeft = barTopLeft;
	}

	virtual void draw(const viewPortState& state) override {
		background.draw(state);
		shape.draw(state);
	}

	virtual void callback(const WindowStat& status, const viewPortState& state) {
		Point mousePos = status.getMousePos(state);
		// std::cout << mousePos.x << ' ' << mousePos.y << '\n';
		if (!mouseInside(status, state))
			return;

	}
	virtual void move(const Point& moveVec, const viewPortState& state) {}
	virtual bool changeViewPort() {
		return false;
	}

};


class KeyButton: public AbstractWindow {
protected:
	int key;
	std::function<void()> func;
public:
	virtual void move(const Point& moveVec, const viewPortState& state) override {}
	virtual void draw(const viewPortState& state) override {}
	virtual void callback(const WindowStat& status, const viewPortState& state) override {
		if (status.getKeyAction().key == key && status.getKeyAction().action == GLFW_PRESS)
			func();
	}

	KeyButton(int key, std::function<void()> func):
	key (key),
	func (func) {}

	virtual bool changeViewPort() {
		return false;
	}
};

class Button: public AbstractWindow {
protected:
	Color hover_color;
	Color active_color;
	Color color;
	const char* text;
	bool wasPressed = false;
	void (Button::*draw_method)(const viewPortState& state) = &Button::draw_inactive;
	std::function<void()> action;

	bool mouseInside(const WindowStat& status, const viewPortState& state){
		Point mousePos = status.getMousePos(state);
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

	void draw_inactive(const viewPortState& state) {
		shape.color = color;
		shape.draw(state);
	}

	void draw_hover(const viewPortState& state) {
		shape.color = hover_color;
		shape.draw(state);
	}

	void draw_active(const viewPortState& state) {
		shape.color = active_color;
		shape.draw(state);
	}

	virtual void move(const Point& moveVec, const viewPortState& state) override {
		shape.move(moveVec, state);
	}

	virtual void draw(const viewPortState& state) override {
		(this->*draw_method)(state);
	}

	virtual void callback(const WindowStat& status, const viewPortState& state) override {
		int key = status.getKeyAction().key;
		int act = status.getKeyAction().action;
		if (mouseInside(status, state)) {
			if (key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
				draw_method = &Button::draw_active;
				wasPressed = true;
			} else if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
				wasPressed = false;
				draw_method = &Button::draw_inactive;
				action();
			} else if (!wasPressed) {
				draw_method = &Button::draw_hover;
			}
		} else {
			if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && GLFW_RELEASE) {
				draw_method = &Button::draw_inactive;
				wasPressed = false;
			} else {
				draw_method = &Button::draw_inactive;
			}
		}

	}

	virtual bool changeViewPort() {
		return false;
	}
};