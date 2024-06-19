#include "3D_ENGINE/renderer.hpp"
#include "3D_ENGINE/sector.hpp"
#include "3D_ENGINE/wall.hpp"
#include "3D_ENGINE/window.hpp"
#include "3D_ENGINE/player.hpp"
#include "3D_ENGINE/window_defines.hpp"
#include "3D_ENGINE/renderer.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>

Renderer renderer;

void Renderer::clipBehindPlayer(float *x1, float *y1, float *z1, float *x2, float *y2, float *z2) {
    // If both points behind the player
    if (*y1 < 1 && *y2 < 1) return; 

    if (*y1 < 1) {
        float t = (1 - *y1) / (*y2 - *y1);
        *x1 = *x1 + t * (*x2 - *x1);
        *y1 = 1; 
        *z1 = *z1 + t * (*z2 - *z1);
    } 
    else if (*y2 < 1) {
        float t = (1 - *y2) / (*y1 - *y2);
        *x2 = *x2 + t * (*x1 - *x2);
        *y2 = 1; 
        *z2 = *z2 + t * (*z1 - *z2);
    }
}

void Renderer::drawWall(int xPos1, int xPos2, int bottomPos1, int bottomPos2, int topPos1, int topPos2, RgbColor color, Sector &sector, int orientation) {
    float distBottom    = bottomPos2 - bottomPos1;
    float distTop       = topPos2 - topPos1;
    float distX         = (xPos2 - xPos1 == 0) ? 1 : xPos2 - xPos1;
    float originalX1    = xPos1;

    if (xPos1 < 0)          xPos1 = 0;
    if (xPos1 > SCR_WIDTH)  xPos1 = SCR_WIDTH;
    if (xPos2 < 0)          xPos2 = 0;
    if (xPos2 > SCR_WIDTH)  xPos2 = SCR_WIDTH;

    for (int x = xPos1; x < xPos2; x++) {
        int y1 = bottomPos1 + distBottom * (x - originalX1 + 0.5) / distX;
        int y2 = topPos1 + distTop * (x - originalX1 + 0.5) / distX;

        // Y clipping
        if (y1 < 0)             y1 = 0;
        if (y1 > SCR_HEIGHT)    y1 = SCR_HEIGHT;
        if (y2 < 0)             y2 = 0;
        if (y2 > SCR_HEIGHT)    y2 = SCR_HEIGHT;


        if (orientation == 0) {
            if (sector.surfaceOrientation == 1) sector.surfacePoints[x] = y1;
            if (sector.surfaceOrientation == 2) sector.surfacePoints[x] = y2;
            for (int y = y1; y < y2; y++)
                mainWindow.pixel(x, y, color);
        }

        if (orientation == 1) {
            if (sector.surfaceOrientation == 1) y2 = sector.surfacePoints[x];
            if (sector.surfaceOrientation == 2) y1 = sector.surfacePoints[x];
            for (int y = y1; y < y2; y++)
                mainWindow.pixel(x, y, (sector.surfaceOrientation == 1) ? sector.bottomColor : sector.topColor);
        }
    }
}

void Renderer::draw3D() {
    float wx[4], wy[4], wz[4];
    float cs = cos(player.angle * M_PI / 180);
    float sn = sin(player.angle * M_PI / 180);

    std::sort(sectors.begin(), sectors.end(), [](Sector& a, Sector& b) {
        return a.distanceToPlayer > b.distanceToPlayer;
    });

    for (Sector &sector : sectors) {
        sector.distanceToPlayer = 0;

        int cycles = 2;

        if (player.z < sector.zBottom) { 
            sector.surfaceOrientation = 1;
            std::fill(sector.surfacePoints.begin(), sector.surfacePoints.end(), SCR_HEIGHT);
        }
        else if (player.z > sector.zTop) { 
            sector.surfaceOrientation = 2; 
            std::fill(sector.surfacePoints.begin(), sector.surfacePoints.end(), 0);
        }
        else { 
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

                wx[0] = x1 * cs - y1 * sn;
                wy[0] = y1 * cs + x1 * sn;

                wx[1] = x2 * cs - y2 * sn;
                wy[1] = y2 * cs + x2 * sn;

                wx[2] = wx[0];
                wy[2] = wy[0];

                wx[3] = wx[1];
                wy[3] = wy[1];

                sector.distanceToPlayer += wall.getDistanceToPlayer(wx[0], wy[0], wx[1], wy[1]);

                wz[0] = sector.zBottom - player.z + ((player.lookAngle * wy[0]) / 32.0);
                wz[1] = sector.zBottom - player.z + ((player.lookAngle * wy[1]) / 32.0);
                wz[2] = sector.zTop - player.z + ((player.lookAngle * wy[0]) / 32.0);
                wz[3] = sector.zTop - player.z + ((player.lookAngle * wy[1]) / 32.0);

                // Check if both bottom points are still behind the player after clipping
                if (wy[0] < 1 && wy[1] < 1) continue;

                // Clip wall
                clipBehindPlayer(&wx[0], &wy[0], &wz[0], &wx[1], &wy[1], &wz[1]);
                clipBehindPlayer(&wx[2], &wy[2], &wz[2], &wx[3], &wy[3], &wz[3]);

                wx[0] = wx[0] * 200 / wy[0] + SCR_WIDTH_HALF;
                wy[0] = wz[0] * 200 / wy[0] + SCR_HEIGHT_HALF;
                wx[1] = wx[1] * 200 / wy[1] + SCR_WIDTH_HALF;
                wy[1] = wz[1] * 200 / wy[1] + SCR_HEIGHT_HALF;
                wx[2] = wx[2] * 200 / wy[2] + SCR_WIDTH_HALF;
                wy[2] = wz[2] * 200 / wy[2] + SCR_HEIGHT_HALF;
                wx[3] = wx[3] * 200 / wy[3] + SCR_WIDTH_HALF;
                wy[3] = wz[3] * 200 / wy[3] + SCR_HEIGHT_HALF;

                drawWall(static_cast<int>(wx[0]), static_cast<int>(wx[1]), static_cast<int>(wy[0]), static_cast<int>(wy[1]), static_cast<int>(wy[2]), static_cast<int>(wy[3]), wall.color, sector, orientation);

                // if (wx[0] >= 0 && wx[0] < SCR_WIDTH && wy[0] >= 0 && wy[0] < SCR_HEIGHT)
                //     mainWindow.pixel(static_cast<int>(wx[0]), static_cast<int>(wy[0]), debugColor);
                // if (wx[1] >= 0 && wx[1] < SCR_WIDTH && wy[1] >= 0 && wy[1] < SCR_HEIGHT)
                //     mainWindow.pixel(static_cast<int>(wx[1]), static_cast<int>(wy[1]), debugColor);
                // if (wx[2] >= 0 && wx[2] < SCR_WIDTH && wy[2] >= 0 && wy[2] < SCR_HEIGHT)
                //     mainWindow.pixel(static_cast<int>(wx[2]), static_cast<int>(wy[2]), debugColor);
                // if (wx[3] >= 0 && wx[3] < SCR_WIDTH && wy[3] >= 0 && wy[3] < SCR_HEIGHT)
                //     mainWindow.pixel(static_cast<int>(wx[3]), static_cast<int>(wy[3]), debugColor);
            }

            sector.distanceToPlayer /= sector.walls.size();
        }
    }
}
