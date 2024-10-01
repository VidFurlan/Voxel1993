#pragma once

#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>

#include "3D_ENGINE/window_defines.hpp"
#include "block.hpp"
#include "texture.hpp"

class Chunk;
class Renderer {
   public:
    void renderChunk(Chunk *chunk);
	void renderBlock(Block *block, int x, int y, int z);

   private:
	float distanceToPlayer = 0;

	float chunkX, chunkY, chunkZ;
	float blockX, blockY, blockZBottom, blockZTop;

	std::vector<float> surfacePoints = std::vector<float>(SCR_WIDTH);
	int surfaceOrientation = 0;

	float wallX[4][4], wallY[4][4], wallZ[4][4];
	float xBottom1[4], yBottom1[4], xBottom2[4], yBottom2[4];

	Texture *texture;
	int surfaceScale = 1;
	int uvU = 1, uvV = 1;
	int shade;

	static void clipBehindPlayer(float &x1, float &y1, float &z1, float &x2, float &y2, float &z2);
	void renderBlockSurfaces(Block *block, int xPos1, int xPos2, int bottomPos1, int bottomPos2,
							 int topPos1, int topPos2, int orientation, int w);

	// Test functions
	void testTextures();
	void testFloor();
};
