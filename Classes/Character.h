#pragma once
#include <SFML/Graphics.hpp>
#include "level.h"
#include "camera.h"
#include "animation.h"
#include "collectible.h"

class Character {
protected:
    sf::Sprite sprite;
    sf::Texture texture;

    //animation textures and states
    sf::Texture still;
    sf::Texture runRight;
    sf::Texture runLeft;
    sf::Texture sprintRight;
    sf::Texture sprintLeft;
    sf::Texture Jump;
    sf::Texture collideWallRight;
    sf::Texture collideWallLeft;
    sf::Texture hang;
    Texture hangL;
    sf::Texture sprintDash;
    Texture glide;
    Texture glideL;
    

    Animation Still;
    Animation runR;
    Animation runL;
    Animation sprintR;
    Animation sprintL;
    Animation j;
    Animation collideWallR;
    Animation collideWallL;
    Animation Hanging;
    Animation HangingL;
    Animation sprintDashing;
    Animation sprintDashingL;
    Animation gliding;
    Animation glidingL;
    

    int notMoving = 0;
    int RUNRight = 1;
    int RUNLeft = 2;
    int SPRINTRight = 3;
    int SPRINTLeft = 4;
    int JUMP = 5;
    int COLLIDEWallR = 6;
    int COLLIDEWallL = 7;

    int currentState;
    bool facingRight;

    float x, y;
    float velocityY;
    float gravity;
    float jumpStrength;
    bool isOnGround;
    static int health; //shared health for all characters

    float velocityX;       //current horizontal velocity
    float accel;        //how fast player speeds up
    float maxSpeed;     //maximum horizontal speed
    float friction;     //how fast player slows down
    bool movingRight;
    bool movingLeft;
    bool isInvincible;
    bool isActive;

    bool collision;
    ///////////////////////
    static const int FOLLOW_BUF_SIZE = 30;  //buffer length
    float followX[FOLLOW_BUF_SIZE];
    float followY[FOLLOW_BUF_SIZE];
    int bufHead = 0;
    bool isJumping;      //flag to track active jumps
    bool justJumped;
    bool isHanging;
    

    ////////////////////////////////////////////////////////////////////
    float invincibilityTimer;
    float boostTimer;
    bool hasBoost;
    int id;

    SoundBuffer pJumping;
    SoundBuffer collideE;
    SoundBuffer ringR;
    Sound pJump;
    Sound eCollide;
    Sound ringCollide;


    Animation blastAnim;
    Texture blast;
    Sprite blasting;
    //Animation blastAnim;
    bool isBlastActive;
    float blastX, blastY;
    sf::Clock blastClock;
    float blastDuration;


    SoundBuffer sprint;
    Sound s;
    bool downCollide;
    

public:
    Character() {
        isHanging = false;
        x = 200.0f;
        y = 100.0f;
        velocityY = 0.0f;
        gravity = 0.5f;
        jumpStrength = -20.0f;
        isOnGround = true;
        health = 3; //initialize health to 3
        velocityX = 0.0f;
        accel = 0.1f;
        maxSpeed = 15.0f;
        friction = 0.2f;
        movingLeft = false;
        movingRight = false;
        isActive = false;
        isInvincible = false;
        isJumping = false;
        justJumped = false;
        collision = false;

        invincibilityTimer = 0.0f;
        boostTimer = 0.0f;
        hasBoost = false;

        //animation related
        currentState = 0;
        facingRight = true;
        id = 0;
        pJumping.loadFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Jump.wav");
        pJump.setBuffer(pJumping);
        pJump.setVolume(30);
        collideE.loadFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Destroy.wav");
        eCollide.setBuffer(collideE);

        ringR.loadFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Ring.wav");
        ringCollide.setBuffer(ringR);


        isBlastActive = false;
        blastDuration = 0.5f; // 0.5 seconds blast animation
        blastX = 0;
        blastY = 0;

        blast.loadFromFile("Data/blast.png");
       
        blasting.setTexture(blast);
        blasting.setScale(2.5f, 2.5f);

        blastAnim.setup(&blast, 32, 32, 6, 0.08f, false);
        sprint.loadFromFile("D:/!Second semester/finalproject/Sprites/Sonic the Hedgehog CD 2011 - Sound Effects/Global/Release.wav");
        s.setBuffer(sprint);

        downCollide = false;
        
    }


