#include <SFML/Graphics.hpp>
#include <iostream> 
#include <sstream>
#include <memory>

#include "globalConst.h"
#include "bird.h"
#include "pipe.h"
#include "neuralHandler.h"

int CalculateNearestPipeYDistance(pipe tubii[], gameBird& bird);
void writeText(const std::unique_ptr<sf::Text>& testo, std::unique_ptr<sf::RenderWindow>& window, int score, int generazione, int vivi);
std::unique_ptr<sf::RenderWindow> create_window();
std::pair<std::unique_ptr<sf::Text>, std::shared_ptr<sf::Font>> createText();
void game_loop(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<sf::Text> &testo, std::shared_ptr<sf::Font>& font, gameBird bird[], pipe tubi[], NeuralHandler& reti);

int main()
{
	auto window = create_window();
	auto [text, font] = createText();

	pipe pipes[2] = { screenWidth,screenWidth + pipeGapHeight + pipeThickness };
	NeuralHandler birdsNets(numberOfBirds, 2, 3, 2, 1);
	
	gameBird flock[numberOfBirds];
	for (int i = 0; i < numberOfBirds; i++) {
		flock[i].get_brain(birdsNets.getBrain(i));
	}
	

	game_loop(window, text, font , flock, pipes, birdsNets);

}

void game_loop(std::unique_ptr<sf::RenderWindow>& window, std::unique_ptr<sf::Text> &text, std::shared_ptr<sf::Font>& font, gameBird flock[], pipe pipes[], NeuralHandler& birdsNets){
	int score = 0;
	int generationCount = 0;
	sf::Event event;

	while (window->isOpen()) {

		window->clear(sf::Color::Cyan);

		while (window->pollEvent(event))
			if (event.type == sf::Event::Closed)
				window->close();

		score += pipes[0].updatePosition();
		score += pipes[1].updatePosition();

		for (int i =0; i < numberOfBirds; i++) {
			if (!flock[i].isAlive()) continue; 

			flock[i].jump(CalculateNearestPipeYDistance(pipes, flock[i]));
			
			pipes[0].evaluateCollisions(flock[i]);
			pipes[1].evaluateCollisions(flock[i]);

			flock[i].render(*window);
		}

		pipes[0].render(*window);
		pipes[1].render(*window);

		if (birdsNets.tryMutate()) {
			score = 0;
			pipes[0].reset(screenWidth);
			pipes[1].reset(screenWidth + pipeGapHeight + pipeThickness);
			generationCount++;
			for (int i = 0; i < numberOfBirds; i++) {
				flock[i].resetBird();
			}
		}

		writeText(text, window, score, generationCount, birdsNets.getAliveCount());
		window->display();
	}
}


std::unique_ptr<sf::RenderWindow> create_window(){
    sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
    auto window = std::make_unique<sf::RenderWindow>(sf::VideoMode(screenWidth, screenLenght), "FlappyBirdS", sf::Style::Default, settings);
    window->setFramerateLimit(framerete);
    return window;
}

std::pair<std::unique_ptr<sf::Text>, std::shared_ptr<sf::Font>> createText(){
    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile("font/Bellerose.ttf")) {
        std::cout << "error while loading the font";
        exit(1);
    }
    auto text = std::make_unique<sf::Text>();
    text->setFont(*font);
    text->setStyle(sf::Text::Bold);
	text->setOutlineThickness(0.7);
	text->setOutlineColor(sf::Color::Black);
    return { std::move(text), font };
}


int CalculateNearestPipeYDistance(pipe pipes[], gameBird& bird) {
	if (pipes[0].getPositionX() < pipes[1].getPositionX()) 
		return bird.getHeight() - (pipes[0].getPipePosition().y + pipeGapDistance / 2);

	return bird.getHeight() - (pipes[1].getPipePosition().y + pipeGapDistance / 2);
}


void writeText(const std::unique_ptr<sf::Text>& text, std::unique_ptr<sf::RenderWindow>& window, int score, int generation, int aliveBirds) {
    text->setColor(sf::Color::Yellow);
	text->setCharacterSize(50);
	text->setPosition(window->getSize().x / 2, 0);
    text->setString(std::to_string(score));
    window->draw(*text);

	text->setColor(sf::Color::White);
	text->setCharacterSize(30);
    text->setPosition(0, 0);
    text->setString("Gen: "+std::to_string(generation));
    window->draw(*text);

    text->setPosition(0, text->getCharacterSize());
    text->setString("Alive: "+std::to_string(aliveBirds));
    window->draw(*text);
}



