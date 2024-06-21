#include "3D_ENGINE/sector.hpp"
#include "texture_list.hpp"

Sector::Sector(float _zBottom, float _zTop, float _xPosition, float _yPosition, std::vector<Wall> _walls, RgbColor _topColor, RgbColor _bottomColor) {
    zBottom     = _zBottom;
    zTop        = _zTop;
    xPosition   = _xPosition;
    yPosition   = _yPosition;
    walls       = _walls;
    topColor    = _topColor;
    bottomColor = _bottomColor;
    surfaceTexture = &textures[0];
}
