#include "3D_ENGINE/window.hpp"

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

