#pragma once
#include"enemy.h"
#include"bullet.h"

class BeeBot : public Enemy {
private:
    float zigzagOffset;
    float zigzagAmplitude;
    float zigzagSpeed;
    int movementDirection;
    static const int MAX_BULLETS = 50;
    Bullet* bullets[MAX_BULLETS];
    int currentBulletCount;
    float patrolStartX;
    float patrolEndX;
    float detectionRangeX;
    float detectionRangeY;
    float baseY;
    float shootInterval;

public:
    //
    //constructor
    BeeBot(float startX, float startY, float patrolDistance = 400.0f) :
        zigzagOffset(0),
        zigzagAmplitude(50.0f),
        zigzagSpeed(50.0f),
        movementDirection(1),
        currentBulletCount(0),
        patrolStartX(startX),
        patrolEndX(startX + patrolDistance),
        detectionRangeX(800.0f),
        detectionRangeY(896.0f),
        baseY(startY),
        shootInterval(5.0f) //set to 5 seconds
    {
        x = startX;
        y = startY;
        hp = 5;
        speed = 40.0f;
        isFlying = true;
        shootTimer = 0.0f;
        moveRight = true;
        moveLeft = false;


        for (int i = 0; i < MAX_BULLETS; ++i) {
            bullets[i] = nullptr;
        }

        setupAnimations();
        sprite.setScale(1.8f, 2.0f);
    }

    void setupAnimations() override {
        movingLeft.loadFromFile("Data/enemies/bomber.png");
        movingRight.loadFromFile("Data/enemies/bomberL.png");
        still.loadFromFile("Data/enemies/bomber.png");

        s.setup(&still, 32, 32, 1, 0.08f, true);
        mLeft.setup(&movingLeft, 48, 32, 4, 0.08f, true);
        mRight.setup(&movingRight, 48, 32, 4, 0.08f, true);


        sprite.setTexture(still);
        sprite.setPosition(x, y);
    }

    //update function
    void update(float playerX, float playerY, float deltaTime) override {
        if (!isActive) return;

        //horizontal patrol movement
        if (moveRight) {
            x += speed * deltaTime;
            if (x >= patrolEndX) {
                moveRight = false;
                moveLeft = true;
            }
        }
        else if (moveLeft) {
            x -= speed * deltaTime;
            if (x <= patrolStartX) {
                moveRight = true;
                moveLeft = false;
            }
        }

        //zigzag vertical movement
        zigzagOffset += zigzagSpeed * movementDirection * deltaTime;
        if (zigzagOffset > zigzagAmplitude) {
            zigzagOffset = zigzagAmplitude;
            movementDirection = -1; //change direction
        }
        else if (zigzagOffset < -zigzagAmplitude) {
            zigzagOffset = -zigzagAmplitude;
            movementDirection = 1; //change direction
        }

        y = baseY + zigzagOffset;


        updateAnimation(deltaTime);


        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i] && bullets[i]->getIsActive()) {
                bullets[i]->update(deltaTime);
            }
        }


        bool playerInRangeX = abs(playerX - x) <= detectionRangeX;
        bool playerInRangeY = abs(playerY - y) <= detectionRangeY;


        if (playerInRangeX && playerInRangeY) {
            shootTimer += deltaTime;
            if (shootTimer >= shootInterval) {
                shootTimer = 0;

                float dirX = (playerX + 32) - x;
                float dirY = (playerY + 32) - y;

                float length = sqrt(dirX * dirX + dirY * dirY);
                if (length > 0) {
                    dirX /= length;
                    dirY /= length;

                    shoot(dirX, dirY);
                }
            }
        }

        sprite.setPosition(x, y);
    }

    void shoot(float dirX, float dirY) {
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
            bullets[bulletIndex] = new Bullet(x, y + 32, dirX, dirY);
        }
    }

    void draw(sf::RenderWindow& window, float cameraX, float cameraY) override {
        if (isActive) {
            sprite.setPosition(x - cameraX, y - cameraY);
            window.draw(sprite);


            for (int i = 0; i < currentBulletCount; ++i) {
                if (bullets[i] && bullets[i]->getIsActive()) {
                    bullets[i]->draw(window, cameraX);
                }
            }

            //drawHitBox(window, x - cameraX, y - cameraY, 64, 64);
        }
    }

    void onPlayerCollision() override {
        isActive = false;
    }

    bool checkBulletCollision(float playerX, float playerY, int playerWidth, int playerHeight) override {
        bool collided = false;
        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i] && bullets[i]->getIsActive()) {
                //get bullet positions and dimensions
                float bulletX = bullets[i]->getX();
                float bulletY = bullets[i]->getY();

                //AABB collision detection between bullet and player
                if (playerX + 32 < bulletX + 16 &&
                    playerX + playerWidth > bulletX &&
                    playerY + 32 < bulletY + 16 &&
                    playerY + playerHeight > bulletY) {

                    bullets[i]->deactivate(); //deactivate the bullet when collision occurs
                    collided = true; //mark that a collision happened
                }
            }
        }
        return collided;
    }

    ~BeeBot() {
        for (int i = 0; i < currentBulletCount; ++i) {
            if (bullets[i]) {
                delete bullets[i];
            }
        }
    }
};