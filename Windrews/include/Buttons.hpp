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

	bool mouseInside(const WindowStat& status, const viewPortState& state);

	void (HorizontalScrollBar::*draw_method)(const viewPortState& state, const Shaders& shaders) = &HorizontalScrollBar::draw_inactive;

	void draw_inactive(const viewPortState& state, const Shaders& shaders);

	void draw_hover(const viewPortState& state, const Shaders& shaders);

	void draw_active(const viewPortState& state, const Shaders& shaders);

	void move_bar(const Point& moveVec, const viewPortState& state);

public:
	HorizontalScrollBar() = delete;
	HorizontalScrollBar(const Point& topLeft, const Point& bottomRight, double scale, double startPos, WindowNode* scrollableWindow, Color backColor, Color color, Color hover_color, Color active_color);

	virtual void draw(const viewPortState& state, const Shaders& shaders) override;

	virtual void callback(const WindowStat& status, const viewPortState& state) override;

	virtual void move(const Point& moveVec, const viewPortState& state);

	virtual bool changeViewPort();
};

class KeyButton: public AbstractWindow {
protected:
	int key;
	std::function<void()> func;
public:
	virtual void move(const Point& moveVec, const viewPortState& state) override;
	virtual void draw(const viewPortState& state, const Shaders& shaders) override;
	virtual void callback(const WindowStat& status, const viewPortState& state) override;
	virtual bool changeViewPort();

	KeyButton(int key, std::function<void()> func);
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


	void inactive_state() override;
	void active_state() override;
	void hover_state() override;

public:

	PrimitiveButton(const Color inactive_color, const Color hover_color, const Color active_color, const Point points[2], std::function<void()> action);

	virtual bool changeViewPort();
};

class TextureButton: public Button<Picture> {
protected:
	void inactive_state() override;
	void active_state() override;
	void hover_state() override;
public:

	TextureButton(const char* img_path, const Point points[2], std::function<void()> action);

	virtual bool changeViewPort() override;
};