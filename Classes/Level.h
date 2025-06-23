#pragma once
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "camera.h"
#include "enemy.h"
#include "crabmeat.h"
#include "beebot.h"
#include "motobug.h"
#include "batbrain.h"
#include "collectible.h"
#include "Boss.h"
#include <iostream>
#include <fstream> 

using namespace sf;
using namespace std;
class Level {
protected:

    int height = 14;
    int width;
    int cell_size = 64;
    Music lvlMusic; //music for this level
    bool levelMusicOn = true;  // Add this member variable
    float levelMusicVolume = 40.0f;
    const float levelHeightPx = height * cell_size;
    Texture backGround;
    Sprite backGroundSprite;
    ///
    Sprite wall1; //for wall texture only
    Texture wall1Tex; //only for test;
    Sprite wall2; //for wall texture only
    Texture wall2Tex;
    Sprite wall3;
    Texture wall3Tex; //only for test;
    Sprite spike; //only for test;
    Texture sp; //only for test;
    Sprite crystal; //only for test;
    Texture cry; //only for test;
    Sprite checkPoint;
    Texture cp;


    int obsCount;
    int eneCount;
    int ringRequried;
    int ringCollected;
    float timer;
    float levelWidthPx;
    Image lvlLayout;
    

    Camera* camera;
    //for enemy 
    static const int MAX_ENEMIES = 50;
    Enemy* enemies[MAX_ENEMIES];
    int currentEnemyCount;

    //for collectibles
    static const int MAX_COLLECTIBLES = 100;
    Collectible* collectibles[MAX_COLLECTIBLES];
    int currentCollectibleCount;

public:
    int** lvl;
    Level(int wid, Camera* cam) {
        width = wid;
        camera = cam;
        lvl = new int* [height];
        for (int i = 0; i < height; ++i) {
            lvl[i] = new int[width];
            for (int j = 0; j < width; ++j) {
                lvl[i][j] = 0;
            }
        }
        levelWidthPx = width * cell_size;

        currentEnemyCount = 0;
        currentCollectibleCount = 0;
        obsCount = 0;
        eneCount = 0;
        ringRequried = 0;
        ringCollected = 0;
        timer = 0.0f;
        
    
        //initialize enemy pointers to nullptr
        for (int i = 0; i < MAX_ENEMIES; ++i) {
            enemies[i] = nullptr;
        }
        //initialize collectible pointers to nullptr
        for (int i = 0; i < MAX_COLLECTIBLES; ++i) {
            collectibles[i] = nullptr;
        }
        backGroundSprite.setScale(1.2f, 1.0f);
    }
    

