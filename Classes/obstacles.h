#pragma once

#include <SFML/Graphics.hpp>
#include <string>
// Forward declaration to avoid include cycles
//class AnimatedCharacter;

class Obstacles {
protected:
    float x, y;
    sf::Texture texture;
    sf::Sprite sprite;
    bool isActive;
public:
    Obstacles(float posX, float posY)
        : x(posX), y(posY), isActive(true) {
        sprite.setPosition(x, y);
    }
    virtual ~Obstacles() {}

    // Load and apply a texture for this obstacle
    void loadTexture(const std::string& path) {
        if (texture.loadFromFile(path)) {
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
        }
    }

    void setPosition(float xPos, float yPos) {
        x = xPos;
        y = yPos;
        sprite.setPosition(x, y);
    }

    // Default AABB collision for next frame
    virtual bool checkCollision(AnimatedCharacter* character) const {
        if (!isActive) return false;
        float nextX = character->getX() + character->getVelocityX();
        float nextY = character->getY() + character->getVelocityY();
        return (nextX + 64 > x && nextX < x + 64 &&
            nextY + 64 > y && nextY < y + 64);
    }

    // Per‐side collision tests
    bool willCollideFromTop(AnimatedCharacter* character) const {
        if (!isActive) return false;
        float nextX = character->getX() + character->getVelocityX();
        float nextY = character->getY() + character->getVelocityY();
        return (nextX + 64 > x && nextX < x + 64 &&
            character->getY() + 64 <= y &&
            nextY + 64 >= y);
    }
    bool willCollideFromBottom(AnimatedCharacter* character) const {
        if (!isActive) return false;
        float nextX = character->getX() + character->getVelocityX();
        float nextY = character->getY() + character->getVelocityY();
        return (nextX + 64 > x && nextX < x + 64 &&
            character->getY() >= y + 64 &&
            nextY <= y + 64);
    }
    bool willCollideFromLeft(AnimatedCharacter* character) const {
        if (!isActive) return false;
        float nextX = character->getX() + character->getVelocityX();
        float nextY = character->getY() + character->getVelocityY();
        return (nextY + 64 > y && nextY < y + 64 &&
            character->getX() + 64 <= x &&
            nextX + 64 >= x);
    }
    bool willCollideFromRight(AnimatedCharacter* character) const {
        if (!isActive) return false;
        float nextX = character->getX() + character->getVelocityX();
        float nextY = character->getY() + character->getVelocityY();
        return (nextY + 64 > y && nextY < y + 64 &&
            character->getX() >= x + 64 &&
            nextX <= x + 64);
    }

    // Called when a collision is detected
    virtual void onCollision(AnimatedCharacter* character) = 0;

    float getX() const { return x; }
    float getY() const { return y; }
    bool getActive() const { return isActive; }
    void setActive(bool active) { isActive = active; }

    virtual void draw(sf::RenderWindow& window, float cameraX, float cameraY) {
        if (!isActive) return;
        sprite.setPosition(x - cameraX, y - cameraY);
        window.draw(sprite);
    }
};

// ===== Subclasses =====

class Spike : public Obstacles {
public:
    Spike(float posX, float posY)
        : Obstacles(posX, posY) {
        loadTexture("Data/spike.png");
    }
    void onCollision(AnimatedCharacter* character) override {
        if (checkCollision(character)) {
            character->setVelocityX(-5.0f);
            character->setVelocityY(-5.0f);
            character->reduceHealth();
            if (!character->getInvincible())
                character->setInvincible(true);
        }
    }
};

class Crystal : public Obstacles {
public:
    Crystal(float posX, float posY)
        : Obstacles(posX, posY) {
        loadTexture("Data/crystal.png");
    }
    void onCollision(AnimatedCharacter* character) override {
        if (!isActive) return;
        if (willCollideFromTop(character)) {
            setActive(false);
            // Character must handle score externally
        }
        else if (willCollideFromLeft(character) || willCollideFromRight(character)) {
            character->setVelocityX(0);
        }
        else if (willCollideFromBottom(character)) {
            character->setVelocityY(0);
        }
    }
};

class BreakableWall : public Obstacles {
public:
    BreakableWall(float posX, float posY)
        : Obstacles(posX, posY) {
        loadTexture("Data/brick2.png");
    }
    void onCollision(AnimatedCharacter* character) override {
        if (!isActive) return;
        if (willCollideFromLeft(character) || willCollideFromRight(character)) {
            if (character->getNo() == 3)
                setActive(false);
            else
                character->setVelocityX(0);
        }
        else if (willCollideFromTop(character) || willCollideFromBottom(character)) {
            character->setVelocityY(0);
        }
    }
};

class Spring : public Obstacles {
public:
    Spring(float posX, float posY)
        : Obstacles(posX, posY) {
        loadTexture("Data/spring.png");
    }
    void onCollision(AnimatedCharacter* character) override {
        if (!isActive) return;
        if (willCollideFromTop(character)) {
            character->setVelocityY(-15.0f);
        }
        else if (willCollideFromLeft(character) || willCollideFromRight(character)) {
            character->setVelocityX(0);
        }
        else if (willCollideFromBottom(character)) {
            character->setVelocityY(0);
        }
    }
};
