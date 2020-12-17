#include "../Windrews.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////HORIZONTAL SCROLL BAR IMPLEMENTATION/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool HorizontalScrollBar::mouseInside(const WindowStat& status, const viewPortState& state){
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

	void HorizontalScrollBar::draw_inactive(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(color);
		shape.draw(state, shaders);
	}

	void HorizontalScrollBar::draw_hover(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(hover_color);
		shape.draw(state, shaders);
	}

	void HorizontalScrollBar::draw_active(const viewPortState& state, const Shaders& shaders) {
		shape.setColor(active_color);
		shape.draw(state, shaders);
	}

	void HorizontalScrollBar::move_bar(const Point& moveVec, const viewPortState& state) {
		
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

	HorizontalScrollBar::HorizontalScrollBar(const Point& topLeft, const Point& bottomRight, double scale, double startPos, WindowNode* scrollableWindow, Color backColor, Color color, Color hover_color, Color active_color):
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

	void HorizontalScrollBar::draw(const viewPortState& state, const Shaders& shaders){
		background.draw(state, shaders);
		(this->*draw_method)(state, shaders);
	}

	void HorizontalScrollBar::callback(const WindowStat& status, const viewPortState& state){
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

	void HorizontalScrollBar::move(const Point& moveVec, const viewPortState& state) {}

	bool HorizontalScrollBar::changeViewPort() {
		return false;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////KEY BUTTON IMPLEMENTATION/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void KeyButton::move(const Point& moveVec, const viewPortState& state) {}
	void KeyButton::draw(const viewPortState& state, const Shaders& shaders) {}
	void KeyButton::callback(const WindowStat& status, const viewPortState& state) {
		if (status.getKeyAction().key == key && status.getKeyAction().action == GLFW_PRESS)
			func();
	}

	KeyButton::KeyButton(int key, std::function<void()> func):
	key (key),
	func (func) {}

	bool KeyButton::changeViewPort() {
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TEXTURE BUTTON IMPLEMENTATION//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void TextureButton::inactive_state() {}
	void TextureButton::active_state() {}
	void TextureButton::hover_state() {}

	TextureButton::TextureButton(const char* img_path, const Point points[2], std::function<void()> action): 
	Button(Picture(points, img_path), action)
	{}

	bool TextureButton::changeViewPort() {
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////PRIMITIVE BUTTON IMPLEMENTATION////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void PrimitiveButton::inactive_state() {
		shape.setColor(inactive_color);
	}
	void PrimitiveButton::active_state() {
		shape.setColor(active_color);
	}
	void PrimitiveButton::hover_state() {
		shape.setColor(hover_color);
	}

	PrimitiveButton::PrimitiveButton(const Color inactive_color, const Color hover_color, const Color active_color, const Point points[2], std::function<void()> action): 
	Button(Octangle(points, {0,0,0}), action),
	inactive_color(inactive_color),
	hover_color(hover_color),
	active_color(active_color)
	{}

	bool PrimitiveButton::changeViewPort() {
		return false;
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////