#pragma once

struct Button {
private:
	Color color;
	Color picked_color;
	Color active_color;
	const char* text;
public:
	Point coordinates[4];

	Button(const Color color, const Color picked_color, const Color active_color, const char* text, const Point points[4]): color(color), picked_color(picked_color), active_color(active_color), text(text){
		for (int i = 0; i < 4; ++i)
			coordinates[i] = points[i];
	}

	void draw() const {
		draw_octangle(coordinates, color);
		draw_text_impl(text, strlen(text), coordinates[0].x, coordinates[0].y, GLUT_BITMAP_HELVETICA_18, colors::BLACK);
	}

	void draw_picked() const {
		draw_octangle(coordinates, picked_color);
	}

	void draw_active() const {
		draw_octangle(coordinates, active_color);
	}
};