    void resetHealth() {
        health = 0;
     }

    void startBlastAnimation(float enemyX, float enemyY) {
        isBlastActive = true;
        blastX = enemyX;
        blastY = enemyY;
        blastClock.restart();
        blastAnim.reset(); // Reset to first frame
        blastAnim.play();  // Start playing the animation
    }

    // Add this to update the blast animation:
    void updateBlastAnimation(float deltaTime) {
        if (isBlastActive) {
            blastAnim.update(deltaTime);

            // Apply current frame to blast sprite
            blastAnim.applyToSprite(blasting);

            // Check if blast animation should end
            // Either by time duration or if it's a non-looping animation that finished
            if (blastClock.getElapsedTime().asSeconds() >= blastDuration) {
                isBlastActive = false;
                blastAnim.pause(); // Stop the animation
            }
        }
    }

    virtual void setupAnimations() {
        //override in derived classes to load specific textures
    }

    void updateAnimation(float deltaTime) {
        //determine animation state based on character's movement
        int newState = currentState;

        // Fixed: Check hanging states first (priority conditions)
        if (isHanging && movingRight) {
            newState = 8;  // hanging right
        }
        else if (isHanging && movingLeft) {
            newState = 13; // hanging left
        }
        // Fixed: Check gliding states for air movement
        else if (!isOnGround && velocityY > 8 && movingRight) {
            newState = 11; // gliding down
        }
        else if (!isOnGround && velocityY > 8 && movingLeft) {
            newState = 12; // gliding up
        }
        // Regular jump state for moderate air movement
        else if (!isOnGround) {
            newState = 5;  // jump
        }
        // Fixed: Reorganized sprint conditions
        else if (velocityX > 12) {
            newState = 9;  // sprint dash right
        }
        else if (velocityX < -12) {
            newState = 10; // sprint dash left
        }
        else if (velocityX > 7) {
            newState = 3;  // sprint right
        }
        else if (velocityX < -7) {
            newState = 4;  // sprint left
        }
        // Regular running
        else if (velocityX > 0 && velocityX < 7) {
            newState = 1;  // run right
        }
        else if (velocityX < 0 && velocityX > -7) {
            newState = 2;  // run left
        }
        // Wall collision states
        else if (collision && movingRight) {
            newState = 6;  
        }
        else if (collision && movingLeft) {
            newState = 7;  
        }
        
        else {
            newState = 0;  
        }

        
        if (velocityX > 0.1f) {
            facingRight = true;
        }
        else if (velocityX < -0.1f) {
            facingRight = false;
        }

        //if state changed, switch animation
        if (newState != currentState) {
            currentState = newState;
        }

        //update the current animation
        if (currentState == 8) {
            Hanging.update(deltaTime);
            Hanging.applyToSprite(sprite);
        }
        else if (currentState == 13) {
            HangingL.update(deltaTime);
            HangingL.applyToSprite(sprite);
        }
        else if (isHanging && (!movingLeft && !movingRight)) {
            Hanging.update(deltaTime);
            Hanging.applyToSprite(sprite);
        }
        else if (currentState == 11) {
            gliding.update(deltaTime);
            gliding.applyToSprite(sprite);
        }
        else if (currentState == 12) {
            glidingL.update(deltaTime);
            glidingL.applyToSprite(sprite);
        }
        else if (currentState == 0) {
            Still.update(deltaTime);
            Still.applyToSprite(sprite);
        }
        else if (currentState == 1) {
            runR.update(deltaTime);
            runR.applyToSprite(sprite);
        }
        else if (currentState == 2) {
            runL.update(deltaTime);
            runL.applyToSprite(sprite);
        }
        else if (currentState == 3) {
            sprintR.update(deltaTime);
            sprintR.applyToSprite(sprite);
        }
        else if (currentState == 9) {
            sprintDashing.update(deltaTime);
            sprintDashing.applyToSprite(sprite);
        }
        else if (currentState == 4) {
            sprintL.update(deltaTime);
            sprintL.applyToSprite(sprite);
        }
        else if (currentState == 10) {
            sprintDashingL.update(deltaTime);
            sprintDashingL.applyToSprite(sprite);
        }
        else if (currentState == 5) {
            j.update(deltaTime);
            j.applyToSprite(sprite);
        }
        else if (currentState == 6) {
            collideWallR.update(deltaTime);
            collideWallR.applyToSprite(sprite);
        }
        else if (currentState == 7) {
            collideWallL.update(deltaTime);
            collideWallL.applyToSprite(sprite);
        }
    }

