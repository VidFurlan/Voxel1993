#pragma once

struct rgbColor {
    short r, g, b;
};

typedef struct {
    int fr1, fr2; // frame 1 frame 2, to create constant frame rate
} frametime;

typedef struct {
    int w, s, a, d; // move up, down, left, right
    int sl, sr;     // strafe left, right
    int m;          // move up, down, look up, down
} keys;

const unsigned int res = 1;
const unsigned int pixelScale = 8 / res;
const unsigned int SCR_WIDTH = 200 * res;
const unsigned int SCR_HEIGHT = 150 * res;
const unsigned int SCR_WIDTH_HALF = SCR_WIDTH / 2;
const unsigned int SCR_HEIGHT_HALF = SCR_HEIGHT / 2;
const unsigned int GLSCR_WIDTH = SCR_WIDTH * pixelScale;
const unsigned int GLSCR_HEIGHT = SCR_HEIGHT * pixelScale;

const rgbColor red = {255, 0, 0};
const rgbColor green = {0, 255, 0};
const rgbColor blue = {0, 0, 255};
const rgbColor yellow = {255, 255, 0};
const rgbColor cyan = {0, 255, 255};
const rgbColor magenta = {255, 0, 255};
const rgbColor white = {255, 255, 255};
const rgbColor black = {0, 0, 0};
