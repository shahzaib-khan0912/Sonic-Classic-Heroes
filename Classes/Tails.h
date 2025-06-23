#pragma once
#include "character.h"
#include <SFML/Window/Keyboard.hpp>

class Tails : virtual public Character {
protected:
    bool isFlying;
    float maxFlightTime;
    float currentFlightTime;
    Animation flyAnimation;
    Texture isFlyingTexture;

    Animation flyAnimationL;
    Texture isFlyingTextureL;

    Character* sonicTarget;
    Character* knucklesTarget;
    bool sonicAttached;
    bool knucklesAttached;
    float sonicOffsetX;
    float knucklesOffsetX;

    Music flying;

public:
    Tails() : Character()
        , isFlying(false)
        , maxFlightTime(7.0f)
        , currentFlightTime(0.0f)
        , sonicTarget(nullptr)
        , knucklesTarget(nullptr)
        , sonicAttached(false)
        , knucklesAttached(false)
        , sonicOffsetX(0.0f)
        , knucklesOffsetX(0.0f)
    {
        setupAnimations();
        sprite.setScale(2.0f, 2.90f);
        sprite.setOrigin(0, 17);
        maxSpeed = 11.0f;
        id = 3;
        flying.openFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Flying.wav");
    }

    //set sonic and knuckles as potential riders
    void setRiderTargets(Character* sonic, Character* knuckles) {
        sonicTarget = sonic;
        knucklesTarget = knuckles;
    }

    bool fullyUnder(Character* c) {
        //tails’s hit-box top-left and dimensions
        float tx = x;
        float ty = y;
        const float tw = 64.0f;
        const float th = 64.0f;

        //rider’s top-left
        float cx = c->getX();
        float cy = c->getY();
        const float cw = 64.0f;
        const float ch = 64.0f;

        //1) horizontal: center-to-center delta
        float tailsCenterX = tx + tw / 2.0f;
        float riderCenterX = cx + cw / 2.0f;
        float dx = riderCenterX - tailsCenterX;
        bool withinX = (dx >= -128.0f && dx <= +128.0f);

        //2) vertical: how far below tails’s bottom is the rider’s top?
        float tailBottom = ty + th;
        float dy = cy - tailBottom;
        //allow them to be up to 15px below, but not above
        bool withinY = (dy >= 64.0f && dy <= 78.0f);

        return withinX && withinY;
    }

    void setupAnimations() override {
        still.loadFromFile("Data/tails/show.png");
        runRight.loadFromFile("Data/tails/tailJog.png");
        runLeft.loadFromFile("Data/tails/tailJogL.png");
        sprintRight.loadFromFile("Data/tails/tailSprint.png");
        sprintLeft.loadFromFile("Data/tails/tailSprintL.png");
        Jump.loadFromFile("Data/tails/tailJump.png");
        collideWallRight.loadFromFile("Data/tails/tailPush.png");
        collideWallLeft.loadFromFile("Data/tails/tailPushL.png");
        isFlyingTexture.loadFromFile("Data/tails/tailsFly.png");
        isFlyingTextureL.loadFromFile("Data/tails/tailsFlyL.png");
        sprintDash.loadFromFile("Data/tails/sprintDash.png");
        glide.loadFromFile("Data/tails/tailsFly.png");

        Still.setup(&still, 49, 40, 17, 0.09f, true);
        runR.setup(&runRight, 49, 40, 10, 0.08f, true);
        runL.setup(&runLeft, 49, 40, 10, 0.08f, true);
        sprintR.setup(&sprintRight, 48, 40, 6, 0.08f, true);
        sprintL.setup(&sprintLeft, 47.8f, 40, 6, 0.08f, true);
        j.setup(&Jump, 49, 40, 6, 0.08f, true);
        collideWallR.setup(&collideWallRight, 49, 40, 9, 0.08f, true);
        collideWallL.setup(&collideWallLeft, 49, 40, 9, 0.08f, true);

        /*flyAnimation.setup(&isFlyingTexture, 34.5, 44, 6, 0.08f, true);
        flyAnimationL.setup(&isFlyingTextureL, 34, 44, 6, 0.08f, true);*/
        flyAnimation.setup(&isFlyingTexture, 48, 48, 4, 0.008f, true);
        flyAnimationL.setup(&isFlyingTextureL, 48, 48, 4, 0.008f, true);
        sprintDashing.setup(&sprintDash, 49, 38, 16, 0.08f, true);
        sprintDashingL.setup(&sprintDash, 49, 38, 16, 0.08f, true);
        gliding.setup(&isFlyingTexture, 48, 48, 4, 0.008f, true);
        glidingL.setup(&isFlyingTextureL, 48, 48, 4, 0.008f, true);
        Still.applyToSprite(sprite);
    }

