#include "3D_ENGINE/renderer.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <climits>
#include <cmath>
#include <functional>

#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/renderer.hpp"
#include "3D_ENGINE/texture_list.hpp"
#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "block.hpp"
#include "chunk.hpp"

/**
 * @brief Clip wall edges to the edge of the screen if they are behind the player
 */
void Renderer::clipBehindPlayer(float &x1, float &y1, float &z1, float &x2, float &y2, float &z2) {
	if (y1 < 1 && y2 < 1) return;

	if (y1 < 1) {
		float t = (1 - y1) / (y2 - y1);
		x1 = x1 + t * (x2 - x1);
		y1 = 1;
		z1 = z1 + t * (z2 - z1);
	} else if (y2 < 1) {
		float t = (1 - y2) / (y1 - y2);
		x2 = x2 + t * (x1 - x2);
		y2 = 1;
		z2 = z2 + t * (z1 - z2);
	}
}

/**
 * @brief Draw a texture on the screen (testing function)
 */
void Renderer::testTextures() {
	int t = 0;
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 16; x++) {
			int p = (16 - y - 1) * 3 * 16 + x * 3;
			int r = textures[t].textureMap[p + 0];
			int g = textures[t].textureMap[p + 1];
			int b = textures[t].textureMap[p + 2];
			mainWindow.drawPixel(x, y, RgbColor{r, g, b});
		}
	}
}

/**
 * @brief Draw an infinite floor (testing function)
 */
void Renderer::testFloor() {
	float lookAngle = -player.lookAngle * 6.2;
	if (lookAngle > SCR_HEIGHT) lookAngle = SCR_HEIGHT;

	float moveVertical = player.z / 16.0;
	if (moveVertical == 0) moveVertical = 0.0001;

	int yStart = -SCR_HEIGHT_HALF, yEnd = -lookAngle;
	if (moveVertical < 0) {
		yStart = -lookAngle;
		yEnd = SCR_HEIGHT_HALF + lookAngle;
	}

	for (int y = yStart; y < yEnd; y++) {
		for (int x = -SCR_WIDTH_HALF; x < SCR_WIDTH_HALF; x++) {
			float z = y + lookAngle;
			if (z == 0) z = 0.0001;
			float fx = x / static_cast<float>(z) * moveVertical;
			float fy = 200 / static_cast<float>(z) * moveVertical;
			float rx = fx * sin(player.angle * M_PI / 180) - fy * cos(player.angle * M_PI / 180) +
					   (player.y / 60.0);
			float ry = fx * cos(player.angle * M_PI / 180) + fy * sin(player.angle * M_PI / 180) -
					   (player.x / 60.0);

			if (rx < 0) rx = -rx + 1;
			if (ry < 0) ry = -ry + 1;
			if (rx <= 0 || ry <= 0 || rx >= 5 || ry >= 5) continue;

			if (static_cast<int>(rx) % 2 == static_cast<int>(ry) % 2) {
				mainWindow.drawPixel(x + SCR_WIDTH_HALF, y + SCR_HEIGHT_HALF, RgbColor{255, 0, 0});
			} else {
				mainWindow.drawPixel(x + SCR_WIDTH_HALF, y + SCR_HEIGHT_HALF, RgbColor{0, 0, 255});
			}
		}
	}
}

/**
 * @brief Draw a wall on the screen given its coordinates after calculating the perspective
 */
