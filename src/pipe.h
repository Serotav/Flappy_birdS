#pragma once
#include <random> 
#include <SFML/Graphics.hpp>
#include <iostream>

#include "global_const.h"
#include "bird.h"
//>> <<

class pipe {

	int pipeHeight;	//y
	int posizione;	//x
	sf::RectangleShape pipeShape;

public:
	pipe(int posizione) {
		this->randomizzaaltezza();
		this->posizione = posizione;
		pipeShape.setSize(sf::Vector2f(pipeThickness, pipeLength));
		pipeShape.setOrigin(0, pipeLength);
		pipeShape.setFillColor(sf::Color::Magenta);
		
	}

	void randomizzaaltezza() {
		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0+2*radius, screenLenght-7*radius);

		pipeHeight = range(rng);
	}

	void azzeraposizione() { posizione = screenWidth; }

	void updatePosition(int &score){
		this->posizione -= 1;
		if (posizione < -pipeThickness) { this->azzeraposizione(); this->randomizzaaltezza(); score++; }
	}

	void render(sf::RenderWindow& window) {	
	
		pipeShape.setPosition(posizione, pipeHeight);
		window.draw(pipeShape);

		pipeShape.setPosition(posizione, pipeHeight+pipeGapDistance+pipeLength);
		window.draw(pipeShape);
		
	}


	void evaluateCollisions(gameBird& bird) {

		this->checkCollisionY(bird, this->posizione, this->pipeHeight, this->posizione, 0, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());
		this->checkCollisionX(bird, this->posizione, this->pipeHeight, this->posizione + pipeThickness, this->pipeHeight, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());
		this->checkCollisionX(bird, this->posizione, this->pipeHeight + pipeGapDistance, this->posizione + pipeThickness, this->pipeHeight + pipeGapDistance, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());

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

        if ((circleCenterX + circleRadius > this->posizione) &&
            (circleCenterX - circleRadius < this->posizione + pipeThickness)) {
            bird.die();
            return;
        }
    }
    else {
        if ((circleCenterX + circleRadius > this->posizione) &&
            (circleCenterX - circleRadius < this->posizione + pipeThickness)) {
            bird.die();
        }
    }
}


	float ritornaposizione() {
		return posizione;
	}

	sf::Vector2i rposizioni() {
		sf::Vector2i vettore;
		vettore.x = posizione;
		vettore.y = pipeHeight;
		return vettore;


	}

	void restart(int posizione) {
		this->randomizzaaltezza();
		this->posizione = posizione;
	}


};