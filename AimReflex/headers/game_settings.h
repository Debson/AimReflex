#pragma once
//SDL_Color gameColor = { 255, 0, 255, 255};

// Resolution
const int SCREEN_WIDTH = 920;
const int SCREEN_HEIGHT = 480;

const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;

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

// Points for hir
const int HIT = 300;
// Points for miss
const int MISS = 100;
// Points when target expire
const int TARGET_MISS = 500;
