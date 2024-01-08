#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include "ball.h"
#include "paddle.h"
#include "constants.h"


SDL_Window * window = nullptr;
SDL_Renderer * renderer = nullptr;
bool GameIsRunning = false;
int lastFrameTime = 0;

bool lastColisionLeft;

Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;

bool initializeWindow(){
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

    return true;
}

void setup(){
    ball.setup();
    leftPaddle.leftSetup();
    rightPaddle.rightSetup();
    lastColisionLeft = true;
    std::cout << "Current Score: " << leftPaddle.score << " - " << rightPaddle.score << '\n';
}

void processInput(){
    SDL_Event event;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    
    while(SDL_PollEvent(&event)) if(event.type == SDL_QUIT) GameIsRunning=false;
    
    if(keystates[SDL_SCANCODE_ESCAPE]) GameIsRunning=false;
    
    if(keystates[SDL_SCANCODE_W]) 
        leftPaddle.moveUp();
    else if(keystates[SDL_SCANCODE_S])      
        leftPaddle.moveDown();
    else
        leftPaddle.stop();

    if(keystates[SDL_SCANCODE_UP]) 
        rightPaddle.moveUp();
    else if(keystates[SDL_SCANCODE_DOWN])      
        rightPaddle.moveDown();
    else
        rightPaddle.stop();


}

void update(){

    // Better implementation with SDL_Delay
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && (timeToWait > FRAME_TARGET_TIME)){
        SDL_Delay(timeToWait);
    }

    float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0; // in secondsme = (SDL_GetTicks() - lastFrameTime) / 1000.0; // in secondsme = (SDL_GetTicks() - lastFrameTime) / 1000.0; // in seconds

    lastFrameTime = SDL_GetTicks();

    leftPaddle.move(deltaTime);
    rightPaddle.move(deltaTime);
    
    // Collisions
    if (!lastColisionLeft && ball.x < 200){
        bool checkX = (ball.x <= leftPaddle.x + leftPaddle.width) && (ball.x + ball.width >= leftPaddle.x);
        bool checkY = (ball.y + ball.height >= leftPaddle.y) && (ball.y <= leftPaddle.y + leftPaddle.height);
        if (checkX && checkY){
            ball.collision();
            lastColisionLeft = true;
        }
    }
    else if (lastColisionLeft && ball.x > 1720){
        bool checkX = (ball.x <= rightPaddle.x + rightPaddle.width) && (ball.x + ball.width >= rightPaddle.x);
        bool checkY = (ball.y + ball.height >= rightPaddle.y) && (ball.y <= rightPaddle.y + rightPaddle.height);
        if (checkX && checkY){
            ball.collision();
            lastColisionLeft = false;
        }
    }

    bool rightReached = (ball.x + ball.width) >= 1920;
    bool leftReached = ball.x <= 0;
    if (rightReached){
        leftPaddle.score++;
        ball.resetRight();
        std::cout << "Current Score: " << leftPaddle.score << " - " << rightPaddle.score << '\n';
    }
    else if (leftReached){
        rightPaddle.score++;
        ball.resetLeft();
        std::cout << "Current Score: " << leftPaddle.score << " - " << rightPaddle.score << '\n';
    }
    if (rightPaddle.score >= 10 || leftPaddle.score >= 10){
        GameIsRunning = false;
    }

    ball.move(deltaTime);
    
}

void render(){
    SDL_SetRenderDrawColor(renderer, 28, 28, 28, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ballRectangle = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height,
    };
    SDL_Rect leftPaddleRectangle = {
        (int) leftPaddle.x,
        (int) leftPaddle.y,
        (int) leftPaddle.width,
        (int) leftPaddle.height
    };
    SDL_Rect rightPaddleRectangle = {
        (int) rightPaddle.x,
        (int) rightPaddle.y,
        (int) rightPaddle.width,
        (int) rightPaddle.height
    };

    SDL_SetRenderDrawColor(renderer, 222, 222, 222, 255);
    SDL_RenderFillRect(renderer, &ballRectangle);
    SDL_RenderFillRect(renderer, &leftPaddleRectangle);
    SDL_RenderFillRect(renderer, &rightPaddleRectangle);

    SDL_RenderPresent(renderer);

    return;
}

void destroyWindow(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(){
    GameIsRunning = initializeWindow();

    setup();

    while(GameIsRunning){
        processInput();
        update();
        render();
    }

    destroyWindow();

    return 0; 
}