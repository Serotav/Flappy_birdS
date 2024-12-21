#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Global_const.h"
#include "Neuralnet.h"

//>> <<
class gameBird {

	int height;
	int x_position;

	float speed;
	sf::CircleShape birdBody;
	neural* brain;
public: bool isAlive = true;


public:
	gameBird() {height = screenLenght / 2; speed = 0; x_position = radius + 5; ;  birdBody.setRadius(radius); birdBody.setFillColor(sf::Color(ranzodmizeColor(), ranzodmizeColor(), ranzodmizeColor())); birdBody.setOrigin(radius, radius); }
	int rAltezza() const { return this->height; }
	int rPosizione() const { return this->x_position; }
	float rRaggio() const { return radius; }

	void updatePosition(int speed = 0) {
		this->speed += 0.25 - speed;

		if (this->speed > topspeed) { this->speed = topspeed; }
		if (this->speed < -topspeed) { this->speed = -topspeed; }

		height += this->speed;

	}

	void render(sf::RenderWindow& window) {
		if (isAlive) {
			birdBody.setPosition(x_position, height);
			window.draw(birdBody);
		}
	}

	void jump(int distanzainy) {
		if (!isAlive) return; 
		
		this->checkBounds();
		float movementVector[2] = {speed, distanzainy};

		if (brain->lavora(movementVector)[0] > 0) 
			this->updatePosition(10);
			
		else 
			this->updatePosition();
			

	}

	void checkBounds() {
		if (height > screenLenght - radius) {this->die();}
		if (height < 0 + radius) {this->die();}
	}

	sf::Vector2f ritornaposizione() {
		return sf::Vector2f(x_position, height);
	}


	int ranzodmizeColor() {

		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0, 255);

		return range(rng);
	}

	void resetBird() {
		height = screenLenght / 2; 
		isAlive = true;

	}

	void get_brain(neural* new_brain) {
		brain = new_brain;
	}

	void die() {
		if (!isAlive) return;
		isAlive = false;
		brain->die();
	}

	bool isAlive() {
		return isAlive;
	}

};