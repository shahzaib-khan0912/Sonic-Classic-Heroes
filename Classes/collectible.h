#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "animation.h"

class Collectible {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y;
    bool isActive;
    int type;
    Animation animation;
    bool hasAnimation;
    static int score; //static score variable shared across all collectibles

public:
    Collectible(float posX, float posY, int collectibleType) {
        x = posX;
        y = posY;
        isActive = true;
        type = collectibleType;
        hasAnimation = false;
    }

    //static method to get score
    static int getScore() {
        return score;
    }

    //static method to reset score
    static void resetScore() {
        score = 0;
    }

    virtual void update(float deltaTime) {
        if (hasAnimation) {
            animation.update(deltaTime);
            animation.applyToSprite(sprite);
        }
    }

    virtual void draw(sf::RenderWindow& window, float cameraX, float cameraY) {
        if (isActive) {
            sprite.setPosition(x - cameraX, y - cameraY);
            window.draw(sprite);
        }
    }

    bool checkCollision(float playerX, float playerY, float playerWidth, float playerHeight) {
        if (!isActive) return false;

        float collectibleWidth = 64;
        float collectibleHeight = 64;

        return (playerX < x + collectibleWidth &&
            playerX + playerWidth > x &&
            playerY < y + collectibleHeight &&
            playerY + playerHeight > y);
    }

    virtual void onCollect() {
        isActive = false;
        //increase score by 10 if it's a ring
        if (type == 1) {
            score += 10; //increase score by 10 for rings
            //debug output
            std::cout << "Ring collected! Score: " << score << std::endl;
        }
    }

    float getX() const { return x; }
    float getY() const { return y; }
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }
    int getType() const { return type; }

    virtual ~Collectible() {}
};

//initialize static score variable
int Collectible::score = 0;

class Ring : public Collectible {
private:
    float rotationAngle;

public:
    Ring(float posX, float posY) : Collectible(posX, posY, 1) {
        if (texture.loadFromFile("Data/collectible/ring.png")) {
            sprite.setTexture(texture);
            sprite.setScale(3.0f, 3.0f);
            animation.setup(&texture, 16, 16, 4, 0.1f, true);
            animation.play();
            hasAnimation = true;
        }
        rotationAngle = 0.0f;
    }

    void update(float deltaTime) override {
        Collectible::update(deltaTime);
        if (!hasAnimation) {
            rotationAngle += 180.0f * deltaTime;
            sprite.setRotation(rotationAngle);
        }
    }

    void onCollect() override {
        Collectible::onCollect(); //call base class method to handle score
    }
};

class ExtraLife : public Collectible {
private:
    float floatTimer;
    float baseY;

public:

    
    ExtraLife(float posX, float posY) : Collectible(posX, posY, 2) {
        if (texture.loadFromFile("Data/collectible/heart.png")) {
            sprite.setTexture(texture);
            sprite.setScale(1.0f, 1.0f);
            animation.setup(&texture, 64, 64, 5, 0.1f, true);
            animation.play();
            hasAnimation = true;
        }
        baseY = posY;
        floatTimer = 0.0f;
    }

    void update(float deltaTime) override {
        Collectible::update(deltaTime);
        floatTimer += deltaTime;
        y = baseY + sin(floatTimer * 1.5f) * 5.0f; //floating effect
    }

    void onCollect() override {
        Collectible::onCollect();
        //health increase is handled in Character::checkCollectibles
    }
};

class SpecialBoost : public Collectible {
private:
    float pulseTimer;
    float baseScale;

public:
    SpecialBoost(float posX, float posY) : Collectible(posX, posY, 3) {
        if (texture.loadFromFile("Data/collectible/inv.png")) {
            sprite.setTexture(texture);
            sprite.setScale(2.0f, 2.0f);
            hasAnimation = true;
            animation.setup(&texture, 32, 32, 4, 0.3f, true);
            animation.play();
        }
        baseScale = 0.7f;
        pulseTimer = 0.0f;
    }

    void update(float deltaTime) override {
        Collectible::update(deltaTime);
        if (!hasAnimation) {
            pulseTimer += deltaTime;
            float scale = baseScale + sin(pulseTimer * 3.0f) * 0.1f;
            sprite.setScale(scale, scale);
        }
    }

    void onCollect() override {
        Collectible::onCollect();
        //special boost effect is handled in Character::checkCollectibles
    }
};