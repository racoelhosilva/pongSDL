#include "paddle.h"

void Paddle::leftSetup(){
    x = 80; y = 500;
    width = 20; height = 200;
    speed = 0;
    score = 0;
}

void Paddle::rightSetup(){
    x = 1800; y = 500;
    width = 20; height = 200;
    speed = 0;
    score = 0;
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