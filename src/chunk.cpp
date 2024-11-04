#include "chunk.hpp"

#include <iostream>

#include "block.hpp"
#include "renderer_manager.hpp"
#include "world.hpp"

Chunk::Chunk(int x, int y, int z)
	: x(x * CHUNK_SIZE * Block::BLOCK_SIZE),
	  y(y * CHUNK_SIZE * Block::BLOCK_SIZE),
	  z(z * CHUNK_SIZE * Block::BLOCK_SIZE) {
	blocks = new Block **[CHUNK_SIZE];

	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i] = new Block *[CHUNK_SIZE];

		for (int j = 0; j < CHUNK_SIZE; j++) {
			blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] blocks[i][j];
		}

		delete[] blocks[i];
	}

	delete[] blocks;
}

void Chunk::render(Renderer *renderer) {
	if (renderer == nullptr) {
		std::cout << "\x1b[31m" << "Renderer is null" << std::endl;
	}

	update();

	renderer->renderChunk(this);

	rendererManager.freeRenderer(renderer);
}

void Chunk::update() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				updateVisibleFaces(x, y, z);
			}
		}
	}
}

void Chunk::updateVisibleFaces(int x, int y, int z) {
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) return;
	if (!blocks[x][y][z].isActive()) return;

	Block *block = &blocks[x][y][z];
	block->visibleFaces[BlockFace::XPos] = isFaceVisible(x + 1, y, z);
	block->visibleFaces[BlockFace::XNeg] = isFaceVisible(x - 1, y, z);
	block->visibleFaces[BlockFace::YPos] = isFaceVisible(x, y + 1, z);
	block->visibleFaces[BlockFace::YNeg] = isFaceVisible(x, y - 1, z);
	block->visibleFaces[BlockFace::ZPos] = isFaceVisible(x, y, z + 1);
	block->visibleFaces[BlockFace::ZNeg] = isFaceVisible(x, y, z - 1);
}

bool Chunk::isFaceVisible(int x, int y, int z) {
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        return !world.isBlockSolid(x, y, z);
	}
	return !blocks[x][y][z].isActive();
}
