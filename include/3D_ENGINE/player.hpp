#pragma once

#include <cmath>

class Player {
   public:
	Player();
	Player(int x, int y, int z);

	float x, y, z;
	float angle;
	float lookAngle;
	float speed = 1;
	float fov = 200;
};

extern Player player;
