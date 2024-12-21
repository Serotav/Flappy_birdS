#include "neuralPrev.h"

// Constructor
NeuralPassato::NeuralPassato(int numIn, int numHidden, int layers, int numOut)
    : m_numInputNeurons(numIn)
    , m_numHiddenNeurons(numHidden)
    , m_numHiddenLayers(layers)
    , m_numOutputNeurons(numOut)
    , m_score(0.0f)
{
    // Allocate weight arrays
    m_weightsInputHidden = new float*[m_numInputNeurons + 1];
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        m_weightsInputHidden[i] = new float[m_numHiddenNeurons];
    }

    m_weightsHidden = new float**[m_numHiddenLayers - 1];
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++)
    {
        m_weightsHidden[i] = new float*[m_numHiddenNeurons + 1];
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++)
        {
            m_weightsHidden[i][j] = new float[m_numHiddenNeurons];
        }
    }

    m_weightsHiddenOutput = new float*[m_numHiddenNeurons + 1];
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        m_weightsHiddenOutput[i] = new float[m_numOutputNeurons];
    }
}

// Copy constructor
NeuralPassato::NeuralPassato(const NeuralPassato& other)
    : m_numInputNeurons(other.m_numInputNeurons)
    , m_numHiddenNeurons(other.m_numHiddenNeurons)
    , m_numHiddenLayers(other.m_numHiddenLayers)
    , m_numOutputNeurons(other.m_numOutputNeurons)
    , m_score(other.m_score)
{
    // Allocate and copy input->hidden
    m_weightsInputHidden = new float*[m_numInputNeurons + 1];
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        m_weightsInputHidden[i] = new float[m_numHiddenNeurons];
        for (int j = 0; j < (int)m_numHiddenNeurons; j++)
        {
            m_weightsInputHidden[i][j] = other.m_weightsInputHidden[i][j];
        }
    }

    // Allocate and copy hidden->hidden
    m_weightsHidden = new float**[m_numHiddenLayers - 1];
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++)
    {
        m_weightsHidden[i] = new float*[m_numHiddenNeurons + 1];
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++)
        {
            m_weightsHidden[i][j] = new float[m_numHiddenNeurons];
            for (int k = 0; k < (int)m_numHiddenNeurons; k++)
            {
                m_weightsHidden[i][j][k] = other.m_weightsHidden[i][j][k];
            }
        }
    }

    // Allocate and copy hidden->output
    m_weightsHiddenOutput = new float*[m_numHiddenNeurons + 1];
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        m_weightsHiddenOutput[i] = new float[m_numOutputNeurons];
        for (int j = 0; j < (int)m_numOutputNeurons; j++)
        {
            m_weightsHiddenOutput[i][j] = other.m_weightsHiddenOutput[i][j];
        }
    }
}

// Destructor
NeuralPassato::~NeuralPassato(){
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        delete[] m_weightsInputHidden[i];
    }
    delete[] m_weightsInputHidden;

    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++)
    {
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++)
        {
            delete[] m_weightsHidden[i][j];
        }
        delete[] m_weightsHidden[i];
    }
    delete[] m_weightsHidden;

    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        delete[] m_weightsHiddenOutput[i];
    }
    delete[] m_weightsHiddenOutput;
}

// Operators
bool NeuralPassato::operator<(const NeuralPassato& other) const{
    return m_score < other.m_score;
}

bool NeuralPassato::operator>(const NeuralPassato& other) const{
    return m_score > other.m_score;
}

NeuralPassato& NeuralPassato::operator=(const NeuralPassato& other){
    m_score             = other.m_score;
    m_numInputNeurons   = other.m_numInputNeurons;
    m_numHiddenNeurons  = other.m_numHiddenNeurons;
    m_numHiddenLayers   = other.m_numHiddenLayers;
    m_numOutputNeurons  = other.m_numOutputNeurons;

    // Copy input->hidden
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        for (int j = 0; j < (int)m_numHiddenNeurons; j++)
        {
            m_weightsInputHidden[i][j] = other.m_weightsInputHidden[i][j];
        }
    }

    // Copy hidden->hidden
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++)
    {
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++)
        {
            for (int k = 0; k < (int)m_numHiddenNeurons; k++)
            {
                m_weightsHidden[i][j][k] = other.m_weightsHidden[i][j][k];
            }
        }
    }

    // Copy hidden->output
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        for (int j = 0; j < (int)m_numOutputNeurons; j++)
        {
            m_weightsHiddenOutput[i][j] = other.m_weightsHiddenOutput[i][j];
        }
    }

    return *this;
}

// Public functions
void NeuralPassato::saveConnections(neural& net){
    float**  netInputHidden   = net.getWeightsInputHidden();
    float**  netHiddenOutput  = net.getWeightsHiddenOutput();
    float*** netHidden        = net.getWeightsHidden();

    // Copy input->hidden
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        for (int j = 0; j < (int)m_numHiddenNeurons; j++)
        {
            m_weightsInputHidden[i][j] = netInputHidden[i][j];
        }
    }

    // Copy hidden->hidden
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++)
    {
        for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
        {
            for (int j = 0; j < (int)m_numHiddenNeurons; j++)
            {
                m_weightsHidden[layer][i][j] = netHidden[layer][i][j];
            }
        }
    }

    // Copy hidden->output
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        for (int j = 0; j < (int)m_numOutputNeurons; j++)
        {
            m_weightsHiddenOutput[i][j] = netHiddenOutput[i][j];
        }
    }

    m_score = net.getScore();
}

float** NeuralPassato::getWeightsInputHidden() const{
    return m_weightsInputHidden;
}

float** NeuralPassato::getWeightsHiddenOutput() const{
    return m_weightsHiddenOutput;
}

float*** NeuralPassato::getWeightsHidden() const{
    return m_weightsHidden;
}

float NeuralPassato::getSnapshotScore() const{
    return m_score;
}

void NeuralPassato::printSnapshot() const{
    std::cout << "\n\nSTORED WEIGHTS (the last neuron is always the bias)\nINPUT->HIDDEN:\n";
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        std::cout << "Neuron i=" << i << ": ";
        for (int j = 0; j < (int)m_numHiddenNeurons; j++)
        {
            std::cout << m_weightsInputHidden[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\nHIDDEN->HIDDEN:\n";
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++)
    {
        std::cout << "Between layer " << layer << " and " << layer + 1 << "\n";
        for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
        {
            std::cout << "Neuron i=" << i << ": ";
            for (int j = 0; j < (int)m_numHiddenNeurons; j++)
            {
                std::cout << m_weightsHidden[layer][i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n\nLAST HIDDEN->OUTPUT:\n";
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++)
    {
        std::cout << "Neuron i=" << i << ": ";
        for (int j = 0; j < (int)m_numOutputNeurons; j++)
        {
            std::cout << m_weightsHiddenOutput[i][j] << " ";
        }
        std::cout << "\n";
    }
}

