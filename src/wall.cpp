#include "3D_ENGINE/wall.hpp"

#include "3D_ENGINE/player.hpp"

Wall::Wall(float xBottom1, float yBottom1, float xBottom2, float yBottom2, RgbColor color) {
	this->xBottom1 = xBottom1;
	this->yBottom1 = yBottom1;
	this->xBottom2 = xBottom2;
	this->yBottom2 = yBottom2;
	this->color = color;

	float angle = atan2f(yBottom2 - yBottom1, xBottom2 - xBottom1);
	angle = (angle * 180) / M_PI;
	if (angle < 0) angle += 360;
	int newShade = angle;
	if (newShade > 180) newShade = 180 - (newShade - 180);
	if (newShade > 90) newShade = 90 - (newShade - 90);
	shade = newShade;
}
