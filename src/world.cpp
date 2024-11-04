#include "world.hpp"
#include "chunk.hpp"
#include <iostream>

World world;

World::World() {
	chunks = new Chunk ***[LOADED_WORLD_SIZE];

	for (int i = 0; i < LOADED_WORLD_SIZE; i++) {
		chunks[i] = new Chunk **[LOADED_WORLD_SIZE];

		for (int j = 0; j < LOADED_WORLD_SIZE; j++) {
            chunks[i][j] = new Chunk *[LOADED_WORLD_SIZE];

            for (int k = 0; k < LOADED_WORLD_SIZE; k++) {
                //if (k == 0)
                    chunks[i][j][k] = new Chunk(i, j, k);      
                //else 
                //    chunks[i][j][k] = nullptr;
            }
        }
	}
}

World::~World() {}

bool World::isBlockSolid(int x, int y, int z) {
    if (x < 0 || x >= LOADED_WORLD_SIZE * Chunk::CHUNK_SIZE ||
        y < 0 || y >= LOADED_WORLD_SIZE * Chunk::CHUNK_SIZE ||
        z < 0 || z >= LOADED_WORLD_SIZE * Chunk::CHUNK_SIZE) {
        return false;
    }

    // Calculate the chunk and block coordinates
    // They can be negative, so we need to add the size of the world
    int chunkX = (x - Chunk::CHUNK_SIZE + 1) / (Chunk::CHUNK_SIZE);
    int chunkY = (y - Chunk::CHUNK_SIZE + 1) / (Chunk::CHUNK_SIZE);
    int chunkZ = (z - Chunk::CHUNK_SIZE + 1) / (Chunk::CHUNK_SIZE);

    int blockX = x % (Chunk::CHUNK_SIZE);
    int blockY = y % (Chunk::CHUNK_SIZE);
    int blockZ = z % (Chunk::CHUNK_SIZE);

    std::cout << "Chunk: " << chunkX << " " << chunkY << " " << chunkZ << std::endl;
    std::cout << "Block: " << blockX << " " << blockY << " " << blockZ << std::endl;

    if (chunks[chunkX][chunkY][chunkZ] == nullptr) {
        return false;
    }
    return chunks[chunkX][chunkY][chunkZ]->getBlock(blockX, blockY, blockZ)->isActive();
}

Chunk *World::getChunk(int x, int y, int z) { 
    return chunks[x][y][z];
}
