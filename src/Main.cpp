#include <SFML/Graphics.hpp>
#include <iostream> 
#include <sstream>
#include <memory>

#include "Global_const.h"
#include "Bird.h"
#include "Pipe.h"
#include "NeuralHandler.h"

sf::Vector2i pipevicina(pipe tubii[]);
int DistanzaYpipeVicina(pipe tubii[], gameBird& bird);
void write_text(const std::unique_ptr<sf::Text>& testo, std::unique_ptr<sf::RenderWindow>& window, int score, int generazione, int vivi);
int numero_vivi(gameBird[]);
std::unique_ptr<sf::RenderWindow> create_window();
std::pair<std::unique_ptr<sf::Text>, std::shared_ptr<sf::Font>> create_text();
void game_loop(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<sf::Text> &testo, std::shared_ptr<sf::Font>& font, gameBird bird[], pipe tubi[], NeuralHandler& reti);

int main()
{
	auto window = create_window();
	auto [testo, font] = create_text();

	pipe tubi[2] = { screenWidth,screenWidth + pipeGapHeight + pipeThickness };
	NeuralHandler reti(numberOfBirds, 2, 3, 2, 1);
	
	gameBird flock[numberOfBirds];
	for (int i = 0; i < numberOfBirds; i++) {
		flock[i].get_brain(reti.rp_Singolarete(i));
	}
	

	game_loop(window, testo, font , flock, tubi, reti);

}

void game_loop(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<sf::Text> &testo, std::shared_ptr<sf::Font>& font, gameBird bird[], pipe tubi[], NeuralHandler& reti){
	int score = 0;
	int generationCount = 0;
	sf::Event event;

	while (window->isOpen()) {

		window->clear(sf::Color::Cyan);

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
		}

		for (int i = 0; i < numberOfBirds; i++) {

			if (bird[i].isAlive()) {
				bird[i].jump(DistanzaYpipeVicina(tubi, bird[i]));
				tubi[0].totalecollisioni(bird[i].vivo, bird[i]);
				tubi[1].totalecollisioni(bird[i].vivo, bird[i]);
			}

			bird[i].render(*window);
		}

		tubi[0].updateAndDraw(*window, score);
		tubi[1].updateAndDraw(*window, score);

		if (reti.mutato()) {
			score = 0;
			tubi[0].restart(screenWidth);
			tubi[1].restart(screenWidth + pipeGapHeight + pipeThickness);
			generationCount++;
			for (int i = 0; i < numberOfBirds; i++) {
				bird[i].resetta();
			}
		}

		write_text(testo, window, score, generationCount, reti.NumeroVivi());
		window->display();
	}
}



std::unique_ptr<sf::RenderWindow> create_window(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 80;
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenWidth, screenLenght), "Flappy_birdS", sf::Style::Default, settings);
    window->setFramerateLimit(framerete);
    return window;
}

std::pair<std::unique_ptr<sf::Text>, std::shared_ptr<sf::Font>> create_text(){
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile("font/Bellerose.ttf")) {
        std::cout << "error while loading the font";
        exit(1);
    }
    auto testo = std::make_unique<sf::Text>();
    testo->setFont(*font);
    testo->setCharacterSize(40);
    testo->setStyle(sf::Text::Bold);
    testo->setFillColor(sf::Color::Yellow);

    return { std::move(testo), font };
}

sf::Vector2i pipevicina(pipe tubii[]) {
	if (tubii[0].ritornaposizione() < tubii[1].ritornaposizione()) 
		return tubii[0].rposizioni();
	return tubii[1].rposizioni();
}

int DistanzaYpipeVicina(pipe tubii[], gameBird& bird) {
	if (tubii[0].ritornaposizione() < tubii[1].ritornaposizione()) 
		return bird.rAltezza() - (tubii[0].rposizioni().y + pipeGapDistance / 2);

	return bird.rAltezza() - (tubii[1].rposizioni().y + pipeGapDistance / 2);
}


void write_text(const std::unique_ptr<sf::Text>& testo, std::unique_ptr<sf::RenderWindow>& window, int score, int generazione, int vivi) {
    testo->setPosition(window->getSize().x / 2, 0);
    testo->setString(std::to_string(score));
    window->draw(*testo);

    testo->setPosition(0, 0);
    testo->setString("Gen:");
    window->draw(*testo);

    testo->setPosition(80, 0);
    testo->setString(std::to_string(generazione));
    window->draw(*testo);

    testo->setPosition(330, 0);
    testo->setString("Vivi:");
    window->draw(*testo);

    testo->setPosition(390, 0);
    testo->setString(std::to_string(vivi));
    window->draw(*testo);
}



