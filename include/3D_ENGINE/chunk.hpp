#pragma once

#include "block.hpp"
#include "renderer.hpp"

class Chunk {
   public:
	Chunk(int x, int y, int z);
	~Chunk();

	void render(Renderer *renderer);
	void update();

	static const int CHUNK_SIZE = 4;
	int x, y, z;

   private:
	Block ***blocks;
	Renderer *renderer;

	void updateVisibleFaces(int x, int y, int z);
    bool isFaceVisible(int x, int y, int z);
};
