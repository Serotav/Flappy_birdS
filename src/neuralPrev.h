#pragma once
#include "neuralnet.h"

class neural;

class NeuralPrev 
{
private:
    // Architecture info
    size_t m_numInputNeurons;
    size_t m_numHiddenNeurons;
    size_t m_numHiddenLayers;
    size_t m_numOutputNeurons;

    // Weights
    float**  m_weightsInputHidden;     // [m_numInputNeurons + 1][m_numHiddenNeurons]
    float*** m_weightsHidden;          // [m_numHiddenLayers - 1][m_numHiddenNeurons + 1][m_numHiddenNeurons]
    float**  m_weightsHiddenOutput;    // [m_numHiddenNeurons + 1][m_numOutputNeurons]

public:
    float m_score; // score associated with these weights

    // Constructors / Destructors
    NeuralPrev(int numIn, int numHidden, int layers, int numOut);
    ~NeuralPrev();
    NeuralPrev(const NeuralPrev&);

    // Operator overloads
    NeuralPrev& operator=(const NeuralPrev&);
    bool operator<(const NeuralPrev&) const;
    bool operator>(const NeuralPrev&) const;

public:
    // Public functions
    void saveConnections(neural& net);                 // Saves all connection values from a 'neural'
    float**  getWeightsInputHidden() const;            // Returns pointer to input->hidden weights
    float**  getWeightsHiddenOutput() const;           // Returns pointer to last hidden->output weights
    float*** getWeightsHidden() const;                 // Returns pointer to hidden->hidden weights
    void     printSnapshot() const;
    float    getSnapshotScore() const;                 // Returns the score associated with these weights
};

