#include "3D_ENGINE/window.hpp"

Window mainWindow;

Window::Window() {
    initGlfwSettings();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(GLSCR_WIDTH, GLSCR_HEIGHT, "3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    // Set the user pointer to this instance
    glfwSetWindowUserPointer(window, this);

    glPointSize(pixelScale);
    glOrtho(0, GLSCR_WIDTH, 0, GLSCR_HEIGHT, -1, 1); 
}

void Window::initGlfwSettings() {
    glfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

/**
 * @brief Draw a pixel on window
 *
 * @param x Position x
 * @param y Position y
 * @param rgbColor Color of the pixel
 */
void Window::pixel(int x, int y, RgbColor rgbColor) {
    glColor3ub(rgbColor.r, rgbColor.g, rgbColor.b); 
    glBegin(GL_POINTS);
    glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
    glEnd();
}

/**
 * @brief Move player based on input
 */
void Window::movePlayer() {
    // move up, down, left, right
    if (keys.a == 1 && keys.m == 0) {
        player.angle -= player.speed;
        if (player.angle < 0)
            player.angle += 360;
    }
    if (keys.d == 1 && keys.m == 0) {
        player.angle += player.speed;
        if (player.angle > 359)
            player.angle -= 360;
    }

    float dx = sin(player.angle * M_PI / 180) * player.speed;
    float dy = cos(player.angle * M_PI / 180) * player.speed;

    if (keys.w == 1 && keys.m == 0) {
        player.x += dx;
        player.y += dy;
    }
    if (keys.s == 1 && keys.m == 0) {
        player.x -= dx;
        player.y -= dy;
    }

    // strafe left, right
    if (keys.sr == 1) {
        player.x -= dy;
        player.y += dx;
    }
    if (keys.sl == 1) {
        player.x += dy;
        player.y -= dx;
    }

    // move up, down, look up, look down
    if (keys.a == 1 && keys.m == 1)
        player.lookAngle -= 1;
    if (keys.d == 1 && keys.m == 1)
        player.lookAngle += 1;
    if (keys.w == 1 && keys.m == 1)
        player.z += player.speed;
    if (keys.s == 1 && keys.m == 1)
        player.z -= player.speed;

    // std::cout << "deltaTime: " <<() << std::endl;
    // std::cout << "keys: " << "w: " << keys.w << " s: " << keys.s << " a: " << keys.a << " d: " << keys.d << " m: " << keys.m << " sr: " << keys.sr << " sl: " << keys.sl << std::endl;
    // std::cout << "x: " << player.x << " y: " << player.y << " z: " << player.z << " angle: " << player.angle << " lookAngle: " << player.lookAngle << std::endl;
}

/**
 * @brief Clear the background of the window using the background color
 */
void Window::clearBackground() {
    for (int y = 0; y < SCR_HEIGHT; y++)
        for (int x = 0; x < SCR_WIDTH; x++)
            pixel(x, y, backgroundColor);
}

/**
 * @brief Update window display
 */
void Window::updateDisplay() {
    clearBackground();
    movePlayer();
    renderer.draw3D();
    glFlush();
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Get the user pointer and cast it to Window
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W)
            win->keys.w = 1;
        if (key == GLFW_KEY_S)
            win->keys.s = 1;
        if (key == GLFW_KEY_A)
            win->keys.a = 1;
        if (key == GLFW_KEY_D)
            win->keys.d = 1;
        if (key == GLFW_KEY_M)
            win->keys.m = 1;
        if (key == GLFW_KEY_COMMA)
            win->keys.sr = 1;
        if (key == GLFW_KEY_PERIOD)
            win->keys.sl = 1;
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W)
            win->keys.w = 0;
        if (key == GLFW_KEY_S)
            win->keys.s = 0;
        if (key == GLFW_KEY_A)
            win->keys.a = 0;
        if (key == GLFW_KEY_D)
            win->keys.d = 0;
        if (key == GLFW_KEY_M)
            win->keys.m = 0;
        if (key == GLFW_KEY_COMMA)
            win->keys.sr = 0;
        if (key == GLFW_KEY_PERIOD)
            win->keys.sl = 0;
    }
}
