#include "world.hpp"

#include <cstdlib>

#include "chunk.hpp"

World world;

World::World() {
	chunks = new Chunk ***[LOADED_WORLD_SIZE];

	for (int i = 0; i < LOADED_WORLD_SIZE; i++) {
		chunks[i] = new Chunk **[LOADED_WORLD_SIZE];
		for (int j = 0; j < LOADED_WORLD_SIZE; j++) {
			chunks[i][j] = new Chunk *[LOADED_WORLD_SIZE];
			for (int k = 0; k < LOADED_WORLD_SIZE; k++) {
                chunks[i][j][k] = (rand() % 2 == 0) ? new Chunk(i, j, k) : nullptr;
			}
		}
	}

	for (int i = 0; i < LOADED_WORLD_SIZE; i++) {
		for (int j = 0; j < LOADED_WORLD_SIZE; j++) {
			for (int k = 0; k < LOADED_WORLD_SIZE; k++) {
                if (chunks[i][j][k] != nullptr) chunks[i][j][k]->update();
			}
		}
	}
}

World::~World() {}

bool World::isBlockSolid(int xChunk, int yChunk, int zChunk, int x, int y, int z) {
	Chunk *chunk = getChunk(xChunk, yChunk, zChunk);
	if (chunk == nullptr) return false;
	return chunk->getBlock(x, y, z)->isActive();
}

Chunk *World::getChunk(int x, int y, int z) {
	if (x < 0 || x >= LOADED_WORLD_SIZE || y < 0 || y >= LOADED_WORLD_SIZE || z < 0 ||
		z >= LOADED_WORLD_SIZE) {
		return nullptr;
	}

	return chunks[x][y][z];
}
