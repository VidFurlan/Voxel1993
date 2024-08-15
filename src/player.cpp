#include <3D_ENGINE/player.hpp>

Player player(0, -100, 20);

Player::Player() : x(0), y(0), z(0), angle(0), lookAngle(0) {}

Player::Player(int x, int y, int z) : x(x), y(y), z(z), angle(0), lookAngle(0) {}
