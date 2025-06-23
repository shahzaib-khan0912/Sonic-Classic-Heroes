#pragma once
#include <SFML/Graphics.hpp>
class Bullet {
private:
    float x, y;
    float speedX, speedY;
    bool isActive;
    sf::Sprite sprite;
    sf::Texture texture;
    //function to draw bullet hitbox (debugging purpose)
    void drawHitBox(sf::RenderWindow& window, float x, float y, float width, float height,
        sf::Color color = sf::Color(255, 0, 0, 128)) const {
        sf::RectangleShape hitbox;
        hitbox.setSize(sf::Vector2f(width, height));
        hitbox.setPosition(x, y);
        hitbox.setFillColor(sf::Color::Transparent);
        hitbox.setOutlineColor(color);
        hitbox.setOutlineThickness(1.0f);
        window.draw(hitbox);
    }
public:
    Bullet(float startX, float startY, float dirX, float dirY) : isActive(true) {
        x = startX;
        y = startY;
        //normalize direction if necessary
        float length = sqrt(dirX * dirX + dirY * dirY);
        if (length > 0) {
            dirX /= length;
            dirY /= length;
        }
        //set bullet speed 
        float baseSpeed = 15.0f;
        speedX = baseSpeed * dirX;
        speedY = baseSpeed * dirY;
        //load texture and set up sprite
        if (texture.loadFromFile("Data/enemies/bullet.png")) {
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
            sprite.setScale(2.0f, 2.0f);
        }
    }
    void update(float deltaTime) {
        if (!isActive) return;
        //update position based on speed and delta time
        x += speedX * deltaTime * 60.0f; // Scale by deltaTime and a factor to maintain consistent speed
        y += speedY * deltaTime * 60.0f;
        //update sprite position
        sprite.setPosition(x, y);
        //deactivate if bullet goes out of bounds
        if (x < -100 || x > 12900 || y < -100 || y > 996) {
            isActive = false;
        }
    }
    void draw(sf::RenderWindow& window, float cameraX) {
        if (!isActive) return;
        //draw sprite at camera-adjusted position
        sprite.setPosition(x - cameraX, y);
        window.draw(sprite);
        //optionally draw hitbox for debugging
        //drawHitBox(window, x - cameraX, y, 16, 16, sf::Color::Red);
    }
    //getters
    float getX() const { return x; }
    float getY() const { return y; }
    bool getIsActive() const { return isActive; }
    sf::FloatRect getBounds() const {
        return sf::FloatRect(x, y, 16, 16); //adjust size based on your sprite
    }
    //deactivate bullet
    void deactivate() { isActive = false; }
};