    void jump() override {
        velocityY = -20.0f;
        isOnGround = false;
        isJumping = true;
        isFlying = false;
        currentFlightTime = 0.0f;
        sonicAttached = false;
        knucklesAttached = false;
    }

    void update(float deltaTime, Level* currentLevel, Camera* camera) override {
        //flight activation
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !isOnGround && currentFlightTime < maxFlightTime && isActive) {
            isFlying = true;
            flying.play();
            velocityY = -8.0f;
            currentFlightTime += deltaTime;
        }
        else {
            isFlying = false;
            flying.stop();
        }
        if (currentFlightTime >= maxFlightTime) {
            isFlying = false;
        }

        //base movement/physics
        Character::update(deltaTime, currentLevel, camera);

        if (isFlying && !movingLeft && !movingRight) {
            flyAnimation.update(deltaTime);
            flyAnimation.applyToSprite(sprite);
        }
        //flying animation
        else if (isFlying && movingRight) {
            flyAnimation.update(deltaTime);
            flyAnimation.applyToSprite(sprite);
        }
        else if (isFlying && movingLeft) {
            flyAnimationL.update(deltaTime);
            flyAnimationL.applyToSprite(sprite);
        }

        //determine tails's sprite height
        float th = 78.0f;

        //sonic attachment logic
        if (isFlying && sonicTarget && !sonicAttached && fullyUnder(sonicTarget)) {
            sonicAttached = true;
            sonicOffsetX = sonicTarget->getX() - x;
        }
        if (sonicAttached) {
            sonicTarget->setIsHanging(true);
            sonicTarget->setX(x);
            sonicTarget->setY(y + th);
            sonicTarget->setVelocityX(velocityX);
            sonicTarget->setVelocityY(0.0f);
            if (currentLevel->checkCollisionWithWalls(
                sonicTarget->getX(), sonicTarget->getY(), 64, 64,id)) {
                sonicAttached = false;
            }
        }
        else
            sonicTarget->setIsHanging(false);

        //knuckles attachment logic (stacked under sonic if present)
        if (isFlying && knucklesTarget && !knucklesAttached && fullyUnder(knucklesTarget)) {
            knucklesAttached = true;
            knucklesOffsetX = knucklesTarget->getX() - x;
        }
        if (knucklesAttached) {
            knucklesTarget->setIsHanging(true);
            float baseY = sonicAttached ? (sonicTarget->getY() + th) : (y + th);
            knucklesTarget->setX(x);
            knucklesTarget->setY(baseY);
            knucklesTarget->setVelocityX(velocityX);
            knucklesTarget->setVelocityY(0.0f);
            if (currentLevel->checkCollisionWithWalls(
                knucklesTarget->getX(), knucklesTarget->getY(), 64, 64,id)) {
                knucklesAttached = false;
            }
        }
        else
            knucklesTarget->setIsHanging(false);

        //detach all if flight ends
        if (!isFlying) {
            sonicAttached = false;
            knucklesAttached = false;
        }
    }

    bool isJustJumping() const override { return justJumped; }

    void applySpecialBoost() override {
        hasBoost = true;
        boostTimer = 15.0f;
        maxFlightTime = 11.0f;
    }

    void resetBoostEffects() override {
        maxFlightTime = 7.0f;
    }

    void reset() {
        x = 200;
        y = 200;
    }
};
