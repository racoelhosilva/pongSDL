#ifndef BALL_PONG
#define BALL_PONG

#include <cmath>

class Ball {
public:
    float x, y, width, height;
    float speed;
    float angle;

    void setup();
    void move(float deltaTime, bool *wallHit);

    float xCenter();
    float yCenter();

    void resetLeft();
    void resetRight();
};


#endif