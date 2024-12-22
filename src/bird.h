#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "globalConst.h"
#include "neuralnet.h"

//>> <<
class gameBird {

	private:
	int height;
	int x_position;

	float speed;
	sf::CircleShape birdBody;
	neural* brain;
	bool alive = true;


public:
	gameBird() 
		: height(screenLenght / 2), speed(0), x_position(radius + 5) {
		birdBody.setRadius(radius);
		birdBody.setFillColor(sf::Color(randomizeColor(), randomizeColor(), randomizeColor()));
		birdBody.setOrigin(radius, radius);
		birdBody.setOutlineThickness(1);
		birdBody.setOutlineColor(sf::Color::Black);
	}
	int getHeight() const { return this->height; }
	int getPositionX() const { return this->x_position; }
	float getRadius() const { return radius; }

	void updatePosition(int speed = 0) {
		this->speed += 0.25 - speed;

		if (this->speed > topspeed) { this->speed = topspeed; }
		if (this->speed < -topspeed) { this->speed = -topspeed; }

		height += this->speed;

	}

	void render(sf::RenderWindow& window) {
		if (!alive) return; 
		
		birdBody.setPosition(x_position, height);
		window.draw(birdBody);
	}

	void jump(int verticalOffset) {
		if (!alive) return; 
		
		this->checkBounds();
		float movementVector[2] = {speed, verticalOffset};

		if (brain->feedForward(movementVector)[0] > 0.5f) 
			this->updatePosition(jump_height);
			
		else 
			this->updatePosition();
	}

	void checkBounds() {
		if (height > screenLenght - radius) {this->die();}
		if (height < 0 + radius) {this->die();}
	}


	int randomizeColor() {

		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0, 255);

		return range(rng);
	}

	void resetBird() {
		height = screenLenght / 2; 
		alive = true;
	}

	void get_brain(neural* new_brain) {
		brain = new_brain;
	}

	void die() {
		if (!alive) return;
		alive = false;
		brain->die();
	}

	bool isAlive() {
		return alive;
	}

};