#include "paddle.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

void Paddle::leftSetup(SDL_Renderer * renderer, TTF_Font * font){
    x = 80; y = 500;
    width = 20; height = 240;
    speed = 0;
    score = 0;
    renderScore(renderer, font);
}

void Paddle::rightSetup(SDL_Renderer * renderer, TTF_Font * font){
    x = 1800; y = 500;
    width = 20; height = 240;
    speed = 0;
    score = 0;
    renderScore(renderer, font);
}

void Paddle::moveDown(){
    speed = 1000;
}
void Paddle::moveUp(){
    speed = -1000;
}

void Paddle::stop(){
    speed = 0;
}

void Paddle::move(float deltaTime){
    y += speed * deltaTime;
    if (y <= 0) {
        y = 0;
    }
    else if (y + height >= 1080){
        y = 1080 - height;
    }
}

void Paddle::freeScore(){
    if (renderedScore != nullptr){
        SDL_DestroyTexture(renderedScore);
        renderedScore = nullptr;
    }
}

bool Paddle::renderScore(SDL_Renderer * renderer, TTF_Font * font){
    freeScore();
    char stringScore = score + '0';
    SDL_Surface * scoreSurface = TTF_RenderText_Solid(font, &stringScore, color);
    if (scoreSurface == nullptr){return false;}

    renderedScore = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    if (renderedScore == nullptr){return false;}
    SDL_FreeSurface(scoreSurface);
    return true;
}