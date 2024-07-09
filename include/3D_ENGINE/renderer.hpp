#pragma once

#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include "3D_ENGINE/sector.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "wall.hpp"

class Renderer {
   private:
	std::vector<Sector> sectors;
	void clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
	void drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2,
				  RgbColor color, Sector &sector, Wall &wall, int orientation);

   public:
	Renderer();

	void draw3D();
	void testTextures();
	void drawFloor();
};

extern Renderer renderer;
