#include <iostream>
#include "../Windrews/Windrews.hpp"


Point BRUSH_BUTTON_COORDS[2] = {
    {-1.0, 1.0},
    {-0.87, 0.8}
};

Point SAVE_BUTTON_COORDS[2] = {
    {-0.87, 1.0},
    {-0.71, 0.8}
};

Point CANVAS_COORD = Point(300, 50);

int main(int argc, char** argv) {

    Windrew* window = windrewsInit(argc, argv, {0.2, 0.2, 0.2}, 720, 1280);
    if (window == nullptr)
		return -1;  

    Canvas* canvas = new Canvas(CANVAS_COORD, 900, 600, {1, 1, 1}, {1, 0, 0});

    window->make_underwindow(new TextureButton("textures/brush_button.png", BRUSH_BUTTON_COORDS, 
    [canvas](){
        if (canvas->getDrawMode() != BRUSH) {
            canvas->setDrawMode(BRUSH);
        }
        else {
            canvas->setDrawMode(STOP);
        }
    }), nullptr);
    window->make_underwindow(new TextureButton("textures/save_button.png", SAVE_BUTTON_COORDS, [](){std::cout << "SAVE\n";}), nullptr);
    window->make_underwindow(canvas, nullptr);

    while (!window->shouldClose()) {
        window->pollEvents();
        window->draw();
    }

    windrewsTerminate();
}