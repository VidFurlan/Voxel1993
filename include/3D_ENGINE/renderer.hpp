#pragma once

#include "3D_ENGINE/sector.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>

class Renderer {
private:
    std::vector<Sector> sectors = {
        // Red sector
        Sector(0, 100, 0, 0, {
            Wall(0, 0, 100, 0, RgbColor{225, 0, 0}),
            Wall(100, 0, 100, 100, RgbColor{200, 0, 0}),
            Wall(100, 100, 0, 100, RgbColor{225, 0, 0}),
            Wall(0, 100, 0, 0, RgbColor{200, 0, 0})}, 
            RgbColor{255, 0, 0}, RgbColor{150, 0, 0}
        ),

        // Green sector
        Sector(0, 100, 0, 0, {
            Wall(200, 0, 300, 0, RgbColor{0, 225, 0}),
            Wall(300, 0, 300, 100, RgbColor{0, 200, 0}),
            Wall(300, 100, 200, 100, RgbColor{0, 225, 0}),
            Wall(200, 100, 200, 0, RgbColor{0, 200, 0})},
            RgbColor{0, 255, 0}, RgbColor{0, 150, 0}
            ),
    };

    void clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
    void drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2, RgbColor color, Sector &sector, int orientation);

public:
    void draw3D();
};

extern Renderer renderer;
