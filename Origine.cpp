#include <SFML/Graphics.hpp>
#include <iostream> 
#include <sstream>
#include "Global.h"
#include "Bird.h"
#include "Pipe.h"
#include "NeuralHandler.h"

template <typename T>
std::string toString(T arg)
{
	std::stringstream ss;
	ss << arg;
	return ss.str();
}

sf::Vector2i pipevicina(pipe tubii[]);
int DistanzaYpipeVicina(pipe tubii[], ucello& bird);
void scrivi(sf::Text& testo, sf::RenderWindow& window, int score, int generazione, int vivi);
int numero_vivi(ucello[]);

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(larghezza, lunghezza), "Minchia", sf::Style::Default, settings);
	window.setFramerateLimit(framerete);

	int nonmutare = 0;
	int score = 0;
	int generazione = 0;
	
	sf::Font font;
	if (!font.loadFromFile("Bellerose.ttf")) {
		std::cout << "font non caricato";
	}
	sf::Text testo;
	testo.setFont(font);
	testo.setCharacterSize(40);
	testo.Bold;


INIZIO:
	ucello bird[numeroucelli];
	NeuralHandler reti(numeroucelli, 2, 3, 2, 1);
	for (int i = 0; i < numeroucelli; i++) {
		bird[i].ricevi_cervello(reti.rp_Singolarete(i));
	}
	
	pipe tubi[2] = { larghezza,larghezza + spaziopipe + spessorepipe };


	while (window.isOpen()) {

		window.clear(sf::Color::Cyan);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (int i = 0; i < numeroucelli; i++) {

			if (bird[i].isvivo()) {
				bird[i].salta(DistanzaYpipeVicina(tubi, bird[i]));
				tubi[0].totalecollisioni(bird[i].vivo, bird[i]);
				tubi[1].totalecollisioni(bird[i].vivo, bird[i]);
			}

			bird[i].disegna(window);
		}

		tubi[0].updatadisegna(window, score);
		tubi[1].updatadisegna(window, score);

		if (reti.mutato()) {
			
			score = 0;
			tubi[0].restart(larghezza);
			tubi[1].restart(larghezza + spaziopipe + spessorepipe);
			generazione++;
			for (int i = 0; i < numeroucelli; i++) {
				bird[i].resetta();
			}
		}
		
		scrivi(testo, window, score, generazione, reti.NumeroVivi());
		window.display();
		
	}

}

sf::Vector2i pipevicina(pipe tubii[]) {

	if (tubii[0].ritornaposizione() < tubii[1].ritornaposizione()) {

		return tubii[0].rposizioni();

	}
	else {

		return tubii[1].rposizioni();

	}

}

int DistanzaYpipeVicina(pipe tubii[], ucello& bird) {

	if (tubii[0].ritornaposizione() < tubii[1].ritornaposizione()) {

		return bird.rAltezza() - (tubii[0].rposizioni().y + pipegap / 2);

	}
	else {

		return bird.rAltezza() - (tubii[1].rposizioni().y + pipegap / 2);

	}


}



void scrivi(sf::Text& testo, sf::RenderWindow& window, int score, int generazione, int vivi) {

	testo.setFillColor(sf::Color::Yellow);

	testo.setPosition(window.getSize().x / 2, 0);
	testo.setString(toString<int>(score));
	window.draw(testo);

	testo.setPosition(0, 0);
	testo.setString("Gen:");
	window.draw(testo);

	testo.setPosition(80, 0);
	testo.setString(toString<int>(generazione));
	window.draw(testo);

	testo.setPosition(330, 0);
	testo.setString("Vivi:");
	window.draw(testo);

	testo.setPosition(390, 0);
	testo.setString(toString<int>(vivi));
	window.draw(testo);


}

