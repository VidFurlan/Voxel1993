#pragma once

#include "block.hpp"
#include "renderer.hpp"

class Chunk {
   public:
	Chunk(int x, int y, int z);
	~Chunk();

    void render(Renderer *renderer);
	void update();
    void updateBlock(int x, int y, int z);

	static const int CHUNK_SIZE = 4;
    int x, y, z;

   private:
	Block ***blocks;
    Renderer *renderer;

    bool isFaceVisible(int x, int y, int z);
};
