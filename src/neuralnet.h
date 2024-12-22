#pragma once
#include<iostream>
#include<random>
#include<chrono>
#include<cstring>

#include "neuralPrev.h"
#define MUTATION_RANGE 0.50 		// Define how much a parameter can mutate at most. 0.05 would mean it can increase or decrease between -5% and +5%
#define MUTATION_PROBABILITY 0.30 	// Probability that any given parameter will mutate

// Time-related shortcuts
#define CURRENT_TIME            std::chrono::high_resolution_clock::now()
#define DURATION_TYPE           std::chrono::duration<float>
#define TIME_POINT_TYPE         std::chrono::system_clock::time_point

//>> << <>
class NeuralPrev;

class neural {

private: // Network parameters 
    
    size_t m_numInputNeurons;      // number of input neurons
    size_t m_numHiddenNeurons;     // number of neurons in each hidden layer
    size_t m_numHiddenLayers;      // number of hidden layers
    size_t m_numOutputNeurons;     // number of output neurons

    // Connection counts
    size_t m_connectionInputHidden;    // from input layer to first hidden layer
    size_t m_connectionHiddenHidden;   // from one hidden layer to the next
    size_t m_connectionHiddenOutput;   // from last hidden layer to output layer

    // Arrays to store neuron values:
    // m_inputNeurons has [m_numInputNeurons + 1] (including bias)
    // m_outputNeurons has [m_numOutputNeurons]
    // m_hiddenNeurons is [m_numHiddenLayers][m_numHiddenNeurons + 1] (including bias)
    float*  m_inputNeurons;
    float*  m_outputNeurons;
    float** m_hiddenNeurons;

    // Connection weight arrays:
    // m_weightsInputHidden is [m_numInputNeurons + 1][m_numHiddenNeurons]
    // m_weightsHidden is [m_numHiddenLayers - 1][m_numHiddenNeurons + 1][m_numHiddenNeurons]
    // m_weightsHiddenOutput is [m_numHiddenNeurons + 1][m_numOutputNeurons]
    float**  m_weightsInputHidden;
    float*** m_weightsHidden;
    float**  m_weightsHiddenOutput;

    // Time and score
    TIME_POINT_TYPE m_startTime;    
    DURATION_TYPE   m_score;       
    bool            m_isAlive;     

    // Static random objects
    static std::random_device          s_randomDevice;
    static std::mt19937                s_rng;
    static std::uniform_real_distribution<float> s_distributionRange;
    static std::uniform_real_distribution<float> s_distributionMutationRange;

public: //costruttori
    // Constructors and destructor
    neural(unsigned numInputNeurons, unsigned numHiddenNeurons, unsigned numHiddenLayers, unsigned numOutputNeurons);
    ~neural();

    
    neural(const neural&); 	// Copy constructor (not typically used, but provided for vector usage)
    neural& operator=(const neural&) = delete;

private:
    // Private helper methods
    void  setBias();                           // Sets the bias neurons to 1
    void  resetNeurons();                      // Zeros out neuron values (except bias)
    float activationFunction(float) const;// Activation function (e.g., sigmoid)
    float randomizeMutation() const;           // Decides whether and how much a parameter mutates

public:
    // Public interface
    void  randomizeConnections();                          // Assign random weights in [-1, 1]
    float* feedForward(float inputArray[]);                // Propagate input through the network
    void  mutate(neural& parentNetwork);                   // Mutate weights, using another neural as base
    void  mutate(NeuralPrev& parentSnapshot);           // Mutate weights, using a NeuralPassato as base
    float**  getWeightsInputHidden() const;                // Returns the weights from input to hidden
    float**  getWeightsHiddenOutput() const;               // Returns the weights from hidden to output
    float*** getWeightsHidden() const;                     // Returns the hidden-to-hidden weights
    void     die();                                        // Called when the network "dies"; calculates final score
    float    getScore() const;                             // Returns the network’s score (seconds alive)
    bool     isAlive() const;                              // Returns whether the network is still alive

public:
    // Debug methods
    void printNetwork() const;
    void printRandomSamples(int count) const;
};
