#pragma once

#include <iostream>
#include <cmath>

#include <GLFW/glfw3.h>

#include "3D_ENGINE/window_defines.hpp"

/**
 * @class Window
 * @brief Class containing window components
 *
 */
class Window {
public:
    GLFWwindow *window;

    Window();
    
    void update();
    void drawPixel(int x, int y, rgbColor color);
    void display();

private:
    int tick = 0;
    keys K;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    void pixel(int x, int y, int c);
    void initGlfwSettings();
    void movePlayer();
    void clearBackground();
    void draw3D();
};
