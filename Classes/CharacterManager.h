#pragma once
#include <SFML/Graphics.hpp>
#include "character.h"
#include "sonic.h"
#include "knuckles.h"
#include "tails.h"
#include"Shadow.h"
#include "level.h"
#include "camera.h"

class CharacterManager {
private:
    Sonic sonic;
    Knuckles knuckles;
    Tails tails;
    //Shadow shadow;
    Character* characters[3];
    int currentIndex;

public:
    CharacterManager() : currentIndex(0) {
        characters[0] = &sonic;
        characters[1] = &knuckles;
        characters[2] = &tails;
        //characters[3] = &shadow;
    }

    void initialize() {
        //set initial positions
        for (int i = 0; i < 3; i++) {
            characters[i]->setX(100.0f);
            characters[i]->setY(100.0f);
        }

        //set tails to follow other characters
        tails.setRiderTargets(&sonic, &knuckles);

        //set current character as active
        currentIndex = 0;
    }

    void switchCharacter() {
        currentIndex = (currentIndex + 1) % 3;
        
    }

    Character* getCurrentCharacter() {
        for (int i = 0; i < 3; i++) {
            if (i == currentIndex)
                characters[currentIndex]->setActiveChar(true);
            else
                characters[i]->setActiveChar(false);
        }
        
        return characters[currentIndex];
    }

    void updateFollowers(Character* leader, float deltaTime, Level* level, Camera* camera) {
        for (int i = 0; i < 3; i++) {
            if (i != currentIndex) {
                characters[i]->followLeaderMirroring(leader, deltaTime, level, camera);
                //characters[i]->checkCollectibles(level);
            }
        }
    }

    void checkFollowersBulletCollisions(Enemy** enemies, int enemyCount) {
        for (int i = 0; i < 3; i++) {
            if (i != currentIndex) {
                //characters[i]->checkBulletCollisions(enemies, enemyCount);
            }
        }
    }

    void drawCharacters(sf::RenderWindow& window, Camera* camera) {
        //draw all characters
        for (int i = 0; i < 3; i++) {
            characters[i]->draw(window, camera);
        }
    }

    void resetPositions(float x, float y) {
        for (int i = 0; i < 3; i++) {
            characters[i]->setX(x);
            characters[i]->setY(y);
            characters[i]->setVelocityX(0);
            characters[i]->setVelocityY(0);
        }
    }
    void resetHealth() {
        characters[currentIndex]->resetHealth();
    }
};