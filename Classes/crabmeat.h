#pragma once
#include"Enemy.h"
#include"bullet.h"

class CrabMeat : public Enemy {
private:
    float movementRange;
    float startX;
    static const int MAX_BULLETS = 50;
    Bullet* bullets[MAX_BULLETS];
    int currentBulletCount;
    bool reachedEnd;      //flag to track if crab just reached end of movement

public:
    CrabMeat(float startX, float startY) :Enemy(),
        movementRange(200.0f),
        startX(startX),
        currentBulletCount(0),
        reachedEnd(false)
    {
        x = startX;
        y = startY;
        hp = 4;

        speed = 1.0f;
        isFlying = false;
        moveRight = true;
        moveLeft = false;
        currentState = 0;

        for (int i = 0; i < MAX_BULLETS; ++i) {
            bullets[i] = nullptr;
        }

        setupAnimations();
        sprite.setScale(1.5f, 2.0f);
    }

    void setupAnimations() override {

        still.loadFromFile("Data/enemies/crabmeat.png");
        movingLeft.loadFromFile("Data/enemies/crabMeatLeft.png");
        movingRight.loadFromFile("Data/enemies/crabMeatRight.png");


        s.setup(&still, 49, 31, 3, 0.08f, true);
        mLeft.setup(&movingLeft, 44.5, 31, 3, 0.08f, true);
        mRight.setup(&movingRight, 44.5, 30, 4, 0.08f, true);


        s.applyToSprite(sprite);


        sprite.setPosition(x, y);
        sprite.setScale(1.0f, 1.0f);
    }

    void update(float playerX, float playerY, float deltaTime) override {
        if (!isActive) return;

        reachedEnd = false;


        if (moveRight) {
            x += speed;
            if (x > startX + movementRange) {
                moveRight = false;
                moveLeft = true;
                reachedEnd = true;
                shootProjectile(1.0f);
            }
        }
        else {
            x -= speed;
            if (x < startX) {
                moveRight = true;
                moveLeft = false;
                reachedEnd = true;
                shootProjectile(-1.0f);
            }
        }


        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i] && bullets[i]->getIsActive()) {
                bullets[i]->update(deltaTime);
            }
        }


        updateAnimation(deltaTime);


        sprite.setPosition(x, y);
    }


    void shootProjectile(float dirX) {

        int bulletIndex = -1;


        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i] && !bullets[i]->getIsActive()) {
                delete bullets[i];
                bulletIndex = i;
                break;
            }
        }


        if (bulletIndex == -1 && currentBulletCount < MAX_BULLETS) {
            bulletIndex = currentBulletCount++;
        }


        if (bulletIndex != -1) {

            float dirY = -1.2f;
            bullets[bulletIndex] = new Bullet(x + 32, y + 32, dirX * 2.0f, dirY);

        }
    }

    void draw(sf::RenderWindow& window, float cameraX, float cameraY) override {
        if (isActive) {
            sprite.setPosition(x - cameraX, y - cameraY);
            window.draw(sprite);

            //draw bullets
            for (int i = 0; i < currentBulletCount; ++i) {
                if (bullets[i] && bullets[i]->getIsActive()) {
                    bullets[i]->draw(window, cameraX);
                }
            }
        }

        //drawHitBox(window, x - cameraX, y - cameraY, 64, 64);
    }

    void onPlayerCollision() override {
        isActive = false;
    }


    bool checkBulletCollision(float playerX, float playerY, int playerWidth, int playerHeight) override {
        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i] && bullets[i]->getIsActive()) {




                if (playerX + 32 < bullets[i]->getX() + 16 &&
                    playerX + playerWidth > bullets[i]->getX() &&
                    playerY + 32 < bullets[i]->getY() + 16 &&
                    playerY + playerHeight > bullets[i]->getY()) {

                    bullets[i]->deactivate();
                    return true;
                }
            }
        }
        return false;
    }

    ~CrabMeat() {
        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i]) {
                delete bullets[i];
            }
        }
    }
};