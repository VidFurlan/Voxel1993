#pragma once

class Texture {
public:
    const int width, height;
    const unsigned char *textureMap;
    
    Texture(int _width, int _height, const unsigned char *_textureMap) : width(_width), height(_height), textureMap(_textureMap) {}
};