void Renderer::renderBlockSurfaces(Block *block, int xPos1, int xPos2, int bottomPos1,
								   int bottomPos2, int topPos1, int topPos2, int orientation, int w) {
	int wallTexture = 0;
	float horizontalStart = 0,
		  horizontalStep = static_cast<float>(textures[wallTexture].width * uvU) /
						   static_cast<float>(xPos2 - xPos1);

	float distBottom = bottomPos2 - bottomPos1;
	float distTop = topPos2 - topPos1;
	float distX = (xPos2 - xPos1 == 0) ? 1 : xPos2 - xPos1;
	float originalX1 = xPos1;

	// X clipping
	if (xPos1 < 0) {
		xPos1 = 0;
		horizontalStart = (0 - originalX1) * horizontalStep;
	}
	if (xPos1 > SCR_WIDTH) xPos1 = SCR_WIDTH;
	if (xPos2 < 0) xPos2 = 0;
	if (xPos2 > SCR_WIDTH) xPos2 = SCR_WIDTH;

    int y1;
    int y2;
    float verticalStart;
    float verticalStep;

    // Lambda used to not repeat code
    // Not using a function since many parameters would need to be passed
    std::function<void(int)> calculatePixelY = [&](int x) {
		y1 = bottomPos1 + distBottom * (x - originalX1 + 0.5) / distX;
		y2 = topPos1 + distTop * (x - originalX1 + 0.5) / distX;

		verticalStart = 0;
		verticalStep = static_cast<float>(textures[wallTexture].height * uvV) /
							 static_cast<float>(y2 - y1);

		// Y clipping
		if (y1 < 0) {
			verticalStart = (0 - y1) * verticalStep;
			y1 = 0;
		}
		if (y1 > SCR_HEIGHT) y1 = SCR_HEIGHT;
		if (y2 < 0) y2 = 0;
		if (y2 > SCR_HEIGHT) y2 = SCR_HEIGHT;
    };

    if (orientation == 0) {
        for (int x = xPos1; x < xPos2; x++) {
            calculatePixelY(x);

            if (surfaceOrientation == 1) surfacePoints[x] = y1;
            if (surfaceOrientation == 2) surfacePoints[x] = y2;

            // Prevent rendering of invisible faces
            if (w == 0 && !block->visibleFaces[BlockFace::YNeg]) continue;
            if (w == 1 && !block->visibleFaces[BlockFace::XPos]) continue;
            if (w == 2 && !block->visibleFaces[BlockFace::YPos]) continue;
            if (w == 3 && !block->visibleFaces[BlockFace::XNeg]) continue;
            for (int y = y1; y < y2; y++) {
                int p = ((textures[wallTexture].height -
                            (static_cast<int>(verticalStart) % textures[wallTexture].height) - 1) *
                        3 * textures[wallTexture].width +
                        (static_cast<int>(horizontalStart) % textures[wallTexture].width) * 3);
                int r = std::max(textures[wallTexture].textureMap[p + 0] - shade / 2, 0);
                int g = std::max(textures[wallTexture].textureMap[p + 1] - shade / 2, 0);
                int b = std::max(textures[wallTexture].textureMap[p + 2] - shade / 2, 0);

                mainWindow.drawPixel(x, y, RgbColor{r, g, b}, distanceToPlayer);

                verticalStart += verticalStep;
            }
            horizontalStart += horizontalStep;
        }
    } else {
        for (int x = xPos1; x < xPos2; x++) {
            calculatePixelY(x);

            int xOffset = SCR_WIDTH_HALF;
            int yOffset = SCR_HEIGHT_HALF;
            int xNew = x - xOffset;
            int wallOffset = 0;
            float tile = surfaceScale * 120;  // Modify this to change texture scale

            if (surfaceOrientation == 1) {
                if (!block->visibleFaces[BlockFace::ZNeg]) break;
                y2 = surfacePoints[x];
                wallOffset = blockZBottom;
            }
            if (surfaceOrientation == 2) {
                if (!block->visibleFaces[BlockFace::ZPos]) break;
                y1 = surfacePoints[x];
                wallOffset = blockZTop;
            }

            float lookAngle = -player.lookAngle * 6.2;	// 6.2 just works (obtained by trial)
            float moveVertical = (player.z - wallOffset) / static_cast<float>(yOffset);

            if (lookAngle > SCR_HEIGHT) lookAngle = SCR_HEIGHT;
            if (moveVertical == 0) moveVertical = 0.0001;

            int yStart = y1 - SCR_HEIGHT_HALF, yEnd = y2 - yOffset;

            for (int y = yStart; y < yEnd; y++) {
                float z = y + lookAngle;
                if (z == 0) z = 0.0001;

                float fx = xNew / static_cast<float>(z) * moveVertical * tile;
                float fy = player.fov / static_cast<float>(z) * moveVertical * tile;

                float rx = fx * sin(player.angle * M_PI / 180) -
                    fy * cos(player.angle * M_PI / 180) + (player.y / 75.0 * tile);
                float ry = fx * cos(player.angle * M_PI / 180) +
                    fy * sin(player.angle * M_PI / 180) - (player.x / 75.0 * tile);

                if (rx < 0) rx = -rx + 1;
                if (ry < 0) ry = -ry + 1;
                int _texture = 1;
                int p = static_cast<int>(textures[_texture].height -
                        (static_cast<int>(ry) % textures[_texture].height) - 1) *
                    3 * textures[_texture].width +
                    (static_cast<int>(rx) % textures[_texture].width) * 3;
                int r = textures[_texture].textureMap[p + 0];
                int g = textures[_texture].textureMap[p + 1];
                int b = textures[_texture].textureMap[p + 2];

                mainWindow.drawPixel(xNew + xOffset, y + yOffset, RgbColor{r, g, b},
                        distanceToPlayer + (surfaceOrientation == 1 ? 0.01 : 0));
            }
        }
    }
}

void Renderer::renderChunk(Chunk *chunk) {
    chunkX = chunk->x;
    chunkY = chunk->y;
    chunkZ = chunk->z;

    for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
            for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
                renderBlock(chunk->getBlock(x, y, z), x, y, z);
            }
        }
    }
}

/**
 * @brief Render the block
 */
