#ifndef PADDLE_PONG
#define PADDLE_PONG

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


class Paddle {
public:
    float x, y, width, height;
    float speed;
    bool shouldMove;
    SDL_Color color;
    char score;
    SDL_Texture * renderedScore = nullptr;

    void leftSetup(SDL_Renderer * renderer, TTF_Font * font);
    void rightSetup(SDL_Renderer * renderer, TTF_Font * font);
    void moveUp();
    void moveDown();
    void move(float deltaTime);
    void stop();

    void freeScore();
    bool renderScore(SDL_Renderer * renderer, TTF_Font * font);
};

#endif