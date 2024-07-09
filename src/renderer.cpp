#include "3D_ENGINE/renderer.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <climits>
#include <cmath>

#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/renderer.hpp"
#include "3D_ENGINE/sector.hpp"
#include "3D_ENGINE/texture_list.hpp"
#include "3D_ENGINE/wall.hpp"
#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/window_defines.hpp"

Renderer renderer;

Renderer::Renderer() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				sectors.push_back(
					Sector(k * 10 + 0, k * 10 + 10, 0, 0,
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

void Renderer::clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2) {
	// If both points behind the player
	if (*y1 < 1 && *y2 < 1) return;

	if (*y1 < 1) {
		float t = (1 - *y1) / (*y2 - *y1);
		*x1 = *x1 + t * (*x2 - *x1);
		*y1 = 1;
		*z1 = *z1 + t * (*z2 - *z1);
	} else if (*y2 < 1) {
		float t = (1 - *y2) / (*y1 - *y2);
		*x2 = *x2 + t * (*x1 - *x2);
		*y2 = 1;
		*z2 = *z2 + t * (*z1 - *z2);
	}
}

void Renderer::testTextures() {
	int t = 0;
	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 16; x++) {
			int p = (16 - y - 1) * 3 * 16 + x * 3;
			int r = textures[t].textureMap[p + 0];
			int g = textures[t].textureMap[p + 1];
			int b = textures[t].textureMap[p + 2];
			mainWindow.drawPixel(x, y, RgbColor{r, g, b});
		}
}

void Renderer::drawFloor() {
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
			float fx = x / (float)z * moveVertical;
			float fy = 200 / (float)z * moveVertical;
			float rx = fx * sin(player.angle * M_PI / 180) - fy * cos(player.angle * M_PI / 180) +
					   (player.y / 60.0);
			float ry = fx * cos(player.angle * M_PI / 180) + fy * sin(player.angle * M_PI / 180) -
					   (player.x / 60.0);

			if (rx < 0) rx = -rx + 1;
			if (ry < 0) ry = -ry + 1;
			if (rx <= 0 || ry <= 0 || rx >= 5 || ry >= 5) continue;
			if ((int)rx % 2 == (int)ry % 2)
				mainWindow.drawPixel(x + SCR_WIDTH_HALF, y + SCR_HEIGHT_HALF, RgbColor{255, 0, 0});
			else
				mainWindow.drawPixel(x + SCR_WIDTH_HALF, y + SCR_HEIGHT_HALF, RgbColor{0, 0, 255});
		}
	}
}

