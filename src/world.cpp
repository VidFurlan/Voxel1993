#include "world.hpp"
#include "chunk.hpp"

World world;

World::World() {
	chunks = new Chunk ***[LOADED_WORLD_SIZE];

	for (int i = 0; i < LOADED_WORLD_SIZE; i++) {
		chunks[i] = new Chunk **[LOADED_WORLD_SIZE];

		for (int j = 0; j < LOADED_WORLD_SIZE; j++) {
            chunks[i][j] = new Chunk *[LOADED_WORLD_SIZE];

            for (int k = 0; k < LOADED_WORLD_SIZE; k++) {
                chunks[i][j][k] = new Chunk(i, j, k);      
            }
        }
	}
}

World::~World() {}

bool World::isBlockSolid(int x, int y, int z) { return false; }

Chunk *World::getChunk(int x, int y, int z) { 
    return chunks[x][y][z];
}
