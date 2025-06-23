#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "level.h"
#include "camera.h"
#include "charactermanager.h"
#include "levelmanager.h"
#include "menu.h"

class Game {
private:
    sf::RenderWindow window;
    Camera camera;
    CharacterManager characterManager;
    LevelManager levelManager;
    Menu menu;
    sf::Clock gameClock;
    float deltaTime;
    bool isGameRunning;
    bool isGamePaused;  // NEW: Added pause state
    bool isLevelCompleted;
    float levelTransitionTimer;
    const float LEVEL_TRANSITION_DELAY = 5.0f;
    const float LEVEL_COMPLETION_THRESHOLD = 0.96f;
    sf::Font font; //font for displaying health
    bool isGameOver; //flag to track game over state
    sf::Music gameMusic; //music for gameplay
    sf::Sprite currentPLayer;
    sf::Texture currentPLayerTex;

    sf::Sprite circle;
    sf::Texture cirTex;

    SoundBuffer tails;
    SoundBuffer knuckles;
    SoundBuffer sonic;
    Sound s;
    Sound sp;
    Sound k;

public:
    Game() :
        window(sf::VideoMode(1200, 896), "Sonic Classic Heros"),
        camera(1200, 896, 12800, 896),
        characterManager(),
        levelManager(&camera),
        deltaTime(0.0f),
        isGameRunning(false),
        isGamePaused(false),  // NEW: Initialize pause state
        isLevelCompleted(false),
        levelTransitionTimer(0.0f),
        isGameOver(false)
    {
        window.setFramerateLimit(60);

        // Connect menu with level manager for better integration
        menu.setLevelManager(&levelManager);

        //load font for health display
        if (!font.loadFromFile("Data/Fonts/text.otf")) {
            std::cout << "Failed to load font" << std::endl;
        }

        //load game music
        if (!gameMusic.openFromFile("Data/sound/menumusic.mp3")) {
            std::cout << "Failed to load game music" << std::endl;
            //try to load menu music as fallback
            if (!gameMusic.openFromFile("Data/sound/menumusic.mp3")) {
                std::cout << "Failed to load fallback music" << std::endl;
            }
        }
        gameMusic.setLoop(true);
        gameMusic.setVolume(40);

        currentPLayerTex.loadFromFile("Data/player.png");
        currentPLayer.setTexture(currentPLayerTex);

        cirTex.loadFromFile("Data/circle.png");
        circle.setTexture(cirTex);
        circle.setScale(2.5f, 2.5f);

        // Sync initial music settings
        menu.syncMusicSettings();

        tails.loadFromFile("Data/tails.wav");
        sonic.loadFromFile("Data/sonicS.wav");
        knuckles.loadFromFile("Data/kk.wav");
        //sp.setBuffer(sonic);
    }

    void initializeGame() {
        //verify player name is set before starting
        const char* playerName = menu.getPlayerName();
        if (playerName[0] == '\0') {
            std::cout << "Error: Player name not set" << std::endl;
            /*Collectible::resetScore();
            characterManager.resetHealth();*/
            return;
        }

        characterManager.initialize();
        levelManager.loadLevel(menu.getChosenLevel()); //load the selected level

        if (levelManager.getCurrentLevel()->getCols() == 19) {
            characterManager.getCurrentCharacter()->setX(600);
            characterManager.getCurrentCharacter()->setY(700);
        }

        isGameRunning = true;
        isGamePaused = false;  // NEW: Reset pause state
        menu.setAsPaused(false);
        isLevelCompleted = false;
        isGameOver = false;
        menu.resetStartGameRequest(); //reset the start game flag

        //stop menu music when game starts
        menu.stopMusic();

        // Use level-specific music if available, otherwise use game music
        /*if (!levelManager.resumeCurrentLevelMusic()) {
            gameMusic.play();
        }*/

        std::cout << "Starting game with player: " << playerName << " on level: " << menu.getChosenLevel() << std::endl;
    }

    void run() {
        while (window.isOpen()) {
            deltaTime = gameClock.restart().asSeconds();

            handleEvents();

            // NEW: Modified update logic to handle pause state
            if (isGameRunning) {
                if (!isGamePaused) {
                    updateGame();
                }
                else {
                    handlePauseMenuActions();  // NEW: Handle pause menu interactions
                }
            }

            render();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (isGameRunning) {
                if (isGamePaused) {
                    // NEW: Handle pause menu events
                    menu.handleEvent(event);
                }
                else {
                    handleGameEvents(event);
                }
            }
            else {
                menu.handleEvent(event);
                //check if player has entered name and requested to start game
                if (menu.shouldStartGame()) {
                    initializeGame();
                }
                // NEW: Handle exit request from main menu
                if (menu.shouldExit()) {
                    window.close();
                }
            }
        }
    }