void Renderer::drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1,
						int topPos2, RgbColor color, Sector &sector, Wall &wall, int orientation) {
	int wallTexture = 0;
	float horizontalStart = 0,
		  horizontalStep = (float)textures[wallTexture].width * wall.uvU / (float)(xPos2 - xPos1);

	float distBottom = bottomPos2 - bottomPos1;
	float distTop = topPos2 - topPos1;
	float distX = (xPos2 - xPos1 == 0) ? 1 : xPos2 - xPos1;
	float originalX1 = xPos1;

	// X clipping
	if (xPos1 < 0) {
		xPos1 = 0;
		horizontalStart = (0 - originalX1) * horizontalStep;
	};
	if (xPos1 > SCR_WIDTH) xPos1 = SCR_WIDTH;
	if (xPos2 < 0) xPos2 = 0;
	if (xPos2 > SCR_WIDTH) xPos2 = SCR_WIDTH;

	for (int x = xPos1; x < xPos2; x++) {
		int y1 = bottomPos1 + distBottom * (x - originalX1 + 0.5) / distX;
		int y2 = topPos1 + distTop * (x - originalX1 + 0.5) / distX;

		float verticalStart = 0,
			  verticalStep = (float)textures[wallTexture].height * wall.uvV / (float)(y2 - y1);

		// Y clipping
		if (y1 < 0) {
			verticalStart = (0 - y1) * verticalStep;
			y1 = 0;
		}
		if (y1 > SCR_HEIGHT) y1 = SCR_HEIGHT;
		if (y2 < 0) y2 = 0;
		if (y2 > SCR_HEIGHT) y2 = SCR_HEIGHT;

		if (orientation == 0) {
			if (sector.surfaceOrientation == 1) sector.surfacePoints[x] = y1;
			if (sector.surfaceOrientation == 2) sector.surfacePoints[x] = y2;

			for (int y = y1; y < y2; y++) {
				int p = ((textures[wallTexture].height -
						  (static_cast<int>(verticalStart) % textures[wallTexture].height) - 1) *
							 3 * textures[wallTexture].width +
						 (static_cast<int>(horizontalStart) % textures[wallTexture].width) * 3);
				int r = std::max(textures[wallTexture].textureMap[p + 0] - wall.shade / 2, 0);
				int g = std::max(textures[wallTexture].textureMap[p + 1] - wall.shade / 2, 0);
				int b = std::max(textures[wallTexture].textureMap[p + 2] - wall.shade / 2, 0);

				mainWindow.drawPixel(x, y, RgbColor{r, g, b}, sector.distanceToPlayer);

				verticalStart += verticalStep;
			}
			horizontalStart += horizontalStep;
		} else if (orientation == 1) {
			int xOffset = SCR_WIDTH_HALF;
			int yOffset = SCR_HEIGHT_HALF;
			int xNew = x - xOffset;
			int wallOffset = 0;
			float tile = sector.surfaceScale * 120;	 // Modify this to change texture scale

			if (sector.surfaceOrientation == 1) {
				y2 = sector.surfacePoints[x];
				wallOffset = sector.zBottom;
			}
			if (sector.surfaceOrientation == 2) {
				y1 = sector.surfacePoints[x];
				wallOffset = sector.zTop;
			}

			float lookAngle = -player.lookAngle * 6.2;
			float moveVertical = (player.z - wallOffset) / (float)yOffset;

			if (lookAngle > SCR_HEIGHT) lookAngle = SCR_HEIGHT;
			if (moveVertical == 0) moveVertical = 0.0001;

			int yStart = y1 - SCR_HEIGHT_HALF, yEnd = y2 - yOffset;

			for (int y = yStart; y < yEnd; y++) {
				float z = y + lookAngle;
				if (z == 0) z = 0.0001;

				float fx = xNew / (float)z * moveVertical * tile;
				float fy = player.fov / (float)z * moveVertical * tile;

				float rx = fx * sin(player.angle * M_PI / 180) -
						   fy * cos(player.angle * M_PI / 180) + (player.y / 75.0 * tile);
				float ry = fx * cos(player.angle * M_PI / 180) +
						   fy * sin(player.angle * M_PI / 180) - (player.x / 75.0 * tile);

				if (rx < 0) rx = -rx + 1;
				if (ry < 0) ry = -ry + 1;
				int _texture = 1;
				int p =
					(int)(textures[_texture].height - ((int)ry % textures[_texture].height) - 1) *
						3 * textures[_texture].width +
					((int)rx % textures[_texture].width) * 3;
				int r = textures[_texture].textureMap[p + 0];
				int g = textures[_texture].textureMap[p + 1];
				int b = textures[_texture].textureMap[p + 2];

				mainWindow.drawPixel(
					xNew + xOffset, y + yOffset, RgbColor{r, g, b},
					sector.distanceToPlayer + (sector.surfaceOrientation == 1 ? 0.01 : 0));
			}
		}
	}
}

