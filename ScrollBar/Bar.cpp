#include <iostream>
#include "../Windrews/Windrews.hpp"

const Color WHITE = {1.0, 1.0, 1.0};
const Point WHITE_OCTANGLE[] = {
    {-1.0, 1.0},
    {1.0, -0.75},
};

const Color BLUE = {0.0, 0.0, 1.0};
const Point BLUE_OCTANGLE[] = {
    {-1.0, -0.75},
    {1.0, -2.0},
};

const Color RED = {1.0, 0.0, 0.0};
const Point RED_OCTANGLE[] = {
    {-1.0, -2.0},
    {1.0, -4.0},
};

int main(int argc, char** argv) {

    Windrew* window = windrewsInit(argc, argv, {1.0, 1.0, 1.0}, 720, 1280);
    if (window == nullptr)
		return -1;

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);   

    WindowNode* view = window->make_underwindow(new UnderScreen({200, 300}, 600, 400, {0, 1, 0}), nullptr);

    window->make_underwindow(new Octangle(BLUE_OCTANGLE, BLUE), view);
    window->make_underwindow(new Octangle(WHITE_OCTANGLE, WHITE), view);
    window->make_underwindow(new Octangle(RED_OCTANGLE, RED), view);
    window->make_underwindow(new ScrollBar({0.8, 1.0}, {1.0, -1.0}, 0.5, 0.0, {0,0,0}, {1,1,0}), view);

    window->make_underwindow(new KeyButton(GLFW_KEY_DOWN, [view](){view->moveUnderWindows({0.0, 0.2});}), nullptr);
    window->make_underwindow(new KeyButton(GLFW_KEY_UP, [view](){view->moveUnderWindows({0.0, -0.2});}), nullptr);

    while (!window->shouldClose()) {
        window->pollEvents();
        window->draw();
    }

    windrewsTerminate();
}