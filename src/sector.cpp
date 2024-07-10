#include "3D_ENGINE/sector.hpp"

#include <iostream>

#include "texture_list.hpp"

Sector::Sector(float zBottom, float zTop, std::vector<Wall> walls, RgbColor topColor,
			   RgbColor bottomColor) {
	this->zBottom = zBottom;
	this->zTop = zTop;
	this->walls = walls;
	this->topColor = topColor;
	this->bottomColor = bottomColor;
	this->surfaceTexture = &textures[0];
}
