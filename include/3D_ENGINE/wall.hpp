#pragma once

#include "3D_ENGINE/window_defines.hpp"

#include <cmath>

class Wall {
public:
    float xBottom1, yBottom1;
    float xBottom2, yBottom2;
    RgbColor color;

    Wall(float xBottom1, float yBottom1, float xBottom2, float yBottom2, RgbColor color);
    float getDistanceToPlayer(float xBottom1, float yBottom1, float xBottom2, float yBottom2);
};