void Renderer::draw3D() {
	float cs = cos(player.angle * M_PI / 180);
	float sn = sin(player.angle * M_PI / 180);
    
    // Uncomment to use painter's algorithm
	// std::sort(sectors.begin(), sectors.end(),
	// 		  [](Sector &a, Sector &b) { return a.distanceToPlayer > b.distanceToPlayer; });

	for (Sector &sector : sectors) {
		sector.distanceToPlayer = 0;

		int cycles = 2;

		if (player.z < sector.zBottom) {
			sector.surfaceOrientation = 1;
			std::fill(sector.surfacePoints.begin(), sector.surfacePoints.end(), SCR_HEIGHT);
		} else if (player.z > sector.zTop) {
			sector.surfaceOrientation = 2;
			std::fill(sector.surfacePoints.begin(), sector.surfacePoints.end(), 0);
		} else {
			sector.surfaceOrientation = 0;
			cycles = 1;
		}

		for (int orientation = 0; orientation < cycles; orientation++) {
			for (Wall &wall : sector.walls) {
				float x1 = wall.xBottom1 - player.x, y1 = wall.yBottom1 - player.y;
				float x2 = wall.xBottom2 - player.x, y2 = wall.yBottom2 - player.y;

				if (orientation == 1) {
					std::swap(x1, x2);
					std::swap(y1, y2);
				}

				wall.wallX[0] = x1 * cs - y1 * sn;
				wall.wallY[0] = y1 * cs + x1 * sn;

				wall.wallX[1] = x2 * cs - y2 * sn;
				wall.wallY[1] = y2 * cs + x2 * sn;

				wall.wallX[2] = wall.wallX[0];
				wall.wallY[2] = wall.wallY[0];

				wall.wallX[3] = wall.wallX[1];
				wall.wallY[3] = wall.wallY[1];

				wall.wallZ[0] =
					sector.zBottom - player.z + ((player.lookAngle * wall.wallY[0]) / 32.0);
				wall.wallZ[1] =
					sector.zBottom - player.z + ((player.lookAngle * wall.wallY[1]) / 32.0);
				wall.wallZ[2] =
					sector.zTop - player.z + ((player.lookAngle * wall.wallY[0]) / 32.0);
				wall.wallZ[3] =
					sector.zTop - player.z + ((player.lookAngle * wall.wallY[1]) / 32.0);

				if (orientation == 0)
					sector.distanceToPlayer += sqrt(pow((wall.wallX[0] + wall.wallX[1]) / 2, 2) +
													pow((wall.wallY[0] + wall.wallY[1]) / 2, 2));
			}

			if (orientation == 0) sector.distanceToPlayer /= sector.walls.size();

			for (Wall &wall : sector.walls) {
				// Check if both bottom points behind player after
				// clipping
				if (wall.wallY[0] < 1 && wall.wallY[1] < 1) continue;

				// Clip wall
				clipBehindPlayer(&wall.wallX[0], &wall.wallY[0], &wall.wallZ[0], &wall.wallX[1],
								 &wall.wallY[1], &wall.wallZ[1]);
				clipBehindPlayer(&wall.wallX[2], &wall.wallY[2], &wall.wallZ[2], &wall.wallX[3],
								 &wall.wallY[3], &wall.wallZ[3]);

				wall.wallX[0] = wall.wallX[0] * 200 / wall.wallY[0] + SCR_WIDTH_HALF;
				wall.wallY[0] = wall.wallZ[0] * 200 / wall.wallY[0] + SCR_HEIGHT_HALF;
				wall.wallX[1] = wall.wallX[1] * 200 / wall.wallY[1] + SCR_WIDTH_HALF;
				wall.wallY[1] = wall.wallZ[1] * 200 / wall.wallY[1] + SCR_HEIGHT_HALF;
				wall.wallX[2] = wall.wallX[2] * 200 / wall.wallY[2] + SCR_WIDTH_HALF;
				wall.wallY[2] = wall.wallZ[2] * 200 / wall.wallY[2] + SCR_HEIGHT_HALF;
				wall.wallX[3] = wall.wallX[3] * 200 / wall.wallY[3] + SCR_WIDTH_HALF;
				wall.wallY[3] = wall.wallZ[3] * 200 / wall.wallY[3] + SCR_HEIGHT_HALF;

				drawWall(static_cast<int>(wall.wallX[0]), static_cast<int>(wall.wallX[1]),
						 static_cast<int>(wall.wallY[0]), static_cast<int>(wall.wallY[1]),
						 static_cast<int>(wall.wallY[2]), static_cast<int>(wall.wallY[3]),
						 wall.color, sector, wall, orientation);

				// if (wall.wallX[0] >= 0 && wall.wallX[0] < SCR_WIDTH
				// && wall.wallY[0] >= 0 && wall.wallY[0] <
				// SCR_HEIGHT)
				//     mainWindow.pixel(static_cast<int>(wall.wallX[0]),
				//     static_cast<int>(wall.wallY[0]), debugColor);
				// if (wall.wallX[1] >= 0 && wall.wallX[1] < SCR_WIDTH
				// && wall.wallY[1] >= 0 && wall.wallY[1] <
				// SCR_HEIGHT)
				//     mainWindow.pixel(static_cast<int>(wall.wallX[1]),
				//     static_cast<int>(wall.wallY[1]), debugColor);
				// if (wall.wallX[2] >= 0 && wall.wallX[2] < SCR_WIDTH
				// && wall.wallY[2] >= 0 && wall.wallY[2] <
				// SCR_HEIGHT)
				//     mainWindow.pixel(static_cast<int>(wall.wallX[2]),
				//     static_cast<int>(wall.wallY[2]), debugColor);
				// if (wall.wallX[3] >= 0 && wall.wallX[3] < SCR_WIDTH
				// && wall.wallY[3] >= 0 && wall.wallY[3] <
				// SCR_HEIGHT)
				//     mainWindow.pixel(static_cast<int>(wall.wallX[3]),
				//     static_cast<int>(wall.wallY[3]), debugColor);
			}
		}
	}
}
