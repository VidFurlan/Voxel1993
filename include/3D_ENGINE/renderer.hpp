#pragma once

#include <GLFW/glfw3.h>
#include <cmath>

class Renderer {
private:
    void clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2);
    void drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2);

public:
    void draw3D();
};

extern Renderer renderer;
