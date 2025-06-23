#pragma once
#include "sfml/graphics.hpp"
#include "sfml/audio.hpp"
#include "animation.h"
#include <iostream>
using namespace std;
using namespace sf;

class Enemy {
protected:
    //attributes
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Texture still;
    sf::Texture movingLeft;
    sf::Texture movingRight;
    sf::Texture blast;
    Animation mLeft;
    Animation mRight;
    Animation s;
    Animation onBlast;
    int hp;
    float speed;
    float x, y;
    bool isActive;
    bool isFlying;
    float shootTimer;
    const float shootInterval = 3.0f; //shoot after every 3 sec
    float animationTime;
    int frames;
    int currentFrame;
    bool moveRight;
    bool moveLeft;
    int currentState;
    int width;
    int height;

    // NEW: Blast animation state variables
    bool isBlasting;
    float blastTimer;
    const float blastDuration = 1.0f; // How long the blast animation plays

public:
    //constructor 
    Enemy() : hp(0), speed(0), x(0), y(0), isActive(true), isFlying(false), shootTimer(0) {
        animationTime = 0.0f;
        currentFrame = 0;
        frames = 0;
        currentState = 0;
        width = 64;
        height = 64;
        isBlasting = false;
        blastTimer = 0.0f;
    }

    virtual ~Enemy() {}    //destructor

    virtual void setupAnimations() {
        blast.loadFromFile("Data/blast.png");
        onBlast.setup(&blast, 32, 32, 6, false);
        //override in derived classes to load specific textures
    }

    virtual void updateAnimation(float deltaTime) {
        // Handle blast animation first (highest priority)
        if (isBlasting) {
            onBlast.update(deltaTime);
            onBlast.applyToSprite(sprite);
            blastTimer -= deltaTime;

            // Check if blast animation should end
            if (blastTimer <= 0.0f) {
                isBlasting = false;
                // Optionally deactivate enemy after blast
                isActive = false;
            }
            return; // Don't update other animations while blasting
        }

        // Normal animation logic (only when not blasting)
        int newState = currentState;
        if (moveRight) {
            newState = 1;
        }
        else if (moveLeft)
            newState = 2;
        else {
            newState = 0;
        }

        if (newState != currentState) {
            currentState = newState;
        }

        if (currentState == 2) {
            mLeft.update(deltaTime);
            mLeft.applyToSprite(sprite);
        }
        else if (currentState == 1) {
            mRight.update(deltaTime);
            mRight.applyToSprite(sprite);
        }
        else if (currentState == 0) {
            s.update(deltaTime);
            s.applyToSprite(sprite);
        }
    }

    //virtual functions for each enemy type
    virtual void update(float playerX, float playerY, float deltaTime) {
        updateAnimation(deltaTime);
    }

    virtual void draw(sf::RenderWindow& window, float cameraX, float cameraY) = 0;

    //virtual method for bullet collision detection
    virtual bool checkBulletCollision(float playerX, float playerY, int playerWidth, int playerHeight) {
        return false; //base implementation returns false
    }

    //take damage 
    virtual void takeDamage(int amount) {
        hp -= amount;       //decrease in hp 
        if (hp <= 0) {      //if hp is 0
            triggerBlastAnimation(); // Start blast animation
        }
    }

    // NEW: Method to trigger blast animation
    void triggerBlastAnimation() {
        isBlasting = true;
        blastTimer = blastDuration;
        onBlast.reset(); // Reset animation to start from beginning
    }

    //getter functions
    bool getIsActive() const {
        return isActive;
    }

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    bool getIsBlasting() const {
        return isBlasting;
    }

    virtual void onPlayerCollision() = 0; //collision function for all the kinds of enemy

    void setX(float newX) {
        this->x = this->x - newX;
    }
};