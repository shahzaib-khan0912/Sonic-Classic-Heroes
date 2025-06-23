#pragma once
class Sonic : virtual public Character {
protected:
    //float maxSpeed;
    bool useAbility;

    // Speed boost animation components
    Texture SpeedBoost;
    Texture spinDash;
    Texture spinDashL;

    Animation speedUp;
    Sprite speedEffect;
    float speedEffectX;
    float speedEffectY;
    bool isSpeedBoostActive;
    bool wasSpeedBoostActiveLastFrame; // Track previous boost state

public:
    Sonic() : Character() {
        setupAnimations();
        sprite.setScale(2.0f, 2.40f);
        sprite.setOrigin(4, 14);
        maxSpeed = 15.0f;
        useAbility = false;
        id = 1;

        // Initialize speed boost animation
        SpeedBoost.loadFromFile("Data/speedBost.png"); // Add your speed boost texture
        speedEffect.setTexture(SpeedBoost);
        speedEffect.setScale(2.0f, 2.40f);
        speedEffect.setOrigin(26.5f, 24.5f); // Adjust based on your sprite size
        speedEffectX = 0;
        speedEffectY = 0;
        isSpeedBoostActive = false;
        wasSpeedBoostActiveLastFrame = false;
        speedUp.setup(&SpeedBoost, 19, 25, 5, 0.09f, true); // Adjust dimensions as needed
    }

    void setupAnimations() override {
        //load textures
        still.loadFromFile("Data/sonic/show.png");
        runRight.loadFromFile("Data/sonic/0right.png");
        runLeft.loadFromFile("Data/sonic/0left.png");
        sprintRight.loadFromFile("Data/sonic/sonicsprintR.png");
        sprintLeft.loadFromFile("Data/sonic/sonicsprintL.png");
        Jump.loadFromFile("Data/sonic/0upR.png");
        collideWallRight.loadFromFile("Data/sonic/0pushR.png");
        collideWallLeft.loadFromFile("Data/sonic/0pushL.png");
        hang.loadFromFile("Data/sonic/hang.png");
        hangL.loadFromFile("Data/sonic/hangL.png");
        spinDash.loadFromFile("Data/sonic/spinDash.png");
        spinDashL.loadFromFile("Data/sonic/spinDashL.png");

        //set up idle animation (example values - adjust based on your sprite sheets)
        Still.setup(&still, 49, 40, 24, 0.15f, true);
        //set up run animation
        runR.setup(&runRight, 40, 40, 12, 0.08f, true);
        runL.setup(&runLeft, 40, 40, 12, 0.08f, true);
        sprintR.setup(&sprintRight, 40, 40, 8, 0.008f, true);
        sprintL.setup(&sprintLeft, 40, 40, 8, 0.008f, true);
        j.setup(&Jump, 40, 40, 8, 0.08f, true);
        collideWallR.setup(&collideWallRight, 40, 40, 10, 0.08f, true);
        collideWallL.setup(&collideWallLeft, 40, 40, 10, 0.08f, true);
        Hanging.setup(&hang, 49, 44, 8, 0.08f, true);
        HangingL.setup(&hangL, 49, 44, 8, 0.08f, true);
        //set up jump animation
        gliding.setup(&Jump, 40, 40, 8, 0.08f, true);
        glidingL.setup(&Jump, 40, 40, 8, 0.08f, true);
        sprintDashing.setup(&spinDash, 49, 40, 5, 0.08f, true);
        sprintDashingL.setup(&spinDashL, 49, 40, 5, 0.08f, true);
        //start with idle animation
        Still.applyToSprite(sprite);
    }
    
    void update(float deltaTime, Level* currentLevel, Camera* camera) override {
        // Handle speed boost animation
        if (wasSpeedBoostActiveLastFrame) {
            isSpeedBoostActive = true;

            // Only start animation if we just activated boost
            if (!wasSpeedBoostActiveLastFrame) {
                speedUp.reset();
                speedUp.play();
                velocityX *= 4.0f;
            }

            // Update speed effect position to follow character
            speedEffectX = x + (sprite.getGlobalBounds().width / 2); // Center on character
            speedEffectY = y + (sprite.getGlobalBounds().height) - 10 ;

            // Update the animation
            speedUp.update(deltaTime);
            speedUp.applyToSprite(speedEffect);
            
        }
        else {
            isSpeedBoostActive = false;
            speedUp.pause();
        }

        // Store current boost state for next frame
        wasSpeedBoostActiveLastFrame = (isSpeedBoostActive);

        // Call parent update
        Character::update(deltaTime, currentLevel, camera);
    }

    void jump() override {
        velocityY = -18.0;
        isOnGround = false;
        isJumping = true;
        j.play();
    }

    bool isJustJumping() const { return justJumped; }

    void applySpecialBoost() override {
        hasBoost = true;
        boostTimer = 10.0f;
        maxSpeed += 4.0f;
        // Increase default max speed by +4
        isSpeedBoostActive = true;
    }

    void resetBoostEffects() override {
        maxSpeed = 15.0f; // Reset to default speed
        isSpeedBoostActive = false;
    }

    // Get speed effect sprite for external rendering (if needed)
    sf::Sprite* getSpeedEffectSprite() {
        if (isSpeedBoostActive && hasBoost && boostTimer > 0) {
            return &speedEffect;
        }
        return nullptr;
    }

    // Optional: Add render method if your game uses individual render calls
    void draw(sf::RenderWindow& window,Camera* camera)override {
        Character::draw(window, camera);
        // Render the character first
        window.draw(sprite);

        // Render the speed effect if active
        if (isSpeedBoostActive) {
            speedEffect.setPosition(speedEffectX -camera->getX(), speedEffectY);
            window.draw(speedEffect);
        }
    }
};