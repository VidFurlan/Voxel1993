#pragma once

#include <GLFW/glfw3.h>

#include <climits>
#include <cmath>
#include <mutex>
#include <vector>

#include <window_defines.hpp>

/**
 * @class Window
 * @brief Class containing window components
 *
 */
class Window {
   public:
	Window();

	void updateDisplay();
	void drawPixel(int x, int y, RgbColor rgbColor);
	void drawPixel(int x, int y, RgbColor rgbColor, float depth);

	double deltaTime;
	GLFWwindow *window;

   private:
	int tick = 0;
	Keys keys;

	double lastTime = glfwGetTime();
	double lastFpsTime = glfwGetTime();
	int nbFrames = 0;

    std::vector<std::vector<float>> zBuffer;
	std::vector<std::vector<RgbColor>> frameBuffer;
    std::mutex frameBufferMutex;

	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

	void initGlfwSettings();
	void movePlayer();
	void clearBackground();
	void fpsCounter();
	void updateTime();
	void drawBuffer();
	void clearBuffer();
};

extern Window mainWindow;
