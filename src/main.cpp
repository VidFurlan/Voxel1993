#include <iostream>

#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/player.hpp"

int main() {
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(mainWindow.window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        mainWindow.updateDisplay();
        glfwSwapBuffers(mainWindow.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(mainWindow.window);
    glfwTerminate();
    return 0;
}

