#ifndef PADDLE_PONG
#define PADDLE_PONG

class Paddle {
public:
    float x, y, width, height;
    float speed;
    bool shouldMove;
    int score;

    void leftSetup();
    void rightSetup();
    void moveUp();
    void moveDown();
    void move(float deltaTime);
    void stop();
};

#endif