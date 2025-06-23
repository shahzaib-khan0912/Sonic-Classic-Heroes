#pragma once
#include <SFML/Graphics.hpp>
#include "camera.h"
#include "animation.h"
#include <iostream>


class Collectible {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float x, y;
    bool isActive;
    int type; 

    
    Animation animation;
    bool hasAnimation;
    

public:
    Collectible(float posX, float posY, int collectibleType) {
        x = posX;
        y = posY;
        isActive = true;
        type = collectibleType;
        hasAnimation = false;
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
    }

    float getX() const { return x; }
    float getY() const { return y; }
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }
    int getType() const { return type; }

    virtual ~Collectible() {}
};


class Ring : public Collectible {
private:
    float rotationAngle;

public:
    Ring(float posX, float posY) : Collectible(posX, posY, 1) {
        
        if (texture.loadFromFile("Data/collectibles/ring.png")) {
            
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
        Collectible::onCollect();
        
    }
};


class ExtraLife : public Collectible {
private:
    float floatTimer;
    float baseY;

public:
    ExtraLife(float posX, float posY) : Collectible(posX, posY, 2) {
        
        if (texture.loadFromFile("Data/collectibles/heart.png")) {
            
            sprite.setTexture(texture);
            sprite.setScale(1.0f, 1.0f);

            
            animation.setup(&texture, 64, 64, 5, 0.09f, true);
            animation.play();

            
            hasAnimation = true;
        }

        baseY = posY;
        floatTimer = 0.0f;
    }

    void update(float deltaTime) override {
        
        Collectible::update(deltaTime);

        floatTimer += deltaTime;
        y = baseY + sin(floatTimer * 2.0f) * 5.0f; 
    }

    void onCollect() override {
        Collectible::onCollect();
        
    }
};


class SpecialBoost : public Collectible {
private:
    float pulseTimer;
    float baseScale;

public:
    SpecialBoost(float posX, float posY) : Collectible(posX, posY, 3) {

        
        
        if (texture.loadFromFile("Data/collectibles/inv.png")) {
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
        
    }
};