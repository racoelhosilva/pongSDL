#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL_render.h>

static int WINDOW_WIDTH = 1920;
static int WINDOW_HEIGHT = 1080;

static SDL_Color RIGHT_PADDLE_COLOR = {0,0,222,200};
static SDL_Color LEFT_PADDLE_COLOR = {222,0,0,200};

static int maxScore = 9;

static float difficultyMultiplier = 1.0;

static int FPS = 30;
static float FRAME_TARGET_TIME = 1000.0 / FPS;

#endif