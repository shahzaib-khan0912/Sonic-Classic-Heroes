#pragma once
#include <SFML/Graphics.hpp>

class Animation {
private:
    sf::Texture* spriteSheet;
    sf::IntRect currentFrame;
    float frameTime;          //time between frame changes
    float accumulatedTime;    //time since last frame change
    int frameCount;           //total frames in animation
    int currentFrameIndex;    //current frame index
    bool isLooping;           //should animation loop or stop at last frame
    bool isPlaying;           //is animation currently playing

    int frameWidth;           //width of one frame
    int frameHeight;          //height of one frame
    int startX;               //starting X position in the spritesheet
    int startY;               //starting Y position in the spritesheet

public:
    Animation() :
        spriteSheet(nullptr),
        frameTime(0.1f),
        accumulatedTime(0.0f),
        frameCount(1),
        currentFrameIndex(0),
        isLooping(true),
        isPlaying(false),
        frameWidth(0),
        frameHeight(0),
        startX(0),
        startY(0) {
    }


    void setup(sf::Texture* texture, int frameWidth, int frameHeight, int frameCount,
        float frameTime, bool loop = true, int startX = 0, int startY = 0) {
        this->spriteSheet = texture;
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;
        this->frameCount = frameCount;
        this->frameTime = frameTime;
        this->isLooping = loop;
        this->startX = startX;
        this->startY = startY;


        currentFrame = sf::IntRect(startX, startY, frameWidth, frameHeight);
        currentFrameIndex = 0;
        accumulatedTime = 0;
        isPlaying = true;
    }


    void update(float deltaTime) {
        if (!isPlaying || !spriteSheet)
            return;

        accumulatedTime += deltaTime;


        if (accumulatedTime >= frameTime) {

            currentFrameIndex++;
            accumulatedTime = 0;


            if (currentFrameIndex >= frameCount) {
                if (isLooping) {
                    currentFrameIndex = 0;
                }
                else {
                    currentFrameIndex = frameCount - 1;
                    isPlaying = false;
                }
            }


            currentFrame.left = startX + (currentFrameIndex * frameWidth);
            currentFrame.top = startY;
            currentFrame.width = frameWidth;
            currentFrame.height = frameHeight;
        }
    }

    //apply the current animation frame to a sprite
    void applyToSprite(sf::Sprite& sprite) {
        if (spriteSheet) {
            sprite.setTexture(*spriteSheet, true);
            sprite.setTextureRect(currentFrame);
        }
    }


    void play() {
        currentFrameIndex = 0;
        accumulatedTime = 0;
        isPlaying = true;


        currentFrame.left = startX;
        currentFrame.top = startY;
    }


    void pause() {
        isPlaying = false;
    }


    void resume() {
        isPlaying = true;
    }

    ////reset animation to first frame
    void reset() {
        currentFrameIndex = 0;
        accumulatedTime = 0;

        // Reset the frame rectangle
        currentFrame.left = startX;
        currentFrame.top = startY;
    }

    ////check if animation is finished (only relevant for non-looping animations)
    //bool isFinished() const {
    //    return !isLooping && currentFrameIndex >= frameCount - 1;
    //}


};