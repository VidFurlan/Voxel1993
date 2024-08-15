#pragma once

#include <vector>
#include "block.hpp"
#include "sector.hpp"

class Chunk {
   public:
	Chunk();
	~Chunk();

	void update();

	static const int CHUNK_SIZE = 16;

    std::vector<Sector> sectors;

   private:
	Block ***blocks;
};
