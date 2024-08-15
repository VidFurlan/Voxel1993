#pragma once

class Texture {
   public:
	Texture(int _width, int _height, const unsigned char *_textureMap)
		: width(_width), height(_height), textureMap(_textureMap) {}

	const int width, height;
	const unsigned char *textureMap;
};
