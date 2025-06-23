#pragma once
class Knuckles : virtual public Character {
protected:
    bool isTouchingBreakAbleWall;
    bool wasGliding;

    Texture glideSlide;
    Animation glideSliding;

    Animation punching;
    Texture punch;

    Texture Sheild;
    Animation sheildUP;
    Sprite she;
    float sheildX;
    float sheildY;
    bool isSheildUp;
    bool wasInvincibleLastFrame; // Track previous invincible state

public:
    Knuckles() : Character() {
        //texture.loadFromFile("Data/knuckles/knuckles.png");
        setupAnimations();
        sprite.setScale(1.9f, 2.90f);
        sprite.setOrigin(0, 17);
        maxSpeed = 12.0f;
        isTouchingBreakAbleWall = false;
        //maxFlightTime = 8.0f;
        isInvincible = false;
        //number = 0;
        id = 2;
        wasGliding = false;
        Sheild.loadFromFile("Data/sheild.png");
        she.setTexture(Sheild);
        she.setScale(2.2f, 3.3f);
        // Set shield origin to center it properly
        //she.setOrigin(28.5f, 22.5f); // Adjust based on your shield sprite size
        sheildX = 0;
        sheildY = 0;
        isSheildUp = false;
        wasInvincibleLastFrame = false;
        sheildUP.setup(&Sheild, 53, 49, 3, 0.09f, true); // Changed to loop=true
    }

    void setupAnimations() override {
        //Load textures
        still.loadFromFile("Data/knuckles/relax.png");
        runRight.loadFromFile("Data/knuckles/jog.png");
        runLeft.loadFromFile("Data/knuckles/KnucklesjogL.png");
        sprintRight.loadFromFile("Data/knuckles/sprint.png");
        sprintLeft.loadFromFile("Data/knuckles/sprintL.png");
        Jump.loadFromFile("Data/knuckles/jump.png");
        collideWallRight.loadFromFile("Data/knuckles/hitwall.png");
        collideWallLeft.loadFromFile("Data/knuckles/hitwallL.png");
        hang.loadFromFile("Data/knuckles/hang.png");
        hangL.loadFromFile("Data/knuckles/hangL.png");
        glide.loadFromFile("Data/knuckles/g.png");
        glideL.loadFromFile("Data/knuckles/gL.png");
        glideSlide.loadFromFile("Data/knuckles/glideSlide.png");
        punch.loadFromFile("Data/knuckles/punch.png");

        //hang.loadFromFile("Data/knuckles/hang.png");
        //set up idle animation (example values - adjust based on your sprite sheets)
        Still.setup(&still, 49, 40, 7, 0.08f, true);
        //set up run animation
        runR.setup(&runRight, 49, 40, 10, 0.06f, true);
        runL.setup(&runLeft, 48.9, 40, 10, 0.05f, true);
        sprintR.setup(&sprintRight, 49, 40, 6, 0.08f, true);
        sprintL.setup(&sprintLeft, 49, 40, 6, 0.08f, true);
        j.setup(&Jump, 49, 40, 10, 0.00002f, true);
        collideWallR.setup(&collideWallRight, 40, 40, 10, 0.08f, true);
        collideWallL.setup(&collideWallLeft, 39, 40, 10, 0.08f, true);
        Hanging.setup(&hang, 49, 49, 8, 0.08f, true);
        HangingL.setup(&hangL, 49, 49, 8, 0.08f, true);
        gliding.setup(&glide, 48, 48, 3, 0.08f, true);
        glidingL.setup(&glideL, 48, 48, 3, 0.08f, true);
        glideSliding.setup(&glideSlide, 64, 51, 8, 0.008f, true);
        punching.setup(&punch, 50, 50, 10, 0.08f, true);
        //hanging.setup(&hang,49,49,8,0.08f,true);
        //set up jump animation
        //start with idle animation
        Still.applyToSprite(sprite);
    }

    void update(float deltaTime, Level* currentLevel, Camera* camera) override {
        if (velocityY > 8) {
            gravity = 0.06f;
            if (movingRight)
                velocityX += 0.4;
            else
                velocityX -= 0.4;
            //x += velocityX;
            wasGliding = true;
        }
        else {
            gravity = 0.5f;
            //wasGliding = false;
        }
        
        // Handle shield animation based on invincibility
        if (isInvincible) {
            isSheildUp = true;

            // Only start animation if we just became invincible
            if (!wasInvincibleLastFrame) {
                sheildUP.reset();
                sheildUP.play();
            }

            // Update shield position to follow character
            sheildX = x-15; // Center on character
            sheildY = y-64;

            // Update the animation
            sheildUP.update(deltaTime);
            sheildUP.applyToSprite(she);
            
        }
        else {
            isSheildUp = false;
            sheildUP.pause();
        }

        // Store current invincible state for next frame
        wasInvincibleLastFrame = isInvincible;

        if (wasGliding && downCollide) {
            glideSliding.update(deltaTime);
            glideSliding.applyToSprite(sprite);
            wasGliding = false;
        }

        int tileX = (int)x / 64;
        int tileY = (int)y / 64;

        if (currentLevel->checkCollisionWithWalls(x, y, 64, 64, 2) == 2) {
            punching.update(deltaTime);
            punching.applyToSprite(sprite);
        }

        Character::update(deltaTime, currentLevel, camera);
    }

    void jump() override {
        velocityY = -18.0f;
        isOnGround = false;
        isJumping = true;
    }

    bool isJustJumping() const { return justJumped; }

    void applySpecialBoost() override {
        setInvincible(true, 15.0f);
        isSheildUp = true;
    }

    

    void reset() {
        x = 200;
        y = 200;
    }

    // Add a render method to draw the shield
    void draw(sf::RenderWindow& window,Camera* camera)override {
        Character::draw(window, camera);
        // Render the character first
        //window.draw(sprite);

        // Render the shield if it's up
        if (isSheildUp && isInvincible) {
            she.setPosition(sheildX - camera->getX(), sheildY);
            window.draw(she);
        }
    }
};