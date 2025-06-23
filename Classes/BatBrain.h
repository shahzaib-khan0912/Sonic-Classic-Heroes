#pragma once
#include"Enemy.h"

class BatBrain : public Enemy {
public:
    //constructor
    BatBrain(float startX, float startY) :Enemy() {
        x = startX;
        y = startY;
        hp = 3;
        speed = 2.0f;
        isFlying = true;
        isActive = true;
        moveRight = false;
        moveLeft = false;
        currentState = 0;

        setupAnimations();

        sprite.setPosition(x, y);
        sprite.setScale(2.0f, 2.0f);
    }

    void setupAnimations() override {

        still.loadFromFile("Data/enemies/batBrain.png");
        movingLeft.loadFromFile("Data/enemies/batBrain.png");
        movingRight.loadFromFile("Data/enemies/batBrainL.png");


        s.setup(&still, 34, 33, 1, 0.12f, true);
        mLeft.setup(&movingLeft, 33.2, 34, 7, 0.06f, true);
        mRight.setup(&movingRight, 33.2, 34, 7, 0.06f, true);


        s.applyToSprite(sprite);
    }


    void update(float playerX, float playerY, float deltaTime) override {
        if (!isActive) {
            return;
        }

        float dx = playerX - x;
        float dy = playerY - y;
        float distance = sqrt(dx * dx + dy * dy);


        if (dx > 0) {
            moveRight = true;
            moveLeft = false;
        }
        else if (dx < 0) {
            moveRight = false;
            moveLeft = true;
        }

        if (distance > 0) {
            dx /= distance;
            dy /= distance;
            x += dx * speed;
            y += dy * speed;
        }


        updateAnimation(deltaTime);


        sprite.setPosition(x, y);
    }


    void draw(sf::RenderWindow& window, float cameraX, float cameraY) override {
        if (isActive) {
            sprite.setPosition(x - cameraX, y);
            window.draw(sprite);
        }
        //drawHitBox(window, x - cameraX, y - cameraY, 64, 64);
    }

    void onPlayerCollision() override {
        //onBlast.applyToSprite(sprite);
        isActive = false;
    }
};