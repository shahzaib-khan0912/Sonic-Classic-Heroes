#pragma once
#pragma warning(disable:4996)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstring>
#include <fstream> // Added for file handling

class Menu {
private:
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackground;
    sf::Texture optionsBackgroundTexture;
    sf::Sprite optionsBackground;
    sf::Texture leaderboardBackgroundTexture;
    sf::Sprite leaderboardBackground;
    sf::Texture levelSelectBackgroundTexture;
    sf::Sprite levelSelectBackground;
    sf::Texture pauseBackgroundTexture;  // NEW: Pause menu background
    sf::Sprite pauseBackground;          // NEW: Pause menu background sprite
    sf::Font font;
    sf::Text menuText[5]; // New Game, Options, Leaderboard, Level, Exit
    sf::Text levelText[4]; // Level 1, Level 2, Level 3
    sf::Text pauseText[4]; // NEW: Resume, Options, Main Menu, Exit
    sf::Text nameText;
    sf::Text namePrompt;
    sf::Text highScores[5];
    int selectedItem; // Current menu or level selection
    int menuState; // 0=main, 1=name, 2=options, 3=leaderboard, 4=level, 5=pause
    bool musicOn;
    float musicVolume;
    char playerName[20];
    int scores[5];
    char scoreNames[5][20];
    bool startGame;
    bool exitRequested; // New flag for exit
    bool resumeRequested; // NEW: Flag for resume game
    bool returnToMainMenu; // NEW: Flag for return to main menu
    sf::Music menuMusic;
    int chosenLevel;
    LevelManager* levelManager = nullptr;
    bool paused;
    SoundBuffer select;
    Sound sel;

    // File handling methods
    void loadLeaderboard() {
        std::ifstream file("Data/leaderboard.txt");
        if (file.is_open()) {
            for (int i = 0; i < 5; i++) {
                if (file >> scores[i]) {
                    file.ignore(); // ignore the space/newline
                    file.getline(scoreNames[i], 20);
                }
                else {
                    // If file doesn't have enough entries, initialize with empty values
                    scores[i] = 0;
                    strcpy(scoreNames[i], "");
                }
            }
            file.close();
        }
        else {
            // If file doesn't exist, initialize with empty values
            for (int i = 0; i < 5; i++) {
                scores[i] = 0;
                strcpy(scoreNames[i], "");
            }
        }
        updateLeaderboardDisplay();
    }

    void saveLeaderboard() {
        std::ofstream file("Data/leaderboard.txt");
        if (file.is_open()) {
            for (int i = 0; i < 5; i++) {
                file << scores[i] << " " << scoreNames[i] << std::endl;
            }
            file.close();
        }
        else {
            std::cout << "Failed to save leaderboard!" << std::endl;
        }
    }

    void updateLeaderboardDisplay() {
        for (int i = 0; i < 5; i++) {
            if (scores[i] > 0 && strlen(scoreNames[i]) > 0) {
                highScores[i].setString(std::to_string(i + 1) + ". " + scoreNames[i] + ": " + std::to_string(scores[i]));
            }
            else {
                highScores[i].setString(std::to_string(i + 1) + ". ---");
            }
        }
    }

public:
    Menu() : selectedItem(0), menuState(0), musicOn(true), musicVolume(50.0f),
        startGame(false), exitRequested(false), resumeRequested(false),
        returnToMainMenu(false), chosenLevel(1) {
        playerName[0] = '\0';
        initialize();
        select.loadFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Select.wav");
        sel.setBuffer(select);
        paused = false;
    }

    void setLevelManager(LevelManager* manager) {
        levelManager = manager;
    }

