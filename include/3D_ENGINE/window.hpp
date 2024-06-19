#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/renderer.hpp"
#include "3D_ENGINE/player.hpp"

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

    void initGlfwSettings();
    void movePlayer();
    void clearBackground();

public:
    GLFWwindow *window;

    Window();
    
    void updateDisplay();
    void pixel(int x, int y, RgbColor c);
};

extern Window mainWindow;
