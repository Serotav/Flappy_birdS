#pragma once
#include <random> 
#include <SFML/Graphics.hpp>
#include "Global.h"
#include <iostream>
#include "Bird.h"
//>> <<

class pipe {

	int altezza;	//y
	int posizione;	//x
	static int uccisi;
	sf::RectangleShape rettangolo;

public:
	pipe(int posizione) {
		this->randomizzaaltezza();
		this->posizione = posizione;
		rettangolo.setSize(sf::Vector2f(spessorepipe, lunghezzapipe));
		rettangolo.setOrigin(0, lunghezzapipe);
		rettangolo.setFillColor(sf::Color::Magenta);
		
	}

	void randomizzaaltezza() {
		static std::random_device dev;
		static std::mt19937 rng(dev());
		static std::uniform_int_distribution<std::mt19937::result_type> range(0+2*raggio, lunghezza-7*raggio);

		altezza = range(rng);
	}

	void azzeraposizione() { posizione = larghezza; }

	void updatadisegna(sf::RenderWindow& window, int &score) {
		
		this->posizione -= 1;
		if (posizione < -spessorepipe) { this->azzeraposizione(); this->randomizzaaltezza(); score++; }
		

		rettangolo.setPosition(posizione, altezza);
		window.draw(rettangolo);

		rettangolo.setPosition(posizione, altezza+pipegap+lunghezzapipe);
		window.draw(rettangolo);
		
		
		
	}


	void totalecollisioni(bool& vivo, ucello& cazzo) {

		this->controllocollisioniy(this->posizione, this->altezza, this->posizione, 0, cazzo.rPosizione(), cazzo.rAltezza(), cazzo.rRaggio(), vivo);
		this->controllocollisionix(this->posizione, this->altezza, this->posizione + spessorepipe, this->altezza, cazzo.rPosizione(), cazzo.rAltezza(), cazzo.rRaggio(), vivo);
		this->controllocollisionix(this->posizione, this->altezza + pipegap, this->posizione + spessorepipe, this->altezza + pipegap, cazzo.rPosizione(), cazzo.rAltezza(), cazzo.rRaggio(), vivo);

	}

	


	void controllocollisioniy(float punti_a_x,float punti_a_y, float punti_b_x, float punti_b_y, float sfera_x, float sfera_y, float sfera_r, bool& vivo) {
		if (!vivo) return;
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
			
			if (!(impatto1_y >= this->altezza && impatto1_y <= this->altezza + pipegap)) { vivo = false; }
			else if (!(impatto2_y >= this->altezza && impatto2_y <= this->altezza + pipegap)) { vivo = false; }

			/*std::cout << "Impatto 1: X:" << impatto1_x << " Y:" << impatto1_y << "\n";
			std::cout << "Impatto 2: X:" << impatto2_x << " Y:" << impatto2_y << "\n";
			std::cout << "altezza :" << this->altezza << " altezza+gap:" << this->altezza + pipegap << "\n";*/
		}

		else if (d == 0) {
			//std::cout << "impatto-*- \n";
			float t1 = (-b + sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;
			
			if (!(impatto1_y >= this->altezza && impatto1_y <= this->altezza+pipegap)) { vivo = false; }
			

		}
		if (!vivo) return;
		
	}

	void controllocollisionix(float punti_a_x,float punti_a_y, float punti_b_x, float punti_b_y, float sfera_x, float sfera_y, float sfera_r, bool& vivo) {
		
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
			
			if (sfera_x+raggio > this->posizione && sfera_x-raggio < this->posizione + spessorepipe) { vivo = false; }
			

			/*std::cout << "Impatto 1: X:" << impatto1_x << " Y:" << impatto1_y << "\n";
			std::cout << "Impatto 2: X:" << impatto2_x << " Y:" << impatto2_y << "\n";
			std::cout << "Sfera x - raggio :" << sfera_x - raggio << " posizione " << this->posizione << "\n";*/
		}

		else if (d == 0) {
			//std::cout << "impatto-*- \n";
			float t1 = (-b + sqrt(d)) / (2 * a);

			float impatto1_x = punti_a_x + punti_b_x * t1;
			float impatto1_y = punti_a_y + punti_b_y * t1;
			
			if (sfera_x + raggio > this->posizione && sfera_x - raggio < this->posizione + spessorepipe) { vivo = false; }
			

		}
		if (!vivo)
			uccisi++;
		

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
		if (uccisi) {
			std::cout << "Uccisi dai tubi: " << uccisi << "\n";
			uccisi = 0;
		}
	}


};
int pipe::uccisi=0;