#pragma once

#include "3D_ENGINE/texture.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/wall.hpp"
#include "texture_list.hpp"

#include <vector>

class Sector {
public:
    float zBottom, zTop;
    float xPosition, yPosition;
    float distanceToPlayer;

    RgbColor topColor, bottomColor;
    Texture *surfaceTexture;
    int surfaceScale = 1;

    std::vector<Wall> walls;

    std::vector<float> surfacePoints = std::vector<float>(SCR_WIDTH);
    int surfaceOrientation;

    Sector(float zBottom, float zTop, float xPosition, float yPosition, std::vector<Wall> walls, RgbColor topColor, RgbColor bottomColor);
};
