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
private:
    int tick = 0;
    Keys keys;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

    void pixel(int x, int y, RgbColor c);
    void initGlfwSettings();
    void movePlayer();
    void clearBackground();
    void draw3D();

public:
    GLFWwindow *window;

    Window();
    
    void updateDisplay();
    void drawPixel(int x, int y, RgbColor color);
};
