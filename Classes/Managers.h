#pragma once
#include "Level.h"
#include "AnimateCharacter.h"
#include "Sonic.h"
#include "Knuckles.h"
#include "Tails.h"
#include "Level.h"
#include "Camera.h"

class CharacterManager {
private:
    static const int MAX_CHARACTERS = 3;
    AnimatedCharacter* characters[MAX_CHARACTERS];
    int currentIndex;

public:
    CharacterManager(Sonic* sonic, Knuckles* knuckles, Tails* tails) : currentIndex(0) {
        characters[0] = sonic;
        characters[1] = knuckles;
        characters[2] = tails;
    }

    AnimatedCharacter* getControlled() {
        return characters[currentIndex];
    }

    void switchControl() {
        currentIndex = (currentIndex + 1) % MAX_CHARACTERS;
        // Add a small delay to prevent multiple switches
        sf::sleep(sf::milliseconds(200));
    }

    void handleInput(Level* level) {
        if (level) {
            getControlled()->handleInput(level);
        }
    }

    void update(float dt, Level* level, Camera* camera) {
        if (!level || !camera) return;

        // Get the controlled character
        AnimatedCharacter* leader = getControlled();

        // Update leader
        leader->update(dt, level, camera);
        leader->recordFollowPosition();

        // Update followers
        for (int i = 0; i < MAX_CHARACTERS; i++) {
            if (i != currentIndex) {
                // Follower characters mimic the leader's movement
                characters[i]->followLeaderMirroring(leader, dt, level, camera);
                characters[i]->update(dt, level, camera);
            }
        }
    }

    void draw(sf::RenderWindow& window, Camera* camera) {
        if (!camera) return;

        // Draw all characters
        for (int i = 0; i < MAX_CHARACTERS; i++) {
            characters[i]->draw(window, camera);
        }
    }
};

class LevelManager {
public:
    static const int MAX_LEVELS = 10;

private:
    Level* levels[MAX_LEVELS];
    int levelCount;
    int currentLevelIndex;

public:
    LevelManager() : levelCount(0), currentLevelIndex(-1) {
        // Initialize all level pointers to nullptr
        for (int i = 0; i < MAX_LEVELS; i++) {
            levels[i] = nullptr;
        }
    }

    ~LevelManager() {
        // Clean up all allocated levels
        for (int i = 0; i < levelCount; i++) {
            if (levels[i]) {
                delete levels[i];
                levels[i] = nullptr;
            }
        }
    }

    void addLevel(Level* level) {
        if (levelCount < MAX_LEVELS) {
            levels[levelCount++] = level;
        }
    }

    bool selectLevel(int index) {
        if (index >= 0 && index < levelCount) {
            currentLevelIndex = index;
            return true;
        }
        return false;
    }

    Level* getCurrent() {
        if (currentLevelIndex >= 0 && currentLevelIndex < levelCount) {
            return levels[currentLevelIndex];
        }
        return nullptr;
    }

    int getSelectedIndex() const {
        return currentLevelIndex;
    }

    void update(AnimatedCharacter* player, float dt) {
        if (Level* level = getCurrent()) {
            level->updateCollectibles(dt);
            level->updateEnemies(player->getX(), player->getY(), dt);
        }
    }
};