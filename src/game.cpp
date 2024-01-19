#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "ball.h"
#include "paddle.h"
#include "constants.h"


SDL_Window * window = nullptr;
SDL_Renderer * renderer = nullptr;
TTF_Font * font = nullptr;
Mix_Chunk * paddleSound = nullptr;
Mix_Chunk * wallSound = nullptr;
Mix_Chunk * scoreSound = nullptr;

bool GameIsRunning = false;
int lastFrameTime = 0;

bool lastColisionLeft;

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;

bool initialize(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "Error launching SDL2!\n";
        return false;
    }
    
    window = SDL_CreateWindow(nullptr, 
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT,
                        SDL_WINDOW_BORDERLESS);
    if (!window){
        std::cerr << "Error creating SDL2 Window!\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){
        std::cerr << "Error creating SDL2 Renderer\n";
        return false;
    }

    if (TTF_Init() != 0){
        std::cerr << "Error loading SDL TTF\n";
        return false;
    }
    font = TTF_OpenFont("font/alterebro.ttf", 60);
    if (font == nullptr){
        std::cerr << "Error opening font alterebro.ttf\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0){
        std::cerr << "Error loading SDL Mixer\n";
        return false;
    }
    paddleSound = Mix_LoadWAV("audio/paddle.wav");
    wallSound = Mix_LoadWAV("audio/wall.wav");
    scoreSound = Mix_LoadWAV("audio/score.wav");
    if (paddleSound == nullptr || wallSound == nullptr || scoreSound == nullptr){
        std::cerr << "Error opening audio files\n";
        return false;
    }

    return true;
}

void setup(){
    ball.setup();
    leftPaddle.color = LEFT_PADDLE_COLOR;
    leftPaddle.leftSetup(renderer, font);
    rightPaddle.color = RIGHT_PADDLE_COLOR;
    rightPaddle.rightSetup(renderer, font);
    lastColisionLeft = true;
}

void processInput(){
    SDL_Event event;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            GameIsRunning=false;
        }
    }
    
    if(keystates[SDL_SCANCODE_ESCAPE]) {
        GameIsRunning=false;
    }

    if(keystates[SDL_SCANCODE_W]) {
        leftPaddle.moveUp();
    }
    else if(keystates[SDL_SCANCODE_S]) {
        leftPaddle.moveDown();

    }      
    else {
        leftPaddle.stop();
    }
}

void update(){

    /* Delay before next frame */
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && (timeToWait > FRAME_TARGET_TIME)){
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0; // in seconds
    lastFrameTime = SDL_GetTicks();

    leftPaddle.move(deltaTime);
    
    /* Implementation of the CPU paddle */
    int CPUInfoBounds = (600 + (leftPaddle.score - rightPaddle.score) * 80) * difficultyMultiplier;
    bool beforeHitInformCPU = (lastColisionLeft && ball.x > CPUInfoBounds);
    bool afterHitInformCPU = (!lastColisionLeft && ball.x < CPUInfoBounds);
    
    if (beforeHitInformCPU){
        if (ball.yCenter() > (rightPaddle.y + rightPaddle.height / 3)){
            rightPaddle.moveDown();
        }
        else if (ball.yCenter() < (rightPaddle.y + rightPaddle.height / 3)){
            rightPaddle.moveUp();
        }
    }
    else if (afterHitInformCPU){
        int rightPaddleCenter = rightPaddle.y + rightPaddle.height / 2;
        if ((rightPaddleCenter) > 800){
            rightPaddle.moveUp();
        }
        else if ((rightPaddleCenter) < 300){
            rightPaddle.moveDown();
        }
        else if (rightPaddleCenter > 400 && rightPaddleCenter < 600){
            rightPaddle.stop();
        }
    }
    else {
        rightPaddle.stop();
    }

    rightPaddle.move(deltaTime);
    
    /* Ball Movement */
    if (!lastColisionLeft && ball.x < 200){
        bool checkX = (ball.x <= leftPaddle.x + leftPaddle.width) && (ball.x + ball.width >= leftPaddle.x);
        bool checkY = (ball.y + ball.height >= leftPaddle.y) && (ball.y <= leftPaddle.y + leftPaddle.height);
        if (checkX && checkY){
            double in = (leftPaddle.y+(leftPaddle.height/2))-(ball.y+(ball.height/2));
            double nor = in/(leftPaddle.height/2);
            ball.angle = (2 * M_PI) - (nor * (M_PI / 3));

            Mix_PlayChannel(-1, paddleSound, 0);

            lastColisionLeft = true;
            ball.speed += 100;
        }
    }
    else if (lastColisionLeft && ball.x > 1720){
        bool checkX = (ball.x <= rightPaddle.x + rightPaddle.width) && (ball.x + ball.width >= rightPaddle.x);
        bool checkY = (ball.y + ball.height >= rightPaddle.y) && (ball.y <= rightPaddle.y + rightPaddle.height);
        if (checkX && checkY){
            double in = (rightPaddle.y+(rightPaddle.height/2))-(ball.y+(ball.height/2));
            double nor = in/(rightPaddle.height/2);
            ball.angle = (nor * (M_PI / 3)) + M_PI;
            
            Mix_PlayChannel(-1, paddleSound, 0);


            lastColisionLeft = false;
            ball.speed += 100;
        }
    }
    if (ball.angle >= 2 * M_PI){
        ball.angle -= 2 * M_PI;
    }
    else if (ball.angle < 0){
        ball.angle += 2 * M_PI;
    }

    /* Goal Checker */
    bool rightReached = (ball.x + ball.width) >= 1920;
    bool leftReached = ball.x <= 0;
    if (rightReached){
        Mix_PlayChannel(-1, scoreSound, 0);
        leftPaddle.score++;
        leftPaddle.renderScore(renderer, font);
        ball.resetRight();
    }
    else if (leftReached){
        Mix_PlayChannel(-1, scoreSound, 0);
        rightPaddle.score++;
        rightPaddle.renderScore(renderer, font);
        ball.resetLeft();
    }

    /* Wall Hit Checker */
    bool wallHit = false;
    ball.move(deltaTime, &wallHit);
    if (wallHit){
        Mix_PlayChannel(-1, wallSound, 0);
    }

    /* End Game Checker */
    if (rightPaddle.score + leftPaddle.score >= maxScore){
        GameIsRunning = false;
    }
}

