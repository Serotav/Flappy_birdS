#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

#include "neuralnet.h"      // or "neuralnet.h" depending on your file naming
#include "zipf.h"
#include "neuralPrev.h"  // or "neuralpassato.h"

/*
  The NeuralHandler class manages ALL the neural networks. 
  It stores:
    - A vector of live neural networks (m_neuralNets)
    - A vector of past networks (m_pastNetworks), used for the mutation step.

  It has a function that checks if all networks are dead (isAnyNetworkAlive()).
  If they are all dead, tryMutate() mutates them to form a new generation.
*/

class NeuralHandler
{
private:
    std::vector<neural>       m_neuralNets;    // Vector holding the current generation of neural networks
    std::vector<NeuralPrev> m_pastNetworks; // Vector holding the 'snapshots' (past states) for mutation
    size_t                    m_numNetworks;   // How many networks we have
    zipf                      m_zipfDistribution; // Used to pick parents via a Zipf distribution

public:
    // Constructor
    NeuralHandler(size_t numNetworks,
                  unsigned numInputNeurons,
                  unsigned numHiddenNeuronsPerLayer,
                  unsigned numHiddenLayers,
                  unsigned numOutputNeurons);

    // Disable copy constructor / assignment
    NeuralHandler(const NeuralHandler&) = delete;
    NeuralHandler& operator=(const NeuralHandler&) = delete;

    // Destructor
    ~NeuralHandler();

public:
    // Returns a pointer to the i-th neural network
    neural* getBrain(size_t i);

    // Checks if all networks are dead. If they are, mutate them and return true. Otherwise return false.
    bool tryMutate();

    // Returns how many networks are still alive
    size_t getAliveCount() const;

private:
    // Returns true if at least one network is still alive
    bool isAnyNetworkAlive() const;

public:
    // Example debug function: logs how many times each index appears in the provided vector
    void logVectorCounts(std::vector<int>& v);
};