    void setGrid(int x, int y) {
        lvl[x][y] == 0;
    }
    void loadBackGround(const char* path) {
        backGround.loadFromFile(path);
        backGroundSprite.setTexture(backGround);
        backGroundSprite.setTextureRect(IntRect(0, 0, levelWidthPx, levelHeightPx));
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void loadMusic(const char* path) {
        if (lvlMusic.openFromFile(path)) {
            lvlMusic.setVolume(40);
            lvlMusic.setLoop(true);
            lvlMusic.play();
        }
        else {
            std::cout << "Failed to load music: " << path << std::endl;
        }
    }

    void setMusicVolume(float volume) {
        levelMusicVolume = volume;
        lvlMusic.setVolume(levelMusicVolume);
    }

    // Function to get current level music volume
    float getMusicVolume() {
        return levelMusicVolume;
    }

    // Function to turn level music on/off
    void setMusicEnabled(bool enabled) {
        levelMusicOn = enabled;
        if (levelMusicOn) {
            lvlMusic.play();
        }
        else {
            lvlMusic.pause();
        }
    }

    // Function to check if level music is enabled
    bool isMusicEnabled() {
        return levelMusicOn;
    }

    // Function to stop level music completely
    void stopMusic() {
        lvlMusic.stop();
    }

    // Function to resume level music (if it was enabled)
    void resumeMusic() {
        if (levelMusicOn) {
            lvlMusic.play();
        }
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual void build() {
        for (int j = 0; j < width; ++j) {
            lvl[height - 1][j] = 'w';
        }
    }
    //collision with wall, spike and crystals 
    int checkCollisionWithWalls(float playerX, float playerY, float playerWidth, float playerHeight, int id) {
        int playerCellX = playerX / cell_size;
        int playerCellY = playerY / cell_size;

        int range = 1;

        int startI = (playerCellY - range < 0) ? 0 : playerCellY - range;
        int endI = (playerCellY + range >= height) ? height - 1 : playerCellY + range;

        int startJ = (playerCellX - range < 0) ? 0 : playerCellX - range;
        int endJ = (playerCellX + range >= width) ? width - 1 : playerCellX + range;

        for (int i = startI; i <= endI; ++i) {
            for (int j = startJ; j <= endJ; ++j) {
                if (lvl[i][j] == 1 || lvl[i][j] == 2 || lvl[i][j] == 4) {
                    float wallX = j * cell_size;
                    float wallY = i * cell_size;
                    float wallSize = cell_size;

                    if ((playerX < wallX + wallSize) &&
                        (playerX + playerWidth > wallX) &&
                        (playerY < wallY + wallSize) &&
                        (playerY + playerHeight > wallY)) {

                        switch (lvl[i][j]) {
                        case 1: return 1;
                        case 2:
                            if (id == 2) {
                                lvl[i][j] = 0;
                                return 2;
                            }
                            else {
                                return 2;
                            }
                        case 4: return 4;
                        default: return 1;
                        }
                    }
                }
            }
        }

        return 0;
    }


    Sprite getBackGroundSprite() {
        return backGroundSprite;
    }

    virtual void setTextures() = 0;


    void drawObstacles(RenderWindow& window) {
        setTextures();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (lvl[i][j] == 1) { //1 = wall
                    wall1.setPosition(
                        j * cell_size - camera->getX(), i * cell_size);
                    window.draw(wall1);
                }
                else if (lvl[i][j] == 2) { //2 = redwall
                    wall2.setPosition(j * cell_size - camera->getX(), i * cell_size);
                    window.draw(wall2);
                }
                else if (lvl[i][j] == 3) { //3 = crystal
                    crystal.setPosition(j * cell_size - camera->getX(), i * cell_size);
                    window.draw(crystal);
                }
                else if (lvl[i][j] == 4) { //4 = spike
                    spike.setPosition(j * cell_size - camera->getX(), i * cell_size);
                    window.draw(spike);
                }
                else if (lvl[i][j] == 12) {
                    wall3.setPosition(j * cell_size - camera->getX(), i * cell_size);
                    window.draw(wall3);
                }
                checkPoint.setPosition(198 * cell_size - camera->getX(), 12 * cell_size);
                window.draw(checkPoint);
            }
        }
    }

    void loadLevelLayout(const char* path) {
        lvlLayout.loadFromFile(path);
    }

    //enemy functions
    void addEnemy(Enemy* enemy) {
        if (currentEnemyCount < MAX_ENEMIES) {
            enemies[currentEnemyCount++] = enemy;
            eneCount++;
        }
    }

    void updateEnemies(float playerX, float playerY, float deltaTime) {
        for (int i = 0; i < currentEnemyCount; ++i) {
            if (enemies[i] && enemies[i]->getIsActive()) {
                enemies[i]->update(playerX, playerY, deltaTime);
            }
        }
    }

    void drawEnemy(RenderWindow& window) {
        for (int i = 0; i < currentEnemyCount; ++i) {
            if (enemies[i] && enemies[i]->getIsActive()) {
                enemies[i]->draw(window, camera->getX(), camera->getY());
            }
        }
    }
    Enemy** getEnemy() {
        return enemies;
    }

    int getEnemyCount() {
        return currentEnemyCount;
    }

    //collectible functions
    void addCollectible(Collectible* collectible) {
        if (currentCollectibleCount < MAX_COLLECTIBLES) {
            collectibles[currentCollectibleCount++] = collectible;
            if (collectible->getType() == 1) { //ring
                ringRequried++;
            }
        }
    }

    void updateCollectibles(float deltaTime) {
        for (int i = 0; i < currentCollectibleCount; ++i) {
            if (collectibles[i] && collectibles[i]->getIsActive()) {
                collectibles[i]->update(deltaTime);
            }
        }
    }

