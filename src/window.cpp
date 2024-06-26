#include "3D_ENGINE/window.hpp"
#include "renderer.hpp"
#include <climits>
#include <future>

Window mainWindow;

Window::Window() {
    initGlfwSettings();

    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(GLSCR_WIDTH, GLSCR_HEIGHT, "3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetKeyCallback(window, key_callback);

    // Set the user pointer to this instance
    glfwSetWindowUserPointer(window, this);

    glPointSize(pixelScale);
    glOrtho(0, GLSCR_WIDTH, 0, GLSCR_HEIGHT, -1, 1); 

    frameBuffer = std::vector<std::vector<RgbColor>>(SCR_WIDTH, std::vector<RgbColor>(SCR_HEIGHT, backgroundColor));
    zBuffer = std::vector<std::vector<float>>(SCR_WIDTH, std::vector<float>(SCR_HEIGHT, INT_MAX));
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
void Window::drawPixel(int x, int y, RgbColor rgbColor) {
    frameBuffer[x][y] = rgbColor;
}

void Window::drawPixel(int x, int y, RgbColor rgbColor, float depth) {
    if (depth > zBuffer[x][y])
        return;

    frameBuffer[x][y] = rgbColor;
    zBuffer[x][y] = depth;
}

/**
 * @brief Move player based on input
 */
void Window::movePlayer() {
    // move up, down, left, right
    if (keys.a == 1 && keys.m == 0) {
        player.angle -= player.speed * deltaTime;
        if (player.angle < 0)
            player.angle += 360;
    }
    if (keys.d == 1 && keys.m == 0) {
        player.angle += player.speed * deltaTime;
        if (player.angle > 359)
            player.angle -= 360;
    }

    float dx = sin(player.angle * M_PI / 180) * player.speed * deltaTime;
    float dy = cos(player.angle * M_PI / 180) * player.speed * deltaTime;

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
        player.lookAngle -= 1 * deltaTime;
    if (keys.d == 1 && keys.m == 1)
        player.lookAngle += 1 * deltaTime;
    if (keys.w == 1 && keys.m == 1)
        player.z += player.speed * deltaTime;
    if (keys.s == 1 && keys.m == 1)
        player.z -= player.speed * deltaTime;

    // std::cout << "deltaTime: " <<() << std::endl;
    // std::cout << "keys: " << "w: " << keys.w << " s: " << keys.s << " a: " << keys.a << " d: " << keys.d << " m: " << keys.m << " sr: " << keys.sr << " sl: " << keys.sl << std::endl;
    // std::cout << "x: " << player.x << " y: " << player.y << " z: " << player.z << " angle: " << player.angle << " lookAngle: " << player.lookAngle << std::endl;
}

/**
 * @brief Clear the background of the window using the background color
 */
void Window::clearBuffer() {
    for (int y = 0; y < SCR_HEIGHT; y++)
        for (int x = 0; x < SCR_WIDTH; x++) {
            frameBuffer[x][y] = backgroundColor;
            zBuffer[x][y] = INT_MAX;
        }
}

/**
 * @brief Draw the buffer to the window
 */
void Window::drawBuffer() {
    glBegin(GL_POINTS);
    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            glColor3ub(frameBuffer[x][y].r, frameBuffer[x][y].g, frameBuffer[x][y].b);
            glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
        }
    }
    glEnd();
}

void Window::fpsCounter() {
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        std::cout << 1000.0/double(nbFrames) << " ms/frame\n";
        std::cout << double(nbFrames) << " fps\n";

        nbFrames = 0;
        lastTime += 1.0;
    }
}

void Window::updateTime() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    deltaTime *= 100;
    lastTime = currentTime;
}

/**
 * @brief Update window display
 */
void Window::updateDisplay() {
    updateTime();
    clearBuffer();
    movePlayer();
    renderer.draw3D();
    drawBuffer();
    // renderer.drawFloor();
    // renderer.testTextures();
    glFlush();
    fpsCounter();
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
