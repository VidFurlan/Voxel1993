#pragma once

#include <vector>
#include "block.hpp"
#include "sector.hpp"

class Chunk {
   public:
	Chunk(int x, int y, int z);
	~Chunk();

	void update();

	static const int CHUNK_SIZE = 4;
    int x, y, z;

   private:
	Block ***blocks;
};
