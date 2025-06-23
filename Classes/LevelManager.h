#pragma once
#include <iostream>
#include "level.h"
#include "camera.h"

class LevelManager {
private:
    Level* currentLevel;
    int currentLevelNumber;
    Camera* camera;
    const int totalLevels = 4;

    // Music settings that will be applied to all levels
    bool globalMusicEnabled = true;
    float globalMusicVolume = 40.0f;

    // Store level dimensions for each level
    struct LevelDimensions {
        int width;
        int height;
    };
    LevelDimensions levelSizes[4] = {
        {200, 14}, // level 1: 200x14
        {227, 14}, // level 2: 227x14
        {218, 14}, // level 3: 218x14
        {19, 14}
        //{400,14}// level 4: 19x14
    };

    // Music paths for each level
    const char* levelMusicPaths[4] = {
        "Data/lv1.mp3", // level 1 music
        "Data/super02.mp3", // level 2 music
        "Data/hill.mp3", // level 3 music
        "Data/Boss.mp3"
        //"Data/hill.mp3"// level 4 music
    };

public:
    LevelManager(Camera* cam) : camera(cam), currentLevel(nullptr), currentLevelNumber(0) {}

    ~LevelManager() {
        if (currentLevel != nullptr) {
            delete currentLevel;
        }
    }

    void loadLevel(int levelNumber) {
        // Clean up previous level if it exists
        if (currentLevel != nullptr) {
            delete currentLevel;
            currentLevel = nullptr;
        }

        currentLevelNumber = levelNumber;
        int levelIndex = levelNumber - 1; // convert to 0-based index

        // Update camera bounds for the new level
        camera->setLevelWidth(levelSizes[levelIndex].width * 64); // 64 is cell size

        // Create the appropriate level with correct width
        switch (levelNumber) {
        case 1:
            currentLevel = new Level1(levelSizes[levelIndex].width, camera);
            currentLevel->loadBackGround("Data/level1.png");

            currentLevel->loadMusic(levelMusicPaths[levelIndex]);
            
            break;
        case 2:
            currentLevel = new Level2(levelSizes[levelIndex].width, camera);
            currentLevel->loadBackGround("Data/level2.png");
            currentLevel->loadMusic(levelMusicPaths[levelIndex]);
            break;
        case 3:
            currentLevel = new Level3(levelSizes[levelIndex].width, camera);
            currentLevel->loadBackGround("Data/level3.png");
            currentLevel->loadMusic(levelMusicPaths[levelIndex]);
            currentLevel->setMusicVolume(80);
            break;
        case 4:
            currentLevel = new Level4(19, camera);
            currentLevel->loadBackGround("Data/level1.png");
            currentLevel->loadMusic(levelMusicPaths[levelIndex]);
            break;

        default:
            std::cout << "Invalid level number: " << levelNumber << std::endl;
            currentLevel = new Level1(levelSizes[0].width, camera);
            currentLevel->loadBackGround("Data/bg2.png");
            currentLevel->loadMusic(levelMusicPaths[0]);
            break;
        }

    
        
        // Build the level
        currentLevel->build();

        // Apply current music settings to the new level
        applyMusicSettings();

        std::cout << "Level " << levelNumber << " loaded with dimensions: "
            << levelSizes[levelIndex].width << "x" << levelSizes[levelIndex].height << std::endl;
    }

    bool advanceToNextLevel() {
        if (currentLevelNumber < totalLevels) {
            loadLevel(currentLevelNumber + 1);
            return true;
        }
        return false; // no more levels
    }

    Level* getCurrentLevel() {
        return currentLevel;
    }

    int getCurrentLevelNumber() {
        return currentLevelNumber;
    }

    int getCurrentLevelWidth() {
        int levelIndex = currentLevelNumber - 1;
        if (levelIndex >= 0 && levelIndex < totalLevels) {
            return levelSizes[levelIndex].width;
        }
        return 200; // default
    }

    // NEW MUSIC CONTROL FUNCTIONS

    // Set music volume for current level
    void setMusicVolume(float volume) {
        globalMusicVolume = volume;
        if (currentLevel) {
            currentLevel->setMusicVolume(volume);
        }
    }

    // Get current music volume
    float getMusicVolume() {
        return globalMusicVolume;
    }

    // Enable/disable music for current level
    void setMusicEnabled(bool enabled) {
        globalMusicEnabled = enabled;
        if (currentLevel) {
            currentLevel->setMusicEnabled(enabled);
        }
    }

    // Check if music is enabled
    bool isMusicEnabled() {
        return globalMusicEnabled;
    }

    // Stop current level music
    void stopCurrentLevelMusic() {
        if (currentLevel) {
            currentLevel->stopMusic();
        }
    }

    // Resume current level music
    /*void resumeCurrentLevelMusic() {
        if (currentLevel) {
            currentLevel->resumeMusic();
        }
    }*/

    // Apply current music settings to current level
    void applyMusicSettings() {
        if (currentLevel) {
            currentLevel->setMusicVolume(globalMusicVolume);
            currentLevel->setMusicEnabled(globalMusicEnabled);
        }
    }

    // Get music path for specific level (useful for debugging)
    const char* getLevelMusicPath(int levelNumber) {
        int levelIndex = levelNumber - 1;
        if (levelIndex >= 0 && levelIndex < totalLevels) {
            return levelMusicPaths[levelIndex];
        }
        return levelMusicPaths[0]; // default
    }


    void pauseCurrentLevelMusic() {
        if (currentLevel) {
            currentLevel->stopMusic();
        }
    }

    // Resume current level music (returns true if successful)
    bool resumeCurrentLevelMusic() {
        if (currentLevel) {
            currentLevel->resumeMusic();
            return true;
        }
        return false;
    }

    // Check if current level music is playing
    bool isCurrentLevelMusicPlaying() {
        if (currentLevel) {
            return currentLevel->isMusicEnabled();
        }
        return false;
    }

    // Toggle music play/pause for current level
    void toggleCurrentLevelMusic() {
        if (currentLevel) {
            if (currentLevel->isMusicEnabled()) {
                currentLevel->stopMusic();
            }
            else {
                currentLevel->resumeMusic();
            }
        }
    }
};

// UPDATED MENU CLASS INTEGRATION


/*
int main() {
    // Create camera and level manager
    Camera camera;
    LevelManager levelManager(&camera);
    Menu menu;

    // Connect menu with level manager
    menu.setLevelManager(&levelManager);

    // Load initial level
    levelManager.loadLevel(1);

    // Sync initial music settings
    menu.syncMusicSettings();

    // Game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            menu.handleEvent(event);
            // ... other event handling
        }

        // When starting game from menu:
        if (menu.shouldStartGame()) {
            menu.stopMusic();  // Stop menu music
            levelManager.resumeCurrentLevelMusic();  // Start level music
            // ... start game logic
        }

        // When returning to menu:
        if (gameEnded) {
            levelManager.stopCurrentLevelMusic();  // Stop level music
            menu.resumeMusic();  // Start menu music
        }

        // When advancing levels:
        if (levelCompleted) {
            if (levelManager.advanceToNextLevel()) {
                menu.syncMusicSettings();  // Apply settings to new level
            }
        }
    }

    return 0;
}
*/