void render(){

    /* Draw Background */
    SDL_SetRenderDrawColor(renderer, 28, 28, 28, 255);
    SDL_RenderClear(renderer);

    /* Field Decorations */
    SDL_SetRenderDrawColor(renderer, 222, 222, 222, 128);
    for (int yVal = 10; yVal <= WINDOW_HEIGHT - 20; yVal += 40){
        SDL_Rect square = {WINDOW_WIDTH / 2 - 5, yVal, 10, 20};
        SDL_RenderFillRect(renderer, &square);
    }

    /* Draw Scores */
    SDL_Rect leftScorePlace = {WINDOW_WIDTH / 2 - 180, 50, 100,200};
    SDL_RenderCopy(renderer, leftPaddle.renderedScore, nullptr, &leftScorePlace);
    SDL_Rect rightScorePlace = {WINDOW_WIDTH / 2 + 100, 50, 100,200};
    SDL_RenderCopy(renderer, rightPaddle.renderedScore, nullptr, &rightScorePlace);

    /* Draw Ball */
    SDL_Rect ballRectangle = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height,
    };
    SDL_SetRenderDrawColor(renderer, 222, 222, 222, 255);
    SDL_RenderFillRect(renderer, &ballRectangle);

    /* Draw Left Paddle */
    SDL_Rect leftPaddleRectangle = {
        (int) leftPaddle.x,
        (int) leftPaddle.y,
        (int) leftPaddle.width,
        (int) leftPaddle.height
    };
    SDL_SetRenderDrawColor(renderer, leftPaddle.color.r, leftPaddle.color.g, leftPaddle.color.b, leftPaddle.color.a);
    SDL_RenderFillRect(renderer, &leftPaddleRectangle);

    /* Draw Right Paddle*/
    SDL_Rect rightPaddleRectangle = {
        (int) rightPaddle.x,
        (int) rightPaddle.y,
        (int) rightPaddle.width,
        (int) rightPaddle.height
    };
    SDL_SetRenderDrawColor(renderer, rightPaddle.color.r, rightPaddle.color.g,rightPaddle.color.b, rightPaddle.color.a);
    SDL_RenderFillRect(renderer, &rightPaddleRectangle);

    SDL_RenderPresent(renderer);
}

void destroyWindow(){
    leftPaddle.freeScore();
    rightPaddle.freeScore();
    Mix_FreeChunk(paddleSound);
    Mix_FreeChunk(wallSound);
    Mix_FreeChunk(scoreSound);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    Mix_Quit();
    SDL_Quit();
}

int main(){
    GameIsRunning = initialize();

    setup();

    while(GameIsRunning){
        processInput();
        update();
        render();
    }

    destroyWindow();

    return 0; 
}