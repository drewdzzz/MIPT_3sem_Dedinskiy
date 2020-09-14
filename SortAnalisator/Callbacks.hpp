#ifndef CALLBACKS_INCLUDE
#define CALLBACKS_INCLUDE

namespace callbacks{
    void key_check(GLFWwindow* window, int key, int scancode, int action, int mode) {
        if ( ( key == GLFW_KEY_ESCAPE || key == GLFW_KEY_F ) && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
            sort_test(QuickSort, TestRes::QUICKSORT);

        if (key == GLFW_KEY_B && action == GLFW_PRESS)
            sort_test(BubbleSort, TestRes::BUBBLESORT);	

    }
}

#endif