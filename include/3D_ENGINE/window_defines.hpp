#pragma once

struct RgbColor {
	int r, g, b;
};

struct Keys {
	int w = 0, s = 0, a = 0, d = 0;	 // move up, down, left, right
	int sl = 0, sr = 0;				 // strafe left, right
	int m = 0;						 // move up, down, look up, down
};

// Window settings
const int res = 1;
const int pixelScale = 8 / res;
const int SCR_WIDTH = 200 * res;
const int SCR_HEIGHT = 150 * res;
const int SCR_WIDTH_HALF = SCR_WIDTH / 2;
const int SCR_HEIGHT_HALF = SCR_HEIGHT / 2;
const int GLSCR_WIDTH = SCR_WIDTH * pixelScale;
const int GLSCR_HEIGHT = SCR_HEIGHT * pixelScale;

// Colors
const RgbColor red = {255, 0, 0};
const RgbColor green = {0, 255, 0};
const RgbColor blue = {0, 0, 255};
const RgbColor yellow = {255, 255, 0};
const RgbColor cyan = {0, 255, 255};
const RgbColor magenta = {255, 0, 255};
const RgbColor white = {255, 255, 255};
const RgbColor black = {0, 0, 0};

const RgbColor backgroundColor = cyan;
const RgbColor debugColor = magenta;
