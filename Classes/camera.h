#pragma once
#include <SFML/Graphics.hpp>

class Camera {
private:
    float x, y;
    float width, height;
    float levelWidth, levelHeight;

public:
    Camera(float w, float h, float lw, float lh) {
        width = w;
        height = h;
        levelWidth = lw;
        levelHeight = lh;
        x = 0;
        y = 0;
    }

    void update(float playerX, float playerY) {
        //center camera on player horizontally
        x = playerX - width / 2;

        //clamp camera to level boundaries
        if (x < 0) x = 0;
        if (x > levelWidth - width) x = levelWidth - width;

        // For vertical camera movement (if needed)
        // y = playerY - height / 2;
        // if (y < 0) y = 0;
        // if (y > levelHeight - height) y = levelHeight - height;
    }

    float getX() const { return x; }
    float getY() const { return y; }

    void setLevelWidth(float width) {
        levelWidth = width;
    }
};