    void drawCollectibles(RenderWindow& window) {
        for (int i = 0; i < currentCollectibleCount; ++i) {
            if (collectibles[i] && collectibles[i]->getIsActive()) {
                collectibles[i]->draw(window, camera->getX(), 0);
            }
        }
    }

    

    Collectible** getCollectibles() {
        return collectibles;
    }

    

    int getCollectibleCount() {
        return currentCollectibleCount;
    }

    void incrementRingCollected() {
        ringCollected++;
    }

    int getRingCollected() {
        return ringCollected;
    }

    int getRingRequired() {
        return ringRequried;
    }

    int** getLevelGrid() {
        return lvl;
    }
    int getRows() {
        return height;
    }
    int getCols() {
        return width;
    }

    //destructor
    ~Level() {
        //stop music when level is destroyed
        lvlMusic.stop();

        //clean up enemies
        for (int i = 0; i < currentEnemyCount; ++i) {
            if (enemies[i]) {
                delete enemies[i];
            }
        }
        //clean up collectibles
        for (int i = 0; i < currentCollectibleCount; ++i) {
            if (collectibles[i]) {
                delete collectibles[i];
            }
        }

        //clean up level grid
        for (int i = 0; i < height; ++i) {
            delete[] lvl[i];
        }
        delete[] lvl;
    }
};

class Level1 : public Level {
public:
    //constructor
    Level1(int wid, Camera* cam) : Level(wid, cam) {}

