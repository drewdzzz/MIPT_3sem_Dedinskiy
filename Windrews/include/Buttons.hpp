#pragma once

class HorizontalScrollBar: public AbstractWindow {
	Octangle background;
	Octangle shape;
	double scale;
	double startPos;
	Color color;
	Color hover_color;
	Color active_color;
	Point prevMousePos;
	WindowNode* scrollableWindow;

	bool wasPressed = false;

	bool mouseInside(const WindowStat& status, const viewPortState& state){
		Point mousePos = status.getMousePos(state);
		Point bottomRight;
		Point topLeft;
		shape.getPoints(topLeft, bottomRight);
		if (mousePos.x <= bottomRight.x && mousePos.x >= topLeft.x &&
		    mousePos.y <= topLeft.y && mousePos.y >= bottomRight.y)
			return true;
		else
			return false;
	}

	void (HorizontalScrollBar::*draw_method)(const viewPortState& state, const Shaders& shaders) = &HorizontalScrollBar::draw_inactive;

	void draw_inactive(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(color);
		shape.draw(state, shaders);
	}

	void draw_hover(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(hover_color);
		shape.draw(state, shaders);
	}

	void draw_active(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(active_color);
		shape.draw(state, shaders);
	}

	void move_bar(const Point& moveVec, const viewPortState& state) {
		
		Point background_topLeft;
		Point background_bottomRight;
		Point shape_topLeft;
		Point shape_bottomRight;
		background.getPoints(background_topLeft, background_bottomRight);
		shape.getPoints(shape_topLeft, shape_bottomRight);
		
		double top_offset = background_topLeft.y - shape_topLeft.y; 
		double bottom_offset = background_bottomRight.y - shape_bottomRight.y;
		Point newMove;
		newMove.x = moveVec.x;
		if (moveVec.y > 0 && moveVec.y > top_offset)
			newMove.y = top_offset;
		else if (moveVec.y < 0 && moveVec.y < bottom_offset)
			newMove.y = bottom_offset;
		else
			newMove.y = moveVec.y;
		shape.move(newMove, state);
		scrollableWindow->moveUnderWindows(newMove * (-1.0));
	}

public:
	HorizontalScrollBar() = delete;
	HorizontalScrollBar(const Point& topLeft, const Point& bottomRight, double scale, double startPos, WindowNode* scrollableWindow, Color backColor, Color color, Color hover_color, Color active_color):
	color(color),
	hover_color(hover_color),
	active_color(active_color),
	background(topLeft, bottomRight, backColor),
	scale(scale),
	startPos(startPos),
	shape({0,0}, {0,0}, color),
	prevMousePos(0, 0),
	scrollableWindow(scrollableWindow)
	{
		Point barTopLeft;
		Point barBottomRight;
		barTopLeft.x = topLeft.x;
		barBottomRight.x = bottomRight.x;

		barTopLeft.y = bottomRight.y + (topLeft.y - bottomRight.y) * (scale + 1)/2;		
		barBottomRight.y = topLeft.y - (topLeft.y - bottomRight.y) * (scale + 1)/2;		

		barTopLeft.y += (topLeft.y - barTopLeft.y) * startPos;
		barBottomRight.y += (barBottomRight.y - bottomRight.y) * startPos;

		shape.setPoints(barTopLeft, barBottomRight);
	}

	virtual void draw(const viewPortState& state, const Shaders& shaders) override {
		background.draw(state, shaders);
		(this->*draw_method)(state, shaders);
	}

	virtual void callback(const WindowStat& status, const viewPortState& state) override {
		int key = status.getKeyAction().key;
		int act = status.getKeyAction().action;
		if (mouseInside(status, state)) {
			if (key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
				prevMousePos = status.getMousePos(state);
				draw_method = &HorizontalScrollBar::draw_active;
				wasPressed = true;
			} else if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
				wasPressed = false;
				draw_method = &HorizontalScrollBar::draw_hover;
				// action();
			} else if (!wasPressed) {
				draw_method = &HorizontalScrollBar::draw_hover;
			}
		} else {
			if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
				draw_method = &HorizontalScrollBar::draw_inactive;
				wasPressed = false;
			} else if (!wasPressed) {
				draw_method = &HorizontalScrollBar::draw_inactive;
			}
		}

		if (wasPressed) {
			Point currMousePos = status.getMousePos(state);
			Point moveVec(0, currMousePos.y - prevMousePos.y);
			prevMousePos = currMousePos;
			move_bar(moveVec, state);
		}

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
	virtual void draw(const viewPortState& state, const Shaders& shaders) override {}
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

template<class Base>
class Button: public AbstractWindow {
protected:
	bool wasPressed = false;
	std::function<void()> action;

	bool mouseInside(const WindowStat& status, const viewPortState& state){
		Point mousePos = status.getMousePos(state);
		Point bottomRight;
		Point topLeft;
		shape.getPoints(topLeft, bottomRight);
		if (mousePos.x <= bottomRight.x && mousePos.x >= topLeft.x &&
		    mousePos.y <= topLeft.y && mousePos.y >= bottomRight.y)
			return true;
		else
			return false;
	}

	virtual void inactive_state() = 0;
	virtual void active_state() = 0;
	virtual void hover_state() = 0;

public:
	Base shape;

	Button(Base base_shape, std::function<void()> action): 
	shape (base_shape),
	action(action)
	{}


	virtual void move(const Point& moveVec, const viewPortState& state) override {
		shape.move(moveVec, state);
	}

	virtual void draw(const viewPortState& state,  const Shaders& shaders) override {
		shape.draw(state, shaders);
	}

	virtual void callback(const WindowStat& status, const viewPortState& state) override {
		int key = status.getKeyAction().key;
		int act = status.getKeyAction().action;
		if (mouseInside(status, state)) {
			if (key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_PRESS) {
				active_state();
				wasPressed = true;
			} else if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
				wasPressed = false;
				inactive_state();
				action();
			} else if (!wasPressed) {
				hover_state();
			}
		} else {
			if (wasPressed && key == GLFW_MOUSE_BUTTON_LEFT && act == GLFW_RELEASE) {
				inactive_state();
				wasPressed = false;
			} else if (!wasPressed) {
				inactive_state();
			}
		}

	}

	virtual bool changeViewPort() {
		return false;
	}
};

class PrimitiveButton: public Button<Octangle> {
protected:
	Color hover_color;
	Color active_color;
	Color inactive_color;


	void inactive_state() override {
		shape.setColor(inactive_color);
	}
	void active_state() override {
		shape.setColor(active_color);
	}
	void hover_state() override {
		shape.setColor(hover_color);
	}

public:

	PrimitiveButton(const Color inactive_color, const Color hover_color, const Color active_color, const Point points[2], std::function<void()> action): 
	Button(Octangle(points, {0,0,0}), action),
	inactive_color(inactive_color),
	hover_color(hover_color),
	active_color(active_color)
	{}

	virtual bool changeViewPort() {
		return false;
	}
};

class TextureButton: public Button<Picture> {
protected:

	void inactive_state() override {
	}
	void active_state() override {
	}
	void hover_state() override {
	}

public:

	TextureButton(const char* img_path, const Point points[2], std::function<void()> action): 
	Button(Picture(points, img_path), action)
	{}

	virtual bool changeViewPort() {
		return false;
	}
};