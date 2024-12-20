#pragma once
#include "Global.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TrueNeural.h"

//>> <<
class ucello {

	int altezza;
	float speed;
	int posizione;
	static int uccisi;
	sf::CircleShape corpo;
	neural* cervello;
public: bool vivo = true;


public:
	ucello() {altezza = lunghezza / 2; speed = 0; posizione = raggio + 5; ;  corpo.setRadius(raggio); corpo.setFillColor(sf::Color(randomizzacolore(), randomizzacolore(), randomizzacolore())); corpo.setOrigin(raggio, raggio); }

	int rAltezza() const { return this->altezza; }

	int rPosizione() const { return this->posizione; }

	float rRaggio() const { return raggio; }

	void updata(int speed = 0) {
		this->speed += 0.25;
		this->speed -= speed;
		if (this->speed > topspeed) { this->speed = topspeed; }
		if (this->speed < -topspeed) { this->speed = -topspeed; }

		altezza += this->speed;

	}

	void disegna(sf::RenderWindow& window) {
		if (vivo) {
			corpo.setPosition(posizione, altezza);
			window.draw(corpo);
		}
	}

	void salta(int distanzainy) {
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
		
		if (altezza > lunghezza - raggio) { vivo = false;}
		if (altezza < 0 + raggio) { vivo = false; }
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
		altezza = lunghezza / 2; 
		vivo = true;
		if (uccisi) {
			std::cout << "Morti da sopra/sotto: " << uccisi << "\n";
			uccisi = 0;
		}
	}

	void ricevi_cervello(neural* eccolo) {
		cervello = eccolo;
	}

	bool isvivo() {
		return cervello->ritorna_isvivo();
	}

};
int ucello::uccisi = 0;