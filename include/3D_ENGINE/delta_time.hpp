#pragma once

#include <GLFW/glfw3.h>

class DeltaTime {
private:
    double time;
    double lastTime;
    double delta;

public:
    DeltaTime();
    void update();
    double getDeltaTime(); 
};

extern DeltaTime deltaTime;