    void handleGameEvents(sf::Event& event) {
        // NEW: Modified ESC key handling for pause menu
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && isGameRunning) {
            isGamePaused = true;
            menu.setAsPaused(true);
            menu.showPauseMenu();

            //pause game and level music (don't stop completely)
            //gameMusic.pause();
            levelManager.pauseCurrentLevelMusic();  // You'll need to add this method to LevelManager

            return;  // Don't execute the old ESC behavior
        }

        if (!isLevelCompleted && !isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
            characterManager.switchCharacter();
            int iD = characterManager.getCurrentCharacter()->getId();
            if (iD == 1) {
                sp.setBuffer(sonic);
                sp.play();
            }

            else if (iD == 3) {
                s.setBuffer(tails);
                s.play();
            }
            else if (iD == 2) {
                k.setBuffer(knuckles);
                k.play();
            }

            sf::sleep(sf::milliseconds(200)); //small delay to prevent rapid switching
        }
    }

    // NEW: Handle pause menu actions
    void handlePauseMenuActions() {
        if (menu.shouldResume()) {
            isGamePaused = false;
            menu.setAsPaused(false);
            menu.resetResumeRequest();

            // Resume game music
            //gameMusic.play();
            //menu.resumeMusic();
            levelManager.resumeCurrentLevelMusic();  // You'll need to add this method to LevelManager
        }

        if (menu.shouldReturnToMainMenu()) {
            isGameRunning = false;
            isGamePaused = false;
            menu.setAsPaused(false);
            menu.resetReturnToMainMenuRequest();
            characterManager.getCurrentCharacter()->setHealth(3);
            //stop game and level music
            
            levelManager.stopCurrentLevelMusic();

            //restart menu music
            menu.resumeMusic();

            int score = calculateScore();
            if (score > 0) {
                menu.addScore(score);
            }
        }

        if (menu.shouldExit()) {
            window.close();
        }
    }

    void updateGame() {
        if (isGameOver) {
            //handle game over state
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                isGameRunning = false;
                characterManager.getCurrentCharacter()->setHealth(3);
                //stop game and level music
                menu.stopMusic();
                levelManager.stopCurrentLevelMusic();

                //restart menu music
                menu.resumeMusic();

                menu.addScore(calculateScore());
            }
        }
        else if (isLevelCompleted) {
            handleLevelTransition();
        }
        else {
            update();

            //check if player health is zero
            Character* currentCharacter = characterManager.getCurrentCharacter();
            if (currentCharacter->getHealth() <= 0) {
                isGameOver = true;
                currentCharacter->setHealth(3);
            }
        }
    }

    void handleLevelTransition() {
        levelTransitionTimer += deltaTime;

        if (levelTransitionTimer >= LEVEL_TRANSITION_DELAY) {
            levelTransitionTimer = 0.0f;

            if (levelManager.advanceToNextLevel()) {
                characterManager.resetPositions(100.0f, 100.0f);
                isLevelCompleted = false;

                // Sync music settings for new level
                menu.syncMusicSettings();
            }
            else {
                //game completed
                isGameRunning = false;

                //stop game and level music
                menu.stopMusic();
                levelManager.stopCurrentLevelMusic();

                //restart menu music
                menu.resumeMusic();

                menu.addScore(calculateScore());
                characterManager.getCurrentCharacter()->setHealth(3);
            }
        }
    }

    int calculateScore() {
        int collectibleScore = Collectible::getScore();
        return collectibleScore;
    }

    void update() {
        Character* currentCharacter = characterManager.getCurrentCharacter();
        Level* currentLevel = levelManager.getCurrentLevel();

        float prevY = currentCharacter->getY();

        currentCharacter->handleInput(currentLevel);
        currentCharacter->update(deltaTime, currentLevel, &camera);
        currentCharacter->recordFollowPosition();
        currentCharacter->checkCollisonWithEnemy(currentLevel->getEnemy(), currentLevel->getEnemyCount(), prevY);
        currentCharacter->checkCollectibles(currentLevel);

        characterManager.updateFollowers(currentCharacter, deltaTime, currentLevel, &camera);

        currentLevel->updateCollectibles(deltaTime);
        camera.update(currentCharacter->getX(), currentCharacter->getY());
        currentLevel->updateEnemies(currentCharacter->getX(), currentCharacter->getY(), deltaTime);

        currentCharacter->checkBulletCollisions(currentLevel->getEnemy(), currentLevel->getEnemyCount());
        characterManager.checkFollowersBulletCollisions(currentLevel->getEnemy(), currentLevel->getEnemyCount());

        int currentLevelWidth = levelManager.getCurrentLevelWidth();
        float levelEndX = currentLevelWidth * 64 * LEVEL_COMPLETION_THRESHOLD;

        if (currentCharacter->getX() >= levelEndX) {
            isLevelCompleted = true;
        }
    }

    void render() {
        window.clear();

        if (isGameRunning) {
            renderGame();
            // NEW: Render pause menu over the game if paused
            if (isGamePaused) {
                menu.render(window);
            }
        }
        else {
            menu.render(window);
        }

        window.display();
    }

    void renderGame() {
        Level* currentLevel = levelManager.getCurrentLevel();

        window.draw(currentLevel->getBackGroundSprite());
        currentLevel->drawObstacles(window);
        currentLevel->drawEnemy(window);
        currentLevel->drawCollectibles(window);
        characterManager.drawCharacters(window, &camera);

        //draw health display
        renderHealthDisplay();
        renderCurrentPlayer();

        //draw player name
        //renderPlayerInfo();

        if (isLevelCompleted) {
            renderLevelCompleteMessage();
        }

        if (isGameOver) {
            renderGameOverMessage();
        }
    }

    void renderCurrentPlayer() {
        // Draw the player panel sprite
        currentPLayer.setPosition(10, 830);
        currentPLayer.setScale(2.5f, 2.5f);
        window.draw(currentPLayer);

        // Get current character ID to determine arrow position
        int currentCharacterId = characterManager.getCurrentCharacter()->getId();

        // Position arrow based on character ID
        float baseX = 15; // Starting X position for first character
        float characterSpacing = 60; // Distance between characters in the panel

        float arrowX = baseX + ((currentCharacterId - 1) * characterSpacing);
        float arrowY = 830; // Above the character panel

        circle.setPosition(arrowX, arrowY);
        window.draw(circle);
    }

    void renderPlayerInfo() {
        //display player name on screen
        sf::Text playerNameText;
        playerNameText.setFont(font);
        playerNameText.setCharacterSize(25);
        playerNameText.setFillColor(sf::Color::Red);
        playerNameText.setOutlineColor(sf::Color::Black);
        playerNameText.setOutlineThickness(1);
        playerNameText.setPosition(window.getSize().x - 200, 20);
        playerNameText.setString("Player: " + std::string(menu.getPlayerName()));

        window.draw(playerNameText);
    }

    void renderHealthDisplay() {
        //display health on screen
        sf::Text healthText;
        healthText.setFont(font);
        healthText.setCharacterSize(30);
        healthText.setFillColor(sf::Color::Red);
        healthText.setOutlineColor(sf::Color::Black);
        healthText.setOutlineThickness(2);
        healthText.setPosition(20, 20);

        //get current character's health
        int health = characterManager.getCurrentCharacter()->getHealth();
        healthText.setString("HP: " + std::to_string(health));

        window.draw(healthText);

        //display score on screen
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(2);
        scoreText.setPosition(20, 60);

        //get current score
        int score = Collectible::getScore();
        scoreText.setString("Score: " + std::to_string(score));

        window.draw(scoreText);

        //display rings collected/required
        sf::Text ringsText;
        ringsText.setFont(font);
        ringsText.setCharacterSize(30);
        ringsText.setFillColor(sf::Color::Cyan);
        ringsText.setOutlineColor(sf::Color::Black);
        ringsText.setOutlineThickness(2);
        ringsText.setPosition(20, 100);

        Level* currentLevel = levelManager.getCurrentLevel();
        ringsText.setString("Rings: " + std::to_string(currentLevel->getRingCollected()) +
            "/" + std::to_string(currentLevel->getRingRequired()));

        window.draw(ringsText);

        //display current level number
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(30);
        levelText.setFillColor(sf::Color::Green);
        levelText.setOutlineColor(sf::Color::Black);
        levelText.setOutlineThickness(2);
        levelText.setPosition(20, 140);
        levelText.setString("Level: " + std::to_string(levelManager.getCurrentLevelNumber()));

        window.draw(levelText);
    }

    void renderLevelCompleteMessage() {
        sf::Text levelCompleteText;
        levelCompleteText.setFont(font);
        levelCompleteText.setCharacterSize(60);
        levelCompleteText.setFillColor(sf::Color::Yellow);
        levelCompleteText.setPosition(400, 300);
        levelCompleteText.setString("LEVEL COMPLETE!");
        window.draw(levelCompleteText);

        sf::Text continueText;
        continueText.setFont(font);
        continueText.setCharacterSize(30);
        continueText.setFillColor(sf::Color::White);
        continueText.setPosition(450, 400);
        continueText.setString("Loading next level...");
        window.draw(continueText);
    }

    void renderGameOverMessage() {
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(60);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(450, 300);
        gameOverText.setString("GAME OVER");

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setPosition(400, 380);
        scoreText.setString("Final Score: " + std::to_string(calculateScore()));

        sf::Text continueText;
        continueText.setFont(font);
        continueText.setCharacterSize(30);
        continueText.setFillColor(sf::Color::Red);
        continueText.setPosition(350, 450);
        continueText.setString("Press ENTER to return to menu");

        window.draw(gameOverText);
        window.draw(scoreText);
        window.draw(continueText);
    }
};