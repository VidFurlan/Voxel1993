#pragma once

#include "chunk.hpp"
class World {
   public:
	static const int LOADED_WORLD_SIZE = 8;

	World();
	~World();

    bool isBlockSolid(int xChunk, int yChunk, int zChunk, int x, int y, int z);
	Chunk *getChunk(int x, int y, int z);

   private:
    Chunk ****chunks;
};

extern World world;
