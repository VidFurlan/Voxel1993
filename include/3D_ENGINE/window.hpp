#pragma once

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/renderer.hpp"
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

	double lastTime = glfwGetTime();
	double lastFpsTime = glfwGetTime();
	int nbFrames = 0;

	std::vector<std::vector<float>> zBuffer;
	std::vector<std::vector<RgbColor>> frameBuffer;

	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

	void initGlfwSettings();
	void movePlayer();
	void clearBackground();
	void fpsCounter();
	void updateTime();

   public:
	double deltaTime;
	GLFWwindow *window;

	Window();

	void updateDisplay();
	void drawPixel(int x, int y, RgbColor rgbColor);
	void drawPixel(int x, int y, RgbColor rgbColor, float depth);
	void drawBuffer();
	void clearBuffer();
};

extern Window mainWindow;
