#pragma once
#include "Global_const.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Neuralnet.h"

//>> <<
class gameBird {

	int altezza;
	float speed;
	int posizione;
	static int uccisi;
	sf::CircleShape corpo;
	neural* cervello;
public: bool vivo = true;


public:
	gameBird() {altezza = screenLenght / 2; speed = 0; posizione = radius + 5; ;  corpo.setRadius(radius); corpo.setFillColor(sf::Color(randomizzacolore(), randomizzacolore(), randomizzacolore())); corpo.setOrigin(radius, radius); }

	int rAltezza() const { return this->altezza; }

	int rPosizione() const { return this->posizione; }

	float rRaggio() const { return radius; }

	void updata(int speed = 0) {
		this->speed += 0.25;
		this->speed -= speed;
		if (this->speed > topspeed) { this->speed = topspeed; }
		if (this->speed < -topspeed) { this->speed = -topspeed; }

		altezza += this->speed;

	}

	void render(sf::RenderWindow& window) {
		if (vivo) {
			corpo.setPosition(posizione, altezza);
			window.draw(corpo);
		}
	}

	void jump(int distanzainy) {
		this->controlla();
		if (vivo) {
			float temp[2];
			temp[0] = speed;
			temp[1] = distanzainy;

			if (cervello->lavora(temp)[0] > 0) {
				
				this->updata(10);

			}
			this->updata();
			
		}

		/*else if (vivo == false && this->altezza < lunghezza - raggio - 1) {
			this->speed += 0.5;
			if (this->speed > topspeed) { this->speed = topspeed; }
			if (this->speed < -topspeed) { this->speed = -topspeed; }

			this->altezza += this->speed;

		}*/


	}

	void controlla() {
		
		if (altezza > screenLenght - radius) { vivo = false;}
		if (altezza < 0 + radius) { vivo = false; }
		if (!vivo) { cervello->uccidilo(); uccisi++; }
	}

	sf::Vector2f ritornaposizione() {

		return sf::Vector2f(posizione, altezza);
	}


	int randomizzacolore() {

		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0, 255);

		return range(rng);
	}

	void resetta() {
		altezza = screenLenght / 2; 
		vivo = true;
		if (uccisi) {
			std::cout << "Morti da sopra/sotto: " << uccisi << "\n";
			uccisi = 0;
		}
	}

	void get_brain(neural* eccolo) {
		cervello = eccolo;
	}

	bool isAlive() {
		return cervello->ritorna_isvivo();
	}

};
int gameBird::uccisi = 0;