    void initialize() {
        // Load backgrounds
        if (!menuBackgroundTexture.loadFromFile("Data/background.png")) {
            std::cout << "Failed to load menu background!" << std::endl;
        }
        menuBackground.setTexture(menuBackgroundTexture);
        menuBackground.setScale(1.2f, 0.9f);

        if (!optionsBackgroundTexture.loadFromFile("Data/option.png")) {
            std::cout << "Failed to load options background!" << std::endl;
        }
        optionsBackground.setTexture(optionsBackgroundTexture);
        optionsBackground.setScale(1.2f, 0.9f);

        if (!leaderboardBackgroundTexture.loadFromFile("Data/leader.png")) {
            std::cout << "Failed to load leaderboard background!" << std::endl;
        }
        leaderboardBackground.setTexture(leaderboardBackgroundTexture);
        leaderboardBackground.setScale(1.2f, 0.9f);

        if (!levelSelectBackgroundTexture.loadFromFile("Data/level.png")) {
            std::cout << "Failed to load level select background!" << std::endl;
        }
        levelSelectBackground.setTexture(levelSelectBackgroundTexture);
        levelSelectBackground.setScale(1.2f, 0.9f);

        // NEW: Load pause background (you can reuse an existing one or create a new one)
        if (!pauseBackgroundTexture.loadFromFile("Data/background.png")) {
            std::cout << "Failed to load pause background!" << std::endl;
        }
        pauseBackground.setTexture(pauseBackgroundTexture);
        pauseBackground.setScale(1.2f, 0.9f);

        // Load font
        if (!font.loadFromFile("Data/Fonts/text.otf")) {
            std::cout << "Failed to load font!" << std::endl;
        }

        // Setup menu items
        for (int i = 0; i < 5; i++) {
            menuText[i].setFont(font);
            menuText[i].setCharacterSize(40);
            menuText[i].setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::Red);
            menuText[i].setPosition(490, 600 + i * 50);
        }
        menuText[0].setString("New Game");
        menuText[1].setString("Options");
        menuText[2].setString("Leaderboard");
        menuText[3].setString("Levels");
        menuText[4].setString("Quit");

