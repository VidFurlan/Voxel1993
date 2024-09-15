#include "chunk.hpp"
#include "renderer_manager.hpp"

Chunk::Chunk(int x, int y, int z) : x(x * CHUNK_SIZE), y(y * CHUNK_SIZE), z(z * CHUNK_SIZE) {
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
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                renderer->renderBlock(&blocks[x][y][z], this->x + x, this->y + y, this->z + z);
            }
        }
    }

    rendererManager.freeRenderer(renderer);
}

void Chunk::update() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                updateBlock(x, y, z);
            }
        }
    }
}

void Chunk::updateBlock(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE || !blocks[x][y][z].isActive()) {
        return;
    }

    Block *block = &blocks[x][y][z];
    
    bool visibleFaces[6];
    visibleFaces[0] = isFaceVisible(x - 1, y, z);
    visibleFaces[1] = isFaceVisible(x + 1, y, z);
    visibleFaces[2] = isFaceVisible(x, y - 1, z);
    visibleFaces[3] = isFaceVisible(x, y + 1, z);
    visibleFaces[4] = isFaceVisible(x, y, z - 1);
    visibleFaces[5] = isFaceVisible(x, y, z + 1);
    
    
}

bool Chunk::isFaceVisible(int x, int y, int z) {
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        return true;
    }

    return !blocks[x][y][z].isActive();
}
