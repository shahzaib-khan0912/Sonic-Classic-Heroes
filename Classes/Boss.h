#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "enemy.h"


void drawHitBox(sf::RenderWindow& window, float x, float y, int width, int height, sf::Color color = sf::Color::Green) {
	sf::RectangleShape hitbox(sf::Vector2f(width, height));
	hitbox.setPosition(x, y);
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineColor(color);
	hitbox.setOutlineThickness(2.0f);
	window.draw(hitbox);
}
class Boss : public Enemy {
	sf::Texture attacking;
	Animation attack;

	bool isAttacking;
	bool movingUp;
	bool movingDown;
	bool collided;


public:
	Boss(float x, float y) {
		hp = 2;
		speed = 3.0f;
		this->x = x;
		this->y = y;
		collided = false;
		movingUp = false;
		movingDown = false;
		isActive = true;
		isAttacking = true;
		moveRight = true;
		moveLeft = false;
		width = 164;
		height = 128;
		setupAnimations();

	}


	void setupAnimations() override {
		movingLeft.loadFromFile("Data/enemies/BossL.png");
		movingRight.loadFromFile("Data/enemies/BossR.png");
		attacking.loadFromFile("Data/enemies/bossAttack.png");

		attack.setup(&attacking, 76, 73, 4, 0.08f, true);

		
		attack.applyToSprite(sprite);

		sprite.setScale(2.4f, 2.4f);
	}

	void draw(sf::RenderWindow& window, float cameraX, float cameraY) override {
		if (isActive) {
			sprite.setPosition(x - cameraX, y);
			window.draw(sprite);
		}
		drawHitBox(window, x, y, width, height);
	}

	void onPlayerCollision() override {
		if (hp <= 0) {
			isActive = false;
		}
		hp--;
	}

	void update(float playerX, float playerY, float deltaTime) override {
		if (!isActive) return;

		float dx = playerX - (x + 82);
		float dy = playerY - (y+64);
		float distance = sqrt(dx * dx + dy * dy);


		if (dx > 0) {
			moveRight = true;
			moveLeft = false;
		}
		else if (dx < 0) {
			moveRight = false;
			moveLeft = true;
		}

		if (distance > 0) {
			dx /= distance;
			dy /= distance;
			x += dx * speed;
			y += dy * speed;
		}
		updateAnimation(deltaTime);
		/*if (moveLeft) {
			sprite.setTexture(movingLeft);
		}
		else if(moveRight){
			sprite.setTexture(movingRight);
			
		}*/
		if (isAttacking) {
			attack.applyToSprite(sprite);
			attack.update(deltaTime);
		}
		sprite.setPosition(x, y);
		
	}

};