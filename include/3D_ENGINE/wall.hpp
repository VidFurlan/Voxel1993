#pragma once

#include <cmath>
#include <vector>

#include "3D_ENGINE/texture.hpp"
#include "3D_ENGINE/window_defines.hpp"

class Wall {
   public:
	Wall(float xBottom1, float yBottom1, float xBottom2, float yBottom2, RgbColor color);

	float wallX[4], wallY[4], wallZ[4];
	float xBottom1, yBottom1;
	float xBottom2, yBottom2;

	RgbColor color;
	Texture *texture;
	int uvU = 1, uvV = 1;
	int shade;
};
