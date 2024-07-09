#include "3D_ENGINE/wall.hpp"

#include "3D_ENGINE/player.hpp"

Wall::Wall(float _xBottom1, float _yBottom1, float _xBottom2, float _yBottom2, RgbColor _color) {
	xBottom1 = _xBottom1;
	yBottom1 = _yBottom1;
	xBottom2 = _xBottom2;
	yBottom2 = _yBottom2;
	color = _color;

	float angle = atan2f(yBottom2 - yBottom1, xBottom2 - xBottom1);
	angle = (angle * 180) / M_PI;
	if (angle < 0) angle += 360;
	int _shade = angle;
	if (_shade > 180) _shade = 180 - (_shade - 180);
	if (_shade > 90) _shade = 90 - (_shade - 90);
	shade = _shade;
}

float Wall::getDistanceToPlayer(float xBottom1, float yBottom1, float xBottom2, float yBottom2) {
	distanceToPlayer = sqrt(pow((xBottom1 + xBottom2) / 2, 2) + pow((yBottom1 + yBottom2) / 2, 2));
	return distanceToPlayer;
}
