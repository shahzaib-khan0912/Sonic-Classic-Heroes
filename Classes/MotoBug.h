#pragma once
#include"Enemy.h"

class Motobug : public Enemy {
public:

    Motobug(float startX, float startY) :Enemy() {
        x = startX;
        y = startY;
        hp = 2;
        speed = 1.0f;
        isFlying = false;
        frames = 5;
        currentFrame = 0;
        animationTime = 0.0f;
        setupAnimations();
        sprite.setScale(2.0f, 2.0f);
    }

    void setupAnimations() override {
        movingLeft.loadFromFile("Data/enemies/motoBugR.png");
        movingRight.loadFromFile("Data/enemies/motoBugL.png");
        still.loadFromFile("Data/enemies/motoBugS.png");

        s.setup(&still, 36, 30, 1, 0.12f, true);
        mLeft.setup(&movingLeft, 40, 29, 4, 0.09f, true);
        mRight.setup(&movingRight, 40, 29, 4, 0.09f, true);

        sprite.setTexture(still);
        sprite.setPosition(x, y);
    }

    void update(float playerX, float playerY, float deltaTime) override {
        if (!isActive) return;

        float dx = playerX - x;
        float distance = std::abs(dx);

        moveRight = dx > 0;
        moveLeft = dx < 0;

        if (distance > 0) {
            dx /= distance;
            x += dx * speed;
        }

        updateAnimation(deltaTime);

        sprite.setPosition(x, y);
    }

    void draw(sf::RenderWindow& window, float cameraX, float cameraY) override {
        if (isActive) {
            sprite.setPosition(x - cameraX, y);
            window.draw(sprite);
        }
        //drawhitbox(window, x - cameraX, y - cameraY, 64, 64);
    }

    void onPlayerCollision() override {
        isActive = false;
    }
};