void Renderer::renderBlock(Block *block, int x, int y, int z) {
	float cs = cos(player.angle * M_PI / 180);
	float sn = sin(player.angle * M_PI / 180);

	distanceToPlayer = 0;

	blockX = chunkX + x * Block::BLOCK_SIZE;
	blockY = chunkY + y * Block::BLOCK_SIZE;
	blockZBottom = chunkZ + z * Block::BLOCK_SIZE;
	blockZTop = blockZBottom + Block::BLOCK_SIZE;

	int cycles = 2;

	if (player.z < blockZBottom) {
		surfaceOrientation = 1;
		std::fill(surfacePoints.begin(), surfacePoints.end(), SCR_HEIGHT);
	} else if (player.z > blockZTop) {
		surfaceOrientation = 2;
		std::fill(surfacePoints.begin(), surfacePoints.end(), 0);
	} else {
		surfaceOrientation = 0;
		cycles = 1;
	}

	for (int orientation = 0; orientation < cycles; orientation++) {
		for (int w = 0; w < 4; w++) {
			xBottom1[w] = blockX + (w == 1 || w == 2) * Block::BLOCK_SIZE;
			yBottom1[w] = blockY + (w == 2 || w == 3) * Block::BLOCK_SIZE;
			xBottom2[w] = blockX + (w == 0 || w == 1) * Block::BLOCK_SIZE;
			yBottom2[w] = blockY + (w == 1 || w == 2) * Block::BLOCK_SIZE;

			float x1 = xBottom1[w] - player.x, y1 = yBottom1[w] - player.y;
			float x2 = xBottom2[w] - player.x, y2 = yBottom2[w] - player.y;

			if (orientation == 1) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}

			wallX[w][0] = x1 * cs - y1 * sn;
			wallY[w][0] = y1 * cs + x1 * sn;

			wallX[w][1] = x2 * cs - y2 * sn;
			wallY[w][1] = y2 * cs + x2 * sn;

			wallX[w][2] = wallX[w][0];
			wallY[w][2] = wallY[w][0];

			wallX[w][3] = wallX[w][1];
			wallY[w][3] = wallY[w][1];

			wallZ[w][0] = blockZBottom - player.z + ((player.lookAngle * wallY[w][0]) / 32.0);
			wallZ[w][1] = blockZBottom - player.z + ((player.lookAngle * wallY[w][1]) / 32.0);
			wallZ[w][2] = blockZTop - player.z + ((player.lookAngle * wallY[w][0]) / 32.0);
			wallZ[w][3] = blockZTop - player.z + ((player.lookAngle * wallY[w][1]) / 32.0);

			if (orientation == 0)
				distanceToPlayer += sqrt(pow((wallX[w][0] + wallX[w][1]) / 2, 2) +
										 pow((wallY[w][0] + wallY[w][1]) / 2, 2));
		}

		if (orientation == 0) distanceToPlayer /= 4;

		for (int w = 0; w < 4; w++) {
			// Check if both bottom points behind player
			if (wallY[w][0] < 1 && wallY[w][1] < 1) continue;

            // Prevent rendering of invisible faces
            if (w == 1 && !block->visibleFaces[BlockFace::XPos]) continue;
            if (w == 2 && !block->visibleFaces[BlockFace::YPos]) continue;
            if (w == 3 && !block->visibleFaces[BlockFace::XNeg]) continue;
            if (w == 0 && !block->visibleFaces[BlockFace::YNeg]) continue;

			// Clip wall
			clipBehindPlayer(wallX[w][0], wallY[w][0], wallZ[w][0], wallX[w][1], wallY[w][1],
							 wallZ[w][1]);
			clipBehindPlayer(wallX[w][2], wallY[w][2], wallZ[w][2], wallX[w][3], wallY[w][3],
							 wallZ[w][3]);

			wallX[w][0] = wallX[w][0] * 200 / wallY[w][0] + SCR_WIDTH_HALF;
			wallY[w][0] = wallZ[w][0] * 200 / wallY[w][0] + SCR_HEIGHT_HALF;
			wallX[w][1] = wallX[w][1] * 200 / wallY[w][1] + SCR_WIDTH_HALF;
			wallY[w][1] = wallZ[w][1] * 200 / wallY[w][1] + SCR_HEIGHT_HALF;
			wallX[w][2] = wallX[w][2] * 200 / wallY[w][2] + SCR_WIDTH_HALF;
			wallY[w][2] = wallZ[w][2] * 200 / wallY[w][2] + SCR_HEIGHT_HALF;
			wallX[w][3] = wallX[w][3] * 200 / wallY[w][3] + SCR_WIDTH_HALF;
			wallY[w][3] = wallZ[w][3] * 200 / wallY[w][3] + SCR_HEIGHT_HALF;

			renderBlockSurfaces(block, static_cast<int>(wallX[w][0]), static_cast<int>(wallX[w][1]),
								static_cast<int>(wallY[w][0]), static_cast<int>(wallY[w][1]),
								static_cast<int>(wallY[w][2]), static_cast<int>(wallY[w][3]),
								orientation, w);
		}
	}
}
