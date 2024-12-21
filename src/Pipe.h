#pragma once
#include <random> 
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Global_const.h"
#include "Bird.h"
//>> <<

class pipe {

	int altezza;	//y
	int posizione;	//x
	sf::RectangleShape rettangolo;

public:
	pipe(int posizione) {
		this->randomizzaaltezza();
		this->posizione = posizione;
		rettangolo.setSize(sf::Vector2f(pipeThickness, pipeLength));
		rettangolo.setOrigin(0, pipeLength);
		rettangolo.setFillColor(sf::Color::Magenta);
		
	}

	void randomizzaaltezza() {
		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0+2*radius, screenLenght-7*radius);

		altezza = range(rng);
	}

	void azzeraposizione() { posizione = screenWidth; }

	void updateAndDraw(sf::RenderWindow& window, int &score) {
		
		this->posizione -= 1;
		if (posizione < -pipeThickness) { this->azzeraposizione(); this->randomizzaaltezza(); score++; }
		

		rettangolo.setPosition(posizione, altezza);
		window.draw(rettangolo);

		rettangolo.setPosition(posizione, altezza+pipeGapDistance+pipeLength);
		window.draw(rettangolo);
		
	}


	void totalecollisioni(gameBird& bird) {

		this->controllocollisioniy(bird, this->posizione, this->altezza, this->posizione, 0, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());
		this->controllocollisionix(bird, this->posizione, this->altezza, this->posizione + pipeThickness, this->altezza, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());
		this->controllocollisionix(bird, this->posizione, this->altezza + pipeGapDistance, this->posizione + pipeThickness, this->altezza + pipeGapDistance, bird.rPosizione(), bird.rAltezza(), bird.rRaggio());

	}


	void controllocollisioniy(gameBird& bird, float punti_a_x,float punti_a_y, float punti_b_x, float punti_b_y, float sfera_x, float sfera_y, float sfera_r) {
		punti_b_x = punti_b_x - punti_a_x;
		punti_b_y = punti_b_y - punti_a_y;

		float a = punti_b_x * punti_b_x + punti_b_y * punti_b_y;
		float b = 2 * punti_a_x * punti_b_x - 2 * punti_b_x * sfera_x + 2 * punti_a_y * punti_b_y - 2 * punti_b_y * sfera_y;
		float c = punti_a_x * punti_a_x + sfera_x * sfera_x - 2 * punti_a_x * sfera_x
			+ punti_a_y * punti_a_y + sfera_y * sfera_y - 2 * punti_a_y * sfera_y - sfera_r * sfera_r;
		float d = b * b - 4 * a * c;

		if (d > 0) {
			//std::cout << "impatto \n";
			float t1 = (-b + sqrt(d)) / (2 * a);
			float t2 = (-b - sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;

			float impatto2_x = punti_a_x + punti_b_x * t2;
			float impatto2_y = punti_a_y + punti_b_y * t2;
			
			if (!(impatto1_y >= this->altezza && impatto1_y <= this->altezza + pipeGapDistance)) { bird.die(); }
			else if (!(impatto2_y >= this->altezza && impatto2_y <= this->altezza + pipeGapDistance)) { bird.die(); }

			/*std::cout << "Impatto 1: X:" << impatto1_x << " Y:" << impatto1_y << "\n";
			std::cout << "Impatto 2: X:" << impatto2_x << " Y:" << impatto2_y << "\n";
			std::cout << "altezza :" << this->altezza << " altezza+gap:" << this->altezza + pipegap << "\n";*/
		}

		else if (d == 0) {
			//std::cout << "impatto-*- \n";
			float t1 = (-b + sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;
			
			if (!(impatto1_y >= this->altezza && impatto1_y <= this->altezza+pipeGapDistance)) { bird.die(); }
			

		}
		
	}

	void controllocollisionix(gameBird& bird, float punti_a_x,float punti_a_y, float punti_b_x, float punti_b_y, float sfera_x, float sfera_y, float sfera_r) {
		
		punti_b_x = punti_b_x - punti_a_x;
		punti_b_y = punti_b_y - punti_a_y;

		float a = punti_b_x * punti_b_x + punti_b_y * punti_b_y;
		float b = 2 * punti_a_x * punti_b_x - 2 * punti_b_x * sfera_x + 2 * punti_a_y * punti_b_y - 2 * punti_b_y * sfera_y;
		float c = punti_a_x * punti_a_x + sfera_x * sfera_x - 2 * punti_a_x * sfera_x
			+ punti_a_y * punti_a_y + sfera_y * sfera_y - 2 * punti_a_y * sfera_y - sfera_r * sfera_r;
		float d = b * b - 4 * a * c;

		if (d > 0) {
			//std::cout << "impatto \n";
			float t1 = (-b + sqrt(d)) / (2 * a);
			float t2 = (-b - sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;

			float impatto2_x = punti_a_x + punti_b_x * t2;
			float impatto2_y = punti_a_y + punti_b_y * t2;
			
			if (sfera_x+radius > this->posizione && sfera_x-radius < this->posizione + pipeThickness) { bird.die(); }
			

			/*std::cout << "Impatto 1: X:" << impatto1_x << " Y:" << impatto1_y << "\n";
			std::cout << "Impatto 2: X:" << impatto2_x << " Y:" << impatto2_y << "\n";
			std::cout << "Sfera x - raggio :" << sfera_x - raggio << " posizione " << this->posizione << "\n";*/
		}

		else if (d == 0) {
			//std::cout << "impatto-*- \n";
			float t1 = (-b + sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;
			
			if (sfera_x + radius > this->posizione && sfera_x - radius < this->posizione + pipeThickness) { bird.die(); }
			

		}

	}

	float ritornaposizione() {
		return posizione;
	}

	sf::Vector2i rposizioni() {
		sf::Vector2i vettore;
		vettore.x = posizione;
		vettore.y = altezza;
		return vettore;


	}

	void restart(int posizione) {
		this->randomizzaaltezza();
		this->posizione = posizione;
	}


};