        // Setup level selection
        for (int i = 0; i < 3; i++) {
            levelText[i].setFont(font);
            levelText[i].setCharacterSize(40);
            levelText[i].setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::Red);
            levelText[i].setPosition(520, 360 + i * 60);
            levelText[i].setString("Level " + std::to_string(i + 1));
        }

        levelText[3].setFont(font);
        levelText[3].setCharacterSize(40);
        levelText[3].setFillColor(3 == 0 ? sf::Color::Yellow : sf::Color::Red);
        levelText[3].setPosition(490, 350 + 3 * 60);
        levelText[3].setString("BOSS");

        // NEW: Setup pause menu items
        for (int i = 0; i < 4; i++) {
            pauseText[i].setFont(font);
            pauseText[i].setCharacterSize(40);
            pauseText[i].setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::Red);
            pauseText[i].setPosition(490, 600 + i * 60);
        }
        pauseText[0].setString("Resume Game");
        pauseText[1].setString("Options");
        pauseText[2].setString("Main Menu");
        pauseText[3].setString("Exit Game");

        // Setup name input
        nameText.setFont(font);
        nameText.setCharacterSize(30);
        nameText.setFillColor(sf::Color::Red);
        nameText.setPosition(400, 650);
        namePrompt.setFont(font);
        namePrompt.setCharacterSize(40);
        namePrompt.setFillColor(sf::Color::Yellow);
        namePrompt.setPosition(400, 580);
        namePrompt.setString("Enter Your Name:");
        updateNameDisplay();

        // Setup high scores - Load from file instead of hardcoded values
        for (int i = 0; i < 5; i++) {
            highScores[i].setFont(font);
            highScores[i].setCharacterSize(30);
            highScores[i].setFillColor(sf::Color::Red);
            highScores[i].setPosition(400, 300 + i * 40);
        }

        // Load leaderboard from file
        loadLeaderboard();

        // Load music
        if (!menuMusic.openFromFile("Data/super.mp3")) {
            std::cout << "Failed to load menu music!" << std::endl;
        }
        menuMusic.setLoop(true);
        menuMusic.setVolume(musicVolume);
        menuMusic.play();
    }

    void updateNameDisplay() {
        char displayStr[30] = "Player Name: ";
        int len = strlen(playerName);
        if (len == 0) {
            strcat(displayStr, "_");
        }
        else {
            strcat(displayStr, playerName);
        }
        if (menuState == 1) {
            strcat(displayStr, "|");
        }
        nameText.setString(displayStr);
    }

    // NEW: Method to show pause menu during gameplay
    void showPauseMenu() {
        menuState = 5; // Set to pause state
        selectedItem = 0; // Reset selection to first item (Resume)
        updatePauseSelection();
    }

    void handleEvent(sf::Event event) {
        if (event.type != sf::Event::KeyPressed) {
            if (menuState == 1 && event.type == sf::Event::TextEntered) {
                handleNameInput(event);
            }
            return;
        }

        sf::Keyboard::Key key = event.key.code;
        if (menuState == 0) { // Main menu
            if (key == sf::Keyboard::Up) {
                sel.play();
                selectedItem = (selectedItem - 1 + 5) % 5;
                updateMenuSelection();
            }
            else if (key == sf::Keyboard::Down) {
                sel.play();
                selectedItem = (selectedItem + 1) % 5;
                updateMenuSelection();
            }
            else if (key == sf::Keyboard::Enter) {
                sel.play();
                if (selectedItem == 0) {
                    menuState = 1; // Name entry
                    playerName[0] = '\0';
                    updateNameDisplay();
                    chosenLevel = 1; // Default to level 1 for New Game
                }
                else if (selectedItem == 1) {
                    menuState = 2; // Options
                }
                else if (selectedItem == 2) {
                    menuState = 3; // Leaderboard
                }
                else if (selectedItem == 3) {
                    menuState = 4; // Level select
                    selectedItem = 0;
                    updateMenuSelection();
                    updateLevelSelection();
                }
                else if (selectedItem == 4) {
                    exitRequested = true; // Signal exit
                }
            }
        }
        else if (menuState == 1) { // Name entry
            if (key == sf::Keyboard::Escape) {
                menuState = 0;
            }
            else if (key == sf::Keyboard::Enter && playerName[0] != '\0') {
                menuState = 0;
                startGame = true;
                menuMusic.stop();
            }
        }
        else if (menuState == 2) { // Options
            if (key == sf::Keyboard::Escape && paused) {
                menuState = 5;
            }
            else if (key == sf::Keyboard::Escape) {
                menuState = 0;
            }
            else if (key == sf::Keyboard::M) {
                musicOn = !musicOn;
                // Control menu music
                if (musicOn) {
                    menuMusic.play();
                }
                else {
                    menuMusic.pause();
                }
                // Control level music through LevelManager
                if (levelManager) {
                    levelManager->setMusicEnabled(musicOn);
                }
            }
            else if (key == sf::Keyboard::Left && musicVolume > 0) {
                musicVolume -= 5;
                menuMusic.setVolume(musicVolume);
                // Also adjust level music volume through LevelManager
                if (levelManager) {
                    levelManager->setMusicVolume(musicVolume);
                }
            }
            else if (key == sf::Keyboard::Right && musicVolume < 100) {
                musicVolume += 5;
                menuMusic.setVolume(musicVolume);
                // Also adjust level music volume through LevelManager
                if (levelManager) {
                    levelManager->setMusicVolume(musicVolume);
                }
            }
        }
        else if (menuState == 3) { // Leaderboard
            if (key == sf::Keyboard::Escape) {
                menuState = 0;
            }
        }
        else if (menuState == 4) { // Level select
            if (key == sf::Keyboard::Up) {
                sel.play();
                selectedItem = (selectedItem - 1 + 4) % 4;
                updateLevelSelection();
            }
            else if (key == sf::Keyboard::Down) {
                sel.play();
                selectedItem = (selectedItem + 1) % 4;
                updateLevelSelection();
            }
            else if (key == sf::Keyboard::Enter) {
                sel.play();
                chosenLevel = selectedItem + 1;
                menuState = 1; // Go to name entry
                playerName[0] = '\0';
                updateNameDisplay();
            }
            else if (key == sf::Keyboard::Escape) {
                sel.play();
                menuState = 0;
            }
        }
        // NEW: Handle pause menu events
        else if (menuState == 5) { // Pause menu
            if (key == sf::Keyboard::Up) {
                sel.play();
                selectedItem = (selectedItem - 1 + 4) % 4;
                updatePauseSelection();
            }
            else if (key == sf::Keyboard::Down) {
                sel.play();
                selectedItem = (selectedItem + 1) % 4;
                updatePauseSelection();
            }
            else if (key == sf::Keyboard::Enter) {
                sel.play();
                if (selectedItem == 0) {
                    resumeRequested = true; // Resume game
                }
                else if (selectedItem == 1) {
                    menuState = 2; // Options (reuse existing options)
                }
                else if (selectedItem == 2) {
                    updateMenuSelection();
                    menuState = 0;
                    returnToMainMenu = true;
                    resetGame();// Return to main menu
                    
                }
                else if (selectedItem == 3) {
                    exitRequested = true; // Exit game
                    resetStartGameRequest();
                    resetGame();
                }
            }
            else if (key == sf::Keyboard::Escape) {
                resumeRequested = true; // ESC also resumes game
            }
        }
    }

    void handleNameInput(sf::Event event) {
        if (event.text.unicode == '\b') {
            int len = strlen(playerName);
            if (len > 0) {
                playerName[len - 1] = '\0';
            }
        }
        else if (event.text.unicode < 128) {
            int len = strlen(playerName);
            if (len < 19) {
                char c = (char)event.text.unicode;
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                    (c >= '0' && c <= '9') || c == ' ') {
                    playerName[len] = c;
                    playerName[len + 1] = '\0';
                }
            }
        }
        updateNameDisplay();
    }

    void updateMenuSelection() {
        for (int i = 0; i < 5; i++) {
            menuText[i].setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::Red);
        }
    }

    void updateLevelSelection() {
        for (int i = 0; i < 4; i++) {
            levelText[i].setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::Red);
        }
    }

    // NEW: Update pause menu selection
    void updatePauseSelection() {
        for (int i = 0; i < 4; i++) {
            pauseText[i].setFillColor(i == selectedItem ? sf::Color::Yellow : sf::Color::Red);
        }
    }

    void render(sf::RenderWindow& window) {
        if (menuState == 1) {
            window.draw(menuBackground);
            window.draw(namePrompt);
            window.draw(nameText);

        }
        else if (menuState == 2) {
            window.draw(optionsBackground);
            sf::Text options;
            options.setFont(font);
            options.setCharacterSize(40);
            options.setFillColor(sf::Color::Yellow);
            options.setPosition(500, 300);
            char buffer[100];
            sprintf(buffer, "Options\n\nM: Music (%s)\nLeft/Right: Volume (%d)\n\nPress ESC",
                musicOn ? "ON" : "OFF", (int)musicVolume);
            options.setString(buffer);
            window.draw(options);
        }
        else if (menuState == 3) {
            window.draw(leaderboardBackground);
            sf::Text title;
            title.setFont(font);
            title.setCharacterSize(50);
            title.setFillColor(sf::Color::Black);
            title.setPosition(500, 200);
            title.setString("HIGH SCORES");
            window.draw(title);
            for (int i = 0; i < 5; i++) {
                window.draw(highScores[i]);
            }

        }
        else if (menuState == 4) {
            window.draw(levelSelectBackground);
            sf::Text title;
            title.setFont(font);
            title.setCharacterSize(50);
            title.setFillColor(sf::Color::Black);
            title.setPosition(450, 300);
            title.setString("SELECT LEVEL");
            window.draw(title);
            for (int i = 0; i < 4; i++) {
                window.draw(levelText[i]);
            }

        }
        // NEW: Render pause menu
        else if (menuState == 5) {
            window.draw(pauseBackground);
            for (int i = 0; i < 4; i++) {
                window.draw(pauseText[i]);
            }


        }
        else if (exitRequested) {
            window.close();
        }
        else {
            window.draw(menuBackground);
            sf::Text title;
            title.setFont(font);
            title.setCharacterSize(60);
            title.setFillColor(sf::Color::Black);
            title.setPosition(390, 200);
            title.setString("SONIC ClASSIC HEROS");
            title.setScale(0.7f, 0.8f);
            window.draw(title);
            for (int i = 0; i < 5; i++) {
                window.draw(menuText[i]);
            }
        }
    }

    const char* getPlayerName() { return playerName; }
    bool shouldStartGame() { return startGame; }
    void resetStartGameRequest() { startGame = false; }
    int getChosenLevel() { return chosenLevel; }
    bool shouldExit() { return exitRequested; }

    // NEW: Methods for pause menu functionality
    bool shouldResume() { return resumeRequested; }
    void resetResumeRequest() { resumeRequested = false; }
    bool shouldReturnToMainMenu() { return returnToMainMenu; }
    void resetReturnToMainMenuRequest() { returnToMainMenu = false; }
    bool isPaused() { return menuState == 5; }
    void setAsPaused(bool a) {
        paused = a;
    }

    void stopMusic() { menuMusic.stop(); }

    void resumeMusic() {
        if (musicOn) {
            menuMusic.play();
        }
    }

    void syncMusicSettings() {
        if (levelManager) {
            levelManager->setMusicVolume(musicVolume);
            levelManager->setMusicEnabled(musicOn);
        }
    }


    void resetGame() {
        Collectible::resetScore();
        
    }

    void addScore(int score) {
        int pos = -1;
        for (int i = 0; i < 5; i++) {
            if (score > scores[i]) {
                pos = i;
                break;
            }
        }
        if (pos != -1) {
            for (int i = 4; i > pos; i--) {
                scores[i] = scores[i - 1];
                strcpy(scoreNames[i], scoreNames[i - 1]);
            }
            scores[pos] = score;
            if (playerName[0] == '\0') {
                strcpy(scoreNames[pos], "Unknown");
            }
            else {
                strcpy(scoreNames[pos], playerName);
            }
            updateLeaderboardDisplay();
            saveLeaderboard(); // Save to file after adding new score
        }
    }
};