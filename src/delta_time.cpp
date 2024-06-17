#include "3D_ENGINE/delta_time.hpp"

DeltaTime deltaTime;

DeltaTime::DeltaTime() : time(0), lastTime(0), delta(0) {
    update();
}

void DeltaTime::update() {
    time = glfwGetTime();
    delta = time - lastTime;
    lastTime = time;
}

double DeltaTime::getDeltaTime() { 
    return delta;
}
