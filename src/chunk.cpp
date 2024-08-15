#include "chunk.hpp"

Chunk::Chunk() {
	blocks = new Block **[CHUNK_SIZE];
	
    for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i] = new Block *[CHUNK_SIZE];

		for (int j = 0; j < CHUNK_SIZE; j++) {
			blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                sectors.push_back(
                        Sector(k * 10 + 0, k * 10 + 10,
                            {
                            Wall(i * 10 + 0, j * 10 + 0, i * 10 + 10, j * 10 + 0, white),
                            Wall(i * 10 + 10, j * 10 + 0, i * 10 + 10, j * 10 + 10, white),
                            Wall(i * 10 + 10, j * 10 + 10, i * 10 + 0, j * 10 + 10, white),
                            Wall(i * 10 + 0, j * 10 + 10, i * 10 + 0, j * 10 + 0, white),
                            },
                            white, white));
            }
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
