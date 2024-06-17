#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/delta_time.hpp"
#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/delta_time.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

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
        player.z -= player.speed;
    if (keys.s == 1 && keys.m == 1)
        player.z += player.speed;

    // std::cout << "deltaTime: " <<() << std::endl;
    // std::cout << "keys: " << "w: " << keys.w << " s: " << keys.s << " a: " << keys.a << " d: " << keys.d << " m: " << keys.m << " sr: " << keys.sr << " sl: " << keys.sl << std::endl;
    std::cout << "x: " << player.x << " y: " << player.y << " z: " << player.z << " angle: " << player.angle << " lookAngle: " << player.lookAngle << std::endl;
}

/**
 * @brief Clear the background of the window using the background color
 */
void Window::clearBackground() {
    for (int y = 0; y < SCR_HEIGHT; y++)
        for (int x = 0; x < SCR_WIDTH; x++)
            pixel(x, y, backgroundColor);
}

void Window::clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2) {
    float point1 = *y1;
    float point2 = *y2;
    float dist = point2 - point1;
    float s = point1 / (point1 - point2);
    *x1 = *x1 + s * (*x2 - (*x1));
    *y1 = *y1 + s * (*y2 - (*y1));
    *z1 = *z1 + s * (*z2 - (*z1));

    if (*y1 == 0) *y1 = 1;
}

void Window::drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2) {
    int distBottom  = bottomPos2 - bottomPos1;
    int distTop     = topPos2 - topPos1;
    int distX       = (xPos2 - xPos1 == 0) ? 1 : xPos2 - xPos1;

    if (xPos1 < 1) xPos1 = 1;
    if (xPos1 > SCR_WIDTH - 1) xPos1 = SCR_WIDTH - 1;
    if (xPos2 < 1) xPos2 = 1;
    if (xPos2 > SCR_WIDTH - 1) xPos2 = SCR_WIDTH - 1;

    for (int x = xPos1; x < xPos2; x++) {
        int y1 = bottomPos1 + distBottom * (x - xPos1) / distX;
        int y2 = topPos1 + distTop * (x - xPos1) / distX;

        // Y clipping
        if (y1 < 1) y1 = 1;
        if (y1 > SCR_HEIGHT - 1) y1 = SCR_HEIGHT - 1;
        if (y2 < 1) y2 = 1;
        if (y2 > SCR_HEIGHT - 1) y2 = SCR_HEIGHT - 1;
        
        for (int y = y1; y < y2; y++) {
            pixel(x, y, white);
        }
    }
}

void Window::draw3D() {
    float wx[4], wy[4], wz[4];
    float cs = cos(player.angle * M_PI / 180);
    float sn = sin(player.angle * M_PI / 180);

    float x1 = 40 - player.x, y1 = 10 - player.y;
    float x2 = 40 - player.x, y2 = 290 - player.y;

    // Transform world coordinates to view coordinates
    wx[0] = x1 * cs - y1 * sn;
    wy[0] = y1 * cs + x1 * sn;
    
    wx[1] = x2 * cs - y2 * sn;
    wy[1] = y2 * cs + x2 * sn;

    wx[2] = wx[0];
    wy[2] = wy[0];

    wx[3] = wx[1];
    wy[3] = wy[1];

    // Z coordinate is simple translation in this 2.5D example
    wz[0] = 0 - player.z + ((player.lookAngle * wy[0]) / 32.0);
    wz[1] = 0 - player.z + ((player.lookAngle * wy[0]) / 32.0);
    wz[2] = wz[0] + 50;
    wz[3] = wz[1] + 50;

    wx[0] = wx[0] * 200 / wy[0] + SCR_WIDTH_HALF; wy[0] = wz[0] * 200 / wy[0] + SCR_HEIGHT_HALF;
    wx[1] = wx[1] * 200 / wy[1] + SCR_WIDTH_HALF; wy[1] = wz[1] * 200 / wy[1] + SCR_HEIGHT_HALF;
    wx[2] = wx[2] * 200 / wy[2] + SCR_WIDTH_HALF; wy[2] = wz[2] * 200 / wy[2] + SCR_HEIGHT_HALF;
    wx[3] = wx[3] * 200 / wy[3] + SCR_WIDTH_HALF; wy[3] = wz[3] * 200 / wy[3] + SCR_HEIGHT_HALF;

    if (wy[0] < 1 && wy[1] < 1) return;
    if (wy[0] < 1) {
        clipBehindPlayer(&wx[0], &wy[0], &wz[0], &wx[1], &wy[1], &wz[1]);
        clipBehindPlayer(&wx[2], &wy[2], &wz[2], &wx[3], &wy[3], &wz[3]);
    }
    if (wy[1] < 1) {
        clipBehindPlayer(&wx[1], &wy[1], &wz[1], &wx[0], &wy[0], &wz[0]);
        clipBehindPlayer(&wx[3], &wy[3], &wz[3], &wx[2], &wy[2], &wz[2]);
    }

    drawWall(static_cast<int>(wx[0]), static_cast<int>(wx[1]), static_cast<int>(wy[0]), static_cast<int>(wy[1]), static_cast<int>(wy[2]), static_cast<int>(wy[3]));

    if (wx[0] >= 0 && wx[0] < SCR_WIDTH && wy[0] >= 0 && wy[0] < SCR_HEIGHT) {
        pixel(static_cast<int>(wx[0]), static_cast<int>(wy[0]), yellow);
    }
    if (wx[1] >= 0 && wx[1] < SCR_WIDTH && wy[1] >= 0 && wy[1] < SCR_HEIGHT) {
        pixel(static_cast<int>(wx[1]), static_cast<int>(wy[1]), red);
    }
    if (wx[2] >= 0 && wx[2] < SCR_WIDTH && wy[2] >= 0 && wy[2] < SCR_HEIGHT) {
        pixel(static_cast<int>(wx[2]), static_cast<int>(wy[2]), green);
    }
    if (wx[3] >= 0 && wx[3] < SCR_WIDTH && wy[3] >= 0 && wy[3] < SCR_HEIGHT) {
        pixel(static_cast<int>(wx[3]), static_cast<int>(wy[3]), blue);
    }
}

/**
 * @brief Update window display
 */
void Window::updateDisplay() {
    clearBackground();
    movePlayer();
    draw3D();
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
