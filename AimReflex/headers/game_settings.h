#pragma once
#include <SDL.h>

// Screen resolution
const int SCREEN_WIDTH = 920;
const int SCREEN_HEIGHT = 480;

// Game window resolution
const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;

// Frames per second
const int FRAMES_PER_SECOND = 30;

// Background start color
const SDL_Color BACKGROUND_COLOR = { 0, 0, 0, 255 };

// Target start color
const SDL_Color TARGET_COLOR = { 255, 140, 0 };

// How many levels of difficulty
const int LEVEL_COUNT = 20;

// How many targets on game start
const int START_TARGET_COUNT = 2;

// Max targets in game
const int GAME_TARGETS_COUNT = 20;

// Max target's scale
const float MAX_TARGET_SCALE = 0.05f;

// MAX_WIDTH = original_image_width * MAX_SCALE  <= max width to which target scales to
const int MAX_TARGET_WIDTH = 1000 / 100 * 5;

// MAX_HEIGHT = original_image_height * MAX_SCALE <= max height to which target scales to
const int MAX_TARGET_HEIGHT = 1000 / 100 * 5;

// Points for hit
const int HIT = 300;

// Points for miss
const int MISS = 100;

// Points when target expire
const int TARGET_MISS = 500;