    virtual void jump() = 0;

    virtual bool isJustJumping() const { return justJumped; }

    virtual void moveRight() {
        velocityX += accel;
        if (velocityX > maxSpeed) velocityX = maxSpeed;
    }

    virtual void moveLeft() {
        velocityX -= accel;
        if (velocityX < -maxSpeed) velocityX = -maxSpeed;
    }

    virtual void handleInput(Level* currentLevel) {
        movingLeft = false;
        movingRight = false;
        justJumped = false;  //reset jump flag each frame
        collision = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movingLeft = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movingRight = true;

        //moving right
        if (movingRight) {
            moveRight();
        }
        //moving left
        else if (movingLeft) {
            moveLeft();
        }
        //no movement key pressed - apply friction
        else {
            if (velocityX > 0) {
                velocityX -= friction;
                if (velocityX < 0) velocityX = 0;
            }
            else if (velocityX < 0) {
                velocityX += friction;
                if (velocityX > 0) velocityX = 0;
            }
        }

        float newX = x + velocityX;
        int collisonType = currentLevel->checkCollisionWithWalls(newX, y, 64, 64,id);
        if (collisonType == 0) {
            x = newX;
        }
        else if (collisonType == 1 || collisonType == 4 || (collisonType == 2 && id !=2)) {
            velocityX = 0;
            collision = true;
        }
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
            justJumped = true;  //signal that we just jumped
            pJump.play();
            jump();
        }
    }

    virtual void update(float deltaTime, Level* currentLevel, Camera* camera) {
        velocityY += gravity;
        float newY = y + velocityY;
        apply_gravity(currentLevel);

        int collisonType = currentLevel->checkCollisionWithWalls(x, newY, 64, 64,3);
        if (collisonType == 0) {
            y = newY;
            isOnGround = false;
        }
        else if (collisonType == 1 || collisonType == 2) {
            if (velocityY > 0) isOnGround = true;
            velocityY = 0;
        }
        else if (collisonType == 4) {
            velocityY = -15.0f;
            setInvincible(true, 2.0f);
        }

        if (y >= 796.0f ) {
            //health = 0;
            //velocityY = 0.0f;
            //isOnGround = true;
        }
        //invinsible 
        if (isInvincible) {
            invincibilityTimer -= deltaTime;
            if (invincibilityTimer <= 0) {
                isInvincible = false;
            }
        }
        updateBlastAnimation(deltaTime);
        //update animation
        updateAnimation(deltaTime);

        camera->update(x, y);
    }

    virtual void draw(sf::RenderWindow& window, Camera* camera) {
        sprite.setPosition(x - camera->getX(), y);

        //flash effect for invincibility
        if (isInvincible) {
            float flashRate = 0.1f; //flash 10 times per second
            if (static_cast<int>(invincibilityTimer / flashRate) % 2 == 0) {
                window.draw(sprite);
            }
        }
        else {
            window.draw(sprite);
        }

        if (isBlastActive) {
            blasting.setPosition(blastX - camera->getX(), blastY - camera->getY());
            window.draw(blasting);
        }
    }

    float getX() const { return x; }
    float getY() const { return y; }

    bool collidesWith(const Enemy& enemy) const {
        float ex = enemy.getX();
        float ey = enemy.getY();
        float ew = enemy.getWidth();
        float eh = enemy.getHeight();

        return !(x + 64 < ex ||
            x > ex + ew ||
            y + 66 < ey ||
            y > ey + eh);
    }

    void checkCollisonWithEnemy(Enemy** enemy, int enemyCount, float prevY) {
        for (int i = 0; i < enemyCount; i++) {
            if (enemy[i]->getIsActive() && this->collidesWith(*enemy[i])) {
                eCollide.play();

                startBlastAnimation(enemy[i]->getX(), enemy[i]->getY());
                /*if (prevY + 64 < enemy[i]->getY()) {
                    
                    enemy[i]->onPlayerCollision();
                }
                else {
                    
                    if (!isInvincible) {
                        health--; 
                        setInvincible(true, 2.0f); 

                        
                        float knockbackDirection = (enemy[i]->getX() > x) ? -1.0f : 1.0f;
                        velocityX = knockbackDirection * 10.0f;
                        velocityY = -5.0f;
                    }
                }*/
                enemy[i]->triggerBlastAnimation();
                enemy[i]->onPlayerCollision();
            }
        }
    }

    //checking bullet collision
    void checkBulletCollisions(Enemy** enemies, int enemyCount) {
        if (isInvincible) return; //skip if character is invincible

        for (int i = 0; i < enemyCount; ++i) {
            if (enemies[i] && enemies[i]->getIsActive()) {
                //check if any of the enemy's bullets hit the player
                if (enemies[i]->checkBulletCollision(x, y, 64, 64)) {
                    //handle the collision - reduce health and trigger invincibility
                    health--;
                    setInvincible(true, 2.0f); //2 seconds of invincibility after being hit

                    //
                    // al: add a small knockback effect
                    velocityX = (enemies[i]->getX() > x) ? -2.0f : 2.0f;
                    velocityY = -5.0f;

                    break; //only process one bullet hit at a time
                }
            }
        }
    }

    virtual void applySpecialBoost() {
        
    }

    void checkCollectibles(Level* currentLevel) {
        Collectible** collectibles = currentLevel->getCollectibles();
        int collectibleCount = currentLevel->getCollectibleCount();

        for (int i = 0; i < collectibleCount; i++) {
            if (collectibles[i] && collectibles[i]->getIsActive()) {
                if (collectibles[i]->checkCollision(x, y, 64, 64)) {
                    //handle collectible based on type
                    int type = collectibles[i]->getType();

                    if (type == 1) { //ring
                        currentLevel->incrementRingCollected();
                        ringCollide.play();
                    }
                    else if (type == 2) { //extra life
                        health++; //increase health when collecting extra life
                    }
                    else if (type == 3) {
                        
                        applySpecialBoost();
                    }

                    collectibles[i]->onCollect();
                }
            }
        }
    }

    virtual void resetBoostEffects() {
        //base implementation - override in child classes
    }

    bool checkOutOfBounds(int row, int col_left, int col_mid, int col_right, int height, int width) {
        return (row < 0 || row >= height || col_left < 0 || col_left >= width ||
            col_mid < 0 || col_mid >= width || col_right < 0 || col_right >= width);
    }

    bool handleUpwardCollision(float offset_y, float x, int Pheight, int Pwidth, int hit_box_factor_y, int** lvl, int cell_size) {
        int row_top = (int)(offset_y + hit_box_factor_y) / cell_size;
        int row_above = row_top - 1;

        bool canCheckAbove = (row_above >= 0);

        int col_left = (int)(x) / cell_size;
        int col_mid = (int)(x + Pwidth / 2) / cell_size;
        int col_right = (int)(x + Pwidth) / cell_size;
        int cols[3] = { col_left, col_mid, col_right };

        bool hitWall = false;

        for (int i = 0; i < 3; i++) {
            int c = cols[i];

            bool allowCornerJump = false;
            for (int j = 0; j < 3; j++) {
                int col = cols[j];
                int colStartX = col * cell_size;
                int colEndX = (col + 1) * cell_size;

                int playerLeft = x;
                int playerRight = x + Pwidth;

                int overlapLeft = std::max(playerLeft, colStartX);
                int overlapRight = std::min(playerRight, colEndX);
                int overlapWidth = overlapRight - overlapLeft;

                float overlapRatio = (float)overlapWidth / (float)Pwidth;
                bool playerUnderThis = (overlapRatio >= 0.40f);

                if (lvl[row_top][col] == 1 && row_above >= 0 && lvl[row_above][col] == 0) {
                    int left = j > 0 ? cols[j - 1] : -1;
                    int right = j < 2 ? cols[j + 1] : -1;

                    bool leftEmpty = (left == -1) || (lvl[row_top][left] != 1);
                    bool rightEmpty = (right == -1) || (lvl[row_top][right] != 1);

                    if ((leftEmpty || rightEmpty) && playerUnderThis) {
                        allowCornerJump = true;
                        break;
                    }
                }
            }

            bool obstacleAbovePlayer = false;

            for (int j = 0; j < 3; j++) {
                int col = cols[j];
                int tileXStart = col * cell_size;
                int tileXEnd = (col + 1) * cell_size;

                bool overlapsHorizontally = (x + Pwidth > tileXStart) && (x < tileXEnd);

                if (overlapsHorizontally && row_above >= 0.10) {
                    int tileAbove = lvl[row_above][col];

                    if (tileAbove == 3 || tileAbove == 4 || tileAbove == 1) {
                        obstacleAbovePlayer = true;
                        break;
                    }
                }
            }

            if (lvl[row_top][c] == 1) {
                hitWall = true;

                if (allowCornerJump && !obstacleAbovePlayer && row_top > 0) {
                    y = row_top * cell_size - Pheight;
                    velocityY = 0;
                    isOnGround = true;
                    return true;
                }
            }
        }

        if (hitWall && !isOnGround) {
            velocityY = 0;
            return true;
        }

        return false;
    }

    bool handleDownwardCollision(float offset_y, int row, int col_left, int col_mid, int col_right, int** lvl) {
        int bottom_left_down = lvl[row][col_left];
        int bottom_mid_down = lvl[row][col_mid];
        int bottom_right_down = lvl[row][col_right];

        if (bottom_left_down == 1 || bottom_mid_down == 1 || bottom_right_down == 1 || bottom_left_down == 2 || bottom_mid_down == 2 || bottom_right_down == 2) {
            isOnGround = true;
            downCollide = true;
            return true;
        }
        else {
            isOnGround = false;
            downCollide = false;
            return false;
        }
    }

    void apply_gravity(Level* currentLevel) {
        float offset_y = y + velocityY;
        const int cell_size = 64;
        int hit_box_factor_x = 0;
        int hit_box_factor_y = 0;
        int Pheight = 64;
        int Pwidth = 64;
        int height = currentLevel->getRows();
        int width = currentLevel->getCols();
        float terminal_Velocity = 12.0f;

        int row = (int)(offset_y + hit_box_factor_y + Pheight) / cell_size;
        int col_left = (int)(x + hit_box_factor_x) / cell_size;
        int col_mid = (int)(x + hit_box_factor_x + Pwidth / 2) / cell_size;
        int col_right = (int)(x + hit_box_factor_x + Pwidth) / cell_size;

        bool out_of_bounds = checkOutOfBounds(row, col_left, col_mid, col_right, height, width);

        if (!out_of_bounds) {
            int** lvl = currentLevel->getLevelGrid();

            if (velocityY < 0) {
                if (handleUpwardCollision(offset_y, x, Pheight, Pwidth, hit_box_factor_y, lvl, cell_size)) {
                    return;
                }
            }

            if (handleDownwardCollision(offset_y, row, col_left, col_mid, col_right, lvl)) {
                velocityY = 0;
            }
            else {
                const float minY = 0;
                if (offset_y < minY) {
                    offset_y = minY;
                    velocityY = 0;
                }
                y = offset_y;
            }

            if (!isOnGround) {
                velocityY += gravity;
                if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
            }
            else {
                velocityY = 0;
            }
        }
        else {
            if (y > height) {
                x = 100;
                y = 100;
            }
            isOnGround = false;
        }
    }

    bool getIsActive() {
        return isActive;
    }
    void setIsActive(bool a) {
        isActive = a;
    }

    //////////////////////////

    void recordFollowPosition() {
        followX[bufHead] = x;
        followY[bufHead] = y;
        bufHead = (bufHead + 1) % FOLLOW_BUF_SIZE;
    }

    void followLeaderMirroring(Character* leader,
        float dt,
        Level* level,
        Camera* cam)
    {
        const float followRange = 550.0f;
        const float snapRange = 650.0f;
        float dx = leader->getX() - x;
        //get leader's collision state
        bool leaderColliding = leader->isColliding();

        if (fabs(dx) > snapRange) {
            x = leader->getX() - 10;
            y = leader->getY() - 10;
            velocityX = 0;
            velocityY = 0;
            isOnGround = leader->isOnGround;
            collision = false; //reset collision state when teleporting
        }
        else if (fabs(dx) < followRange) {
            //only copy leader's movement if they're not colliding with a wall
            if (!leaderColliding) {
                movingRight = leader->movingRight;
                movingLeft = leader->movingLeft;

                if (movingRight)       moveRight();
                else if (movingLeft)   moveLeft();
                else {
                    if (velocityX > 0) {
                        velocityX = std::max(0.0f, leader->getVelocityX() - friction);
                    }
                    else {
                        velocityX = std::min(0.0f, leader->getVelocityX() + friction);
                    }
                }

                //cap follower's velocity to leader's velocity
                float leaderVX = leader->getVelocityX();
                if (leaderVX > 0 && velocityX > leaderVX) {
                    velocityX = leaderVX;
                }
                else if (leaderVX < 0 && velocityX < leaderVX) {
                    velocityX = leaderVX;
                }
            }
            else {
                //leader is colliding, slow down follower
                if (velocityX > 0) {
                    velocityX -= friction * 2.0f;
                    if (velocityX < 0) velocityX = 0;
                }
                else if (velocityX < 0) {
                    velocityX += friction * 2.0f;
                    if (velocityX > 0) velocityX = 0;
                }
            }

            float newX = x + velocityX;
            if (!level->checkCollisionWithWalls(newX, y, 64, 64,id)) {
                x = newX;
                collision = false; //clear collision flag when no collision
            }
            else {
                velocityX = 0;
                collision = true; //set collision flag when hitting a wall
            }

            update(dt, level, cam);

            if (leader->isJustJumping() && isOnGround && !leaderColliding) {
                jump();
            }
        }
    }

    bool isColliding() const { return collision; }

    int getId() {
        return id;
    }

    void setActiveChar(bool a) {
        isActive = a;
    }

    float getVelocityX() {
        return velocityX;
    }

    int getHealth() {
        return health;
    }

    void setInvincible(bool invincible, float duration = 15.0f) {
        isInvincible = invincible;
        if (isInvincible) {
            invincibilityTimer = duration;
        }
    }

    bool getInvincible() {
        return isInvincible;
    }
    void setX(float x) {
        this->x = x;
    }
    void setY(float y) {
        this->y = y;
    }
    void setVelocityX(float x) {
        velocityX = x;
    }
    void setVelocityY(float y) {
        velocityY = y;
    }
    void setHealth(int h) {
        this->health = h;
    }
    void setIsHanging(bool a) {
        isHanging = a;
    }
    virtual ~Character() {
    }
};

//initialize static health variable
int Character::health = 69;
