## Flappy BirdS

Welcome to **Flappy BirdS**! 🐦🚀

This project was an experiment back in 2022. I decided to dig it up from the grave and give it a new life on Linux. After some serious caffeine-fueled refactoring, I realized, *jeez, was I that bad at coding back then?* 😅

### 🧠 Project Overview

The mission was to build a somewhat simple neural network from scratch to play Flappy Bird. The neural network evolves using a genetic algorithm🧬. 
Here's how it works:

- **Generations Galore:** Each generation's members contribute to creating the next one.
- **Survival of the Fittest:** The higher a bird's score, the more likely it is that its "genes" get passed down. Thanks to the magic of Zipf distribution, the top performers have a better shot.
- **Mutation Madness:** To create a new generation, a random individual from the previous generation is selected, and its neural network values are randomly mutated.

All of this chaos was an excuse to dive deep into the principles of neural networks and get my hands dirty with some good old C++ (hence the love with `new` and `delete`).

### 🛠 How to Run It

Ready to watch some birds learn to fly (or crash spectacularly)? Follow these steps:

**Install SFML:** Make sure you have the necessary libraries installed. On Arch Linux, you can install SFML using the package manager.
```bash
sudo pacman -S --needed sfml
git clone https://github.com/Serotav/Flappy_birdS
cd Flappy_birdS
make -j4
./flappy_birdS
```


### 📝 Final Thoughts

I had a lot of fun trying to fix my old, ugly code and making it a little better. This entire project was a fantastic experience to learn C++ more deeply and to understand how neural networks work. A special shoutout to **Vsauce** for introducing me to the Zipf distribution—back then, I was searching for just the right method and stumbled upon his awesome video.

Flappy BirdS was more than just a game it was a playground for learning and experimentation. While the code might scream "I was learning," it's a testament to the journey of learning neural networks and the quirks of C++.

Happy flying! 🕊️✨