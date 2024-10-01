#pragma once

#include "block.hpp"
#include "renderer.hpp"

class Chunk {
   public:
	static const int CHUNK_SIZE = 4;
	int x, y, z;

	Chunk(int x, int y, int z);
	~Chunk();

    Block *getBlock(int x, int y, int z) { return &blocks[x][y][z]; }
	void updateVisibleFaces(int x, int y, int z);
	void render(Renderer *renderer);
	void update();

   private:
	Block ***blocks;
	Renderer *renderer;

    bool isFaceVisible(int x, int y, int z);
};