    //just for checking:
    bool isColorNear(Color a, Color b, int tolerance = 10) {
        return abs(a.r - b.r) <= tolerance &&
            abs(a.g - b.g) <= tolerance &&
            abs(a.b - b.b) <= tolerance;
    }
    virtual void setTextures() override {
        wall1Tex.loadFromFile("Data/brick1.png");
        wall1.setTexture(wall1Tex);
        wall2Tex.loadFromFile("Data/brick2.png");
        wall2.setTexture(wall2Tex);
        wall3Tex.loadFromFile("Data/brick3.png");
        wall3.setTexture(wall3Tex);
        sp.loadFromFile("Data/spike.png");
        spike.setTexture(sp);
        cry.loadFromFile("Data/crystal.png");
        crystal.setTexture(cry);
        crystal.setScale(0.68085f, 1.0f);
        cp.loadFromFile("Data/checkPoint.png");
        checkPoint.setTexture(cp);
    }
    void build() {
        ifstream file("Data/lvl_layout1.txt");

        if (!file.is_open()) {
            std::cout << "Failed to open file!" << std::endl;
            return;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file >> lvl[y][x];

                //adding enemies based on level layout
                if (lvl[y][x] == 5) { //5 = batbrain 
                    addEnemy(new BatBrain(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 6) { //6 = beebot
                    addEnemy(new BeeBot(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 7) { //7 = motobug 
                    addEnemy(new Motobug(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 8) { //8 = crabmeat 
                    addEnemy(new CrabMeat(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                //add collectibles based on level layout
                else if (lvl[y][x] == 9) { //9 = ring
                    addCollectible(new Ring(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 10) { //10 = extralife
                    addCollectible(new ExtraLife(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 11) { //11 = specialboost
                    addCollectible(new SpecialBoost(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }

                if (file.fail()) {
                    lvl[y][x] = -1;
                }
            }
        }

        //manually adding some enemies at strategic positions

        //batbrain
        addEnemy(new BatBrain(45 * cell_size, 3 * cell_size));  //near first spikes
        addEnemy(new BatBrain(80 * cell_size, 6 * cell_size));  //middle section

        //beebots 
        addEnemy(new BeeBot(120 * cell_size, 5 * cell_size));
        addEnemy(new BeeBot(45 * cell_size, 4 * cell_size));
        addEnemy(new BeeBot(80 * cell_size, 5 * cell_size));

        //motobug
        addEnemy(new Motobug(29 * cell_size, 11 * cell_size)); //first platform
        addEnemy(new Motobug(67 * cell_size, 12 * cell_size)); //middle platform

        //crabmeats 
        addEnemy(new CrabMeat(96 * cell_size, 12 * cell_size)); //guarding crystals
        addEnemy(new CrabMeat(152 * cell_size, 9 * cell_size)); //near end

        file.close();
    }
};

class Level2 : public Level {
public:
    //constructor
    Level2(int wid, Camera* cam) : Level(wid, cam) {}

    //just for checking:
    bool isColorNear(Color a, Color b, int tolerance = 10) {
        return abs(a.r - b.r) <= tolerance &&
            abs(a.g - b.g) <= tolerance &&
            abs(a.b - b.b) <= tolerance;
    }
    virtual void setTextures() override {
        wall1Tex.loadFromFile("Data/unbreakAbleWall.png");
        wall1.setTexture(wall1Tex);
        wall2Tex.loadFromFile("Data/brick2.png");
        wall2.setTexture(wall2Tex);
        wall3Tex.loadFromFile("Data/brick3.png");
        wall3.setTexture(wall3Tex);
        sp.loadFromFile("Data/spike.png");
        spike.setTexture(sp);
        cry.loadFromFile("Data/crystal.png");
        crystal.setTexture(cry);
        crystal.setScale(0.68085f, 1.0f);
        cp.loadFromFile("Data/checkPoint.png");
        checkPoint.setTexture(cp);
    }
    void build() {
        ifstream file("Data/lvl_layout2.txt");

        if (!file.is_open()) {
            std::cout << "Failed to open file!" << std::endl;
            return;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file >> lvl[y][x];

                //adding enemies based on level layout
                if (lvl[y][x] == 5) { //5 = batbrain 
                    addEnemy(new BatBrain(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 6) { //6 = beebot
                    addEnemy(new BeeBot(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 7) { //7 = motobug 
                    addEnemy(new Motobug(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 8) { //8 = crabmeat 
                    addEnemy(new CrabMeat(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                //add collectibles based on level layout
                else if (lvl[y][x] == 9) { //9 = ring
                    addCollectible(new Ring(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 10) { //10 = extralife
                    addCollectible(new ExtraLife(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 11) { //11 = specialboost
                    addCollectible(new SpecialBoost(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }

                if (file.fail()) {
                    lvl[y][x] = -1;
                }
            }
        }

        //manually adding some enemies at strategic positions

        //batbrain
        addEnemy(new BatBrain(15 * cell_size, 5 * cell_size));  //first open area
        addEnemy(new BatBrain(45 * cell_size, 3 * cell_size));  //near first spikes
        addEnemy(new BatBrain(80 * cell_size, 6 * cell_size));  //middle section

        //beebots
        addEnemy(new BeeBot(15 * cell_size, 4 * cell_size));   //after first obstacle
        addEnemy(new BeeBot(120 * cell_size, 5 * cell_size));  //before checkpoint
        addEnemy(new BeeBot(45 * cell_size, 4 * cell_size));   //after first obstacle

        //motobug
        addEnemy(new Motobug(29 * cell_size, 11 * cell_size)); //first platform
        addEnemy(new Motobug(66 * cell_size, 12 * cell_size)); //middle platform

        //crabmeats 
        addEnemy(new CrabMeat(96 * cell_size, 12 * cell_size)); //guarding crystals
        addEnemy(new CrabMeat(154 * cell_size, 9 * cell_size)); //near end

        file.close();
    }
};

class Level3 : public Level {
public:
    //constructor
    Level3(int wid, Camera* cam) : Level(wid, cam) {}

    virtual void setTextures() override {
        wall1Tex.loadFromFile("Data/brick32.png");
        wall1.setTexture(wall1Tex);
        wall2Tex.loadFromFile("Data/brick2.png");
        wall2.setTexture(wall2Tex);
        wall3Tex.loadFromFile("Data/brick33.png");
        wall3.setTexture(wall3Tex);
        sp.loadFromFile("Data/spike.png");
        spike.setTexture(sp);
        cry.loadFromFile("Data/crystal.png");
        crystal.setTexture(cry);
        crystal.setScale(0.68085f, 1.0f);
        cp.loadFromFile("Data/checkPoint.png");
        checkPoint.setTexture(cp);
    }
    void build() {
        ifstream file("Data/lvl_layout3.txt");

        if (!file.is_open()) {
            //fallback to level 2 layout if level 3 layout doesn't exist
            file.open("Data/lvl_layout2.txt");
            if (!file.is_open()) {
                std::cout << "Failed to open file!" << std::endl;
                return;
            }
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file >> lvl[y][x];

                //adding enemies based on level layout
                if (lvl[y][x] == 5) { //5 = batbrain 
                    addEnemy(new BatBrain(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 6) { //6 = beebot
                    addEnemy(new BeeBot(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 7) { //7 = motobug 
                    addEnemy(new Motobug(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 8) { //8 = crabmeat 
                    addEnemy(new CrabMeat(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                //add collectibles based on level layout
                else if (lvl[y][x] == 9) { //9 = ring
                    addCollectible(new Ring(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 10) { //10 = extralife
                    addCollectible(new ExtraLife(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 11) { //11 = specialboost
                    addCollectible(new SpecialBoost(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }

                if (file.fail()) {
                    lvl[y][x] = -1;
                }
            }
        }

        ////rings in challenging locations
        //for (int i = 0; i < 3; i++) {
        //    addCollectible(new Ring(20 * cell_size + i * 40, 8 * cell_size)); //row of rings
        //}
        //addCollectible(new Ring(40 * cell_size, 6 * cell_size)); //above platform
        //addCollectible(new Ring(60 * cell_size, 5 * cell_size)); //hard to reach
        //addCollectible(new Ring(100 * cell_size, 7 * cell_size)); //mid-air

        ////extra lives hidden in difficult spots
        //addCollectible(new ExtraLife(35 * cell_size, 4 * cell_size)); //hidden high
        //addCollectible(new ExtraLife(85 * cell_size, 10 * cell_size)); //behind enemies
        //addCollectible(new ExtraLife(150 * cell_size, 6 * cell_size)); //near end

        ////special boosts at strategic points
        //addCollectible(new SpecialBoost(30 * cell_size, 9 * cell_size)); //early boost
        //addCollectible(new SpecialBoost(75 * cell_size, 8 * cell_size)); //mid-level
        //addCollectible(new SpecialBoost(130 * cell_size, 7 * cell_size)); //before final section

        ////add more challenging enemies for level 3
        addEnemy(new BatBrain(25 * cell_size, 3 * cell_size));
        addEnemy(new BatBrain(55 * cell_size, 4 * cell_size));
        addEnemy(new BatBrain(95 * cell_size, 5 * cell_size));
        addEnemy(new BatBrain(135 * cell_size, 3 * cell_size));

        addEnemy(new BeeBot(40 * cell_size, 5 * cell_size));
        addEnemy(new BeeBot(80 * cell_size, 4 * cell_size));
        addEnemy(new BeeBot(120 * cell_size, 6 * cell_size));

        addEnemy(new Motobug(35 * cell_size, 12 * cell_size));
        addEnemy(new Motobug(75 * cell_size, 12 * cell_size));
        addEnemy(new Motobug(115 * cell_size, 12 * cell_size));

        addEnemy(new CrabMeat(50 * cell_size, 12 * cell_size));
        addEnemy(new CrabMeat(100 * cell_size, 12 * cell_size));
        addEnemy(new CrabMeat(150 * cell_size, 12 * cell_size));

        file.close();
    }
};



class Level4 : public Level {
public:
    //constructor
    Level4(int wid, Camera* cam) : Level(wid, cam) {}

    virtual void setTextures() override {
        wall1Tex.loadFromFile("Data/brick1.png");
        wall1.setTexture(wall1Tex);
        wall2Tex.loadFromFile("Data/brick2.png");
        wall2.setTexture(wall2Tex);
        wall3Tex.loadFromFile("Data/brick3.png");
        wall3.setTexture(wall3Tex);
        sp.loadFromFile("Data/spike.png");
        spike.setTexture(sp);
        cry.loadFromFile("Data/crystal.png");
        crystal.setTexture(cry);
        crystal.setScale(0.68085f, 1.0f);
        cp.loadFromFile("Data/checkPoint.png");
        checkPoint.setTexture(cp);
    }
    void build() {
        ifstream file("Data/lvl_layout0.txt");
        if (!file.is_open()) {
            std::cout << "Failed to open file!" << std::endl;
            return;
        }

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file >> lvl[y][x];
                // Enemy placement
                if (lvl[y][x] == 5) {
                    addEnemy(new BatBrain(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 6) {
                    addEnemy(new BeeBot(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 7) {
                    addEnemy(new Motobug(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 8) {
                    addEnemy(new CrabMeat(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                // Collectible placement
                else if (lvl[y][x] == 9) {
                    addCollectible(new Ring(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 10) {
                    addCollectible(new ExtraLife(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                else if (lvl[y][x] == 11) {
                    addCollectible(new SpecialBoost(x * cell_size, y * cell_size));
                    lvl[y][x] = 0;
                }
                if (file.fail()) {
                    lvl[y][x] = -1;
                }
            }
        }
        // Add the boss
        addEnemy(new Boss(9 * cell_size, 7 * cell_size));
        addEnemy(new BatBrain(3 * cell_size, 3 * cell_size));  //near first spikes
        addEnemy(new BatBrain(4 * cell_size, 6 * cell_size));  //middle section

        ////beebots 
        addEnemy(new BeeBot(7 * cell_size, 5 * cell_size));
        addEnemy(new BeeBot(8 * cell_size, 4 * cell_size));
        addEnemy(new BeeBot(9 * cell_size, 5 * cell_size));

        //motobug
        addEnemy(new Motobug(12 * cell_size, 11 * cell_size)); //first platform
        addEnemy(new Motobug(13 * cell_size, 12 * cell_size)); //middle platform

        ////crabmeats 
        addEnemy(new CrabMeat(7 * cell_size, 12 * cell_size)); //guarding crystals
        addEnemy(new CrabMeat(8 * cell_size, 9 * cell_size)); //near end
        addCollectible(new Ring(4 * cell_size, 6 * cell_size)); //above platform
        addCollectible(new Ring(5 * cell_size, 5 * cell_size)); //hard to reach
        addCollectible(new Ring(10 * cell_size, 7 * cell_size)); //mid-air

        //////extra lives hidden in difficult spots
        addCollectible(new ExtraLife(1 * cell_size, 1 * cell_size)); //hidden high
        addCollectible(new ExtraLife(2 * cell_size, 1 * cell_size)); //behind enemies
        addCollectible(new ExtraLife(3 * cell_size, 1 * cell_size)); //near end
        addCollectible(new ExtraLife(4 * cell_size, 1 * cell_size)); //near end
        addCollectible(new ExtraLife(5 * cell_size, 1 * cell_size)); //near end
        addCollectible(new ExtraLife(6 * cell_size, 1 * cell_size)); //near end

        //////special boosts at strategic points
        addCollectible(new SpecialBoost(1 * cell_size, 3 * cell_size)); //early boost
        addCollectible(new SpecialBoost(2 * cell_size, 3 * cell_size)); //mid-level
        addCollectible(new SpecialBoost(3 * cell_size, 3 * cell_size)); //before final section
        addCollectible(new SpecialBoost(4 * cell_size, 3 * cell_size)); //early boost
        addCollectible(new SpecialBoost(5 * cell_size, 3 * cell_size)); //early boost
        file.close();
    }
};


class Level5 : public Level {
public:
    //constructor
    Level5(int wid, Camera* cam) : Level(wid, cam) {}

    //just for checking:
   
    virtual void setTextures() override {
        wall1Tex.loadFromFile("Data/brick1.png");
        wall1.setTexture(wall1Tex);
        wall2Tex.loadFromFile("Data/brick2.png");
        wall2.setTexture(wall2Tex);
        wall3Tex.loadFromFile("Data/brick3.png");
        wall3.setTexture(wall3Tex);
        sp.loadFromFile("Data/spike.png");
        spike.setTexture(sp);
        cry.loadFromFile("Data/crystal.png");
        crystal.setTexture(cry);
        crystal.setScale(0.68085f, 1.0f);
        cp.loadFromFile("Data/checkPoint.png");
        checkPoint.setTexture(cp);
    }
    void build() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; i++) {
                if (i == 0 || i == width - 1 || j == 0 || j == height - 1)
                    lvl[i][j] == 1;

            }
        }
    }
};