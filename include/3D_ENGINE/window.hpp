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
    void clipBehindPlayer(int *x1, int *y1, int *z1, int *x2, int *y2, int *z2);
    void clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
    void drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2);

public:
    GLFWwindow *window;

    Window();
    
    void updateDisplay();
    void drawPixel(int x, int y, RgbColor color);
};
