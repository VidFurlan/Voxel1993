#pragma once

#include <cmath>

class Player {
   public:
	float x, y, z;
	float angle;
	float lookAngle;
	float speed = 1;
	float fov = 200;

	Player();
	Player(int _x, int _y, int _z);
};

extern Player player;
