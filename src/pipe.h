#pragma once
#include <random> 
#include <SFML/Graphics.hpp>
#include <iostream>

#include "globalConst.h"
#include "bird.h"
//>> <<

class pipe {

	int pipeHeight;	    //y
	int pipePosition;	//x
	sf::RectangleShape pipeShape;

public:
	pipe(int positionX) {
		this->setRandomPipeHeight();
		this->pipePosition = positionX;
		pipeShape.setSize(sf::Vector2f(pipeThickness, pipeLength));
		pipeShape.setOrigin(0, pipeLength);
		pipeShape.setFillColor(sf::Color::Green);
		pipeShape.setOutlineThickness(5);
        pipeShape.setOutlineColor(sf::Color(0, 100, 0));
	}

	void setRandomPipeHeight() {
		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0+2*radius, screenLenght-7*radius);

		pipeHeight = range(rng);
	}

	void resetPipePosition() { pipePosition = screenWidth; }

	bool updatePosition(){
		this->pipePosition -= 1;
		if (pipePosition < -pipeThickness) { this->resetPipePosition(); this->setRandomPipeHeight(); return true; }
        return false;
	}

	void render(sf::RenderWindow& window) {	
	
		pipeShape.setPosition(pipePosition, pipeHeight);
		window.draw(pipeShape);

		pipeShape.setPosition(pipePosition, pipeHeight+pipeGapDistance+pipeLength);
		window.draw(pipeShape);
		
	}


	void evaluateCollisions(gameBird& bird) {

		this->checkCollisionY(bird, this->pipePosition, this->pipeHeight, this->pipePosition, 0, bird.getPositionX(), bird.getHeight(), bird.getRadius());
		this->checkCollisionX(bird, this->pipePosition, this->pipeHeight, this->pipePosition + pipeThickness, this->pipeHeight, bird.getPositionX(), bird.getHeight(), bird.getRadius());
		this->checkCollisionX(bird, this->pipePosition, this->pipeHeight + pipeGapDistance, this->pipePosition + pipeThickness, this->pipeHeight + pipeGapDistance, bird.getPositionX(), bird.getHeight(), bird.getRadius());

	}


void checkCollisionY(
    gameBird& bird,
    float startX,         // line start (x)
    float startY,         // line start (y)
    float endX,           // line end   (x)
    float endY,           // line end   (y)
    float circleCenterX,  // bird center (x)
    float circleCenterY,  // bird center (y)
    float circleRadius
) {

    
    float dx = endX - startX;
    float dy = endY - startY;

    float A = dx * dx + dy * dy;

    float B = 2.0f * (
        startX * dx - dx * circleCenterX +
        startY * dy - dy * circleCenterY
    );

    float C = (startX - circleCenterX) * (startX - circleCenterX)
            + (startY - circleCenterY) * (startY - circleCenterY)
            - circleRadius * circleRadius;

    float D = B * B - 4.0f * A * C;

    if (D < 0.0f) {
        return; 
    }

    float sqrtD = std::sqrt(D);

    float t1 = (-B + sqrtD) / (2.0f * A);
    float impact1X = startX + dx * t1;
    float impact1Y = startY + dy * t1;

    if (D > 0.0f) {
        float t2 = (-B - sqrtD) / (2.0f * A);
        float impact2X = startX + dx * t2;
        float impact2Y = startY + dy * t2;

        bool impact1OutsideGap =
            (impact1Y < this->pipeHeight) ||
            (impact1Y > this->pipeHeight + pipeGapDistance);

        bool impact2OutsideGap =
            (impact2Y < this->pipeHeight) ||
            (impact2Y > this->pipeHeight + pipeGapDistance);

        if (impact1OutsideGap || impact2OutsideGap) {
            bird.die();
        }
    }
    else {
        bool impactOutsideGap =
            (impact1Y < this->pipeHeight) ||
            (impact1Y > this->pipeHeight + pipeGapDistance);

        if (impactOutsideGap) {
            bird.die();
        }
    }
}

void checkCollisionX(
    gameBird& bird,
    float startX,         // line start (x)
    float startY,         // line start (y)
    float endX,           // line end   (x)
    float endY,           // line end   (y)
    float circleCenterX,  // bird center (x)
    float circleCenterY,  // bird center (y)
    float circleRadius
) {
    float dx = endX - startX;
    float dy = endY - startY;

    float A = dx * dx + dy * dy;
    float B = 2.0f * (
        startX * dx - dx * circleCenterX +
        startY * dy - dy * circleCenterY
    );
    float C = (startX - circleCenterX) * (startX - circleCenterX)
            + (startY - circleCenterY) * (startY - circleCenterY)
            - circleRadius * circleRadius;

    float D = B * B - 4.0f * A * C;
    if (D < 0.0f) {
        return; 
    }

    float sqrtD = std::sqrt(D);

    float t1 = (-B + sqrtD) / (2.0f * A);
    float impact1X = startX + dx * t1;
    float impact1Y = startY + dy * t1;

    if (D > 0.0f) {
        float t2 = (-B - sqrtD) / (2.0f * A);
        float impact2X = startX + dx * t2;
        float impact2Y = startY + dy * t2;

        if ((circleCenterX + circleRadius > this->pipePosition) &&
            (circleCenterX - circleRadius < this->pipePosition + pipeThickness)) {
            bird.die();
            return;
        }
    }
    else {
        if ((circleCenterX + circleRadius > this->pipePosition) &&
            (circleCenterX - circleRadius < this->pipePosition + pipeThickness)) {
            bird.die();
        }
    }
}


	float getPositionX() {
		return pipePosition;
	}

	sf::Vector2i getPipePosition() {
		sf::Vector2i vettore;
		vettore.x = pipePosition;
		vettore.y = pipeHeight;
		return vettore;


	}

	void reset(int newPosition) {
		this->setRandomPipeHeight();
		this->pipePosition = newPosition;
	}


};