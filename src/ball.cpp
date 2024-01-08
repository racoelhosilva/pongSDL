#include "ball.h"
#include <cmath>

void Ball::setup(){
    x = 940;
    y = 520;
    width = 40;
    height = 40;
    speed = 850;
    angle = M_PI_4;

}

void Ball::move(float deltaTime){
    x += speed * cos(angle) * deltaTime;
    y += speed * sin(angle) * deltaTime;

    bool bottomReached = (y + height) >= 1080;
    bool topReached = y <= 0;
    if (bottomReached && (angle < M_PI)){
        angle = (2 * M_PI - angle); 
    }
    else if (topReached && (angle > M_PI)){
        angle = (2 * M_PI - angle);
    }
}

float Ball::xCenter(){
    return x + width / 2;
}
float Ball::yCenter(){
    return y + height / 2;
}

void Ball::collision(){
    if (angle < M_PI_2 || (angle > M_PI && angle < 3 * M_PI_2)){
        angle += M_PI_2;
    }
    else {
        angle += 3 * M_PI_2;
    }
    if (angle >= 2 * M_PI){
        angle -= 2 * M_PI;
    } 
    speed += 50;
}

void Ball::resetRight(){
    x = 300; y = 520;
    speed = 850;
}
void Ball::resetLeft(){
    x = 1620; y = 520;
    speed = 850;
}