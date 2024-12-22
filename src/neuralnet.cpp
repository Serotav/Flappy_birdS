#include "neuralnet.h"

// Static members initialization
std::random_device neural::s_randomDevice;
std::mt19937 neural::s_rng(neural::s_randomDevice());
std::uniform_real_distribution<float> neural::s_distributionRange(-1.0f, 1.0f);
std::uniform_real_distribution<float> neural::s_distributionMutationRange(1.0f - MUTATION_RANGE, 1.0f + MUTATION_RANGE);

// Constructor
neural::neural(
    unsigned numInputNeurons, 
    unsigned numHiddenNeurons, 
    unsigned numHiddenLayers, 
    unsigned numOutputNeurons
)
    : m_numInputNeurons(numInputNeurons)
    , m_numHiddenNeurons(numHiddenNeurons)
    , m_numHiddenLayers(numHiddenLayers)
    , m_numOutputNeurons(numOutputNeurons)
    , m_connectionInputHidden(m_numInputNeurons * m_numHiddenNeurons + m_numHiddenNeurons) 	
    , m_connectionHiddenHidden((m_numHiddenNeurons + 1) * m_numHiddenNeurons) 				
    , m_connectionHiddenOutput((m_numHiddenNeurons + 1) * m_numOutputNeurons)
    , m_startTime(CURRENT_TIME)
    , m_isAlive(true)
{
    // Allocate neuron arrays
    m_inputNeurons   = new float[m_numInputNeurons + 1];
    m_outputNeurons  = new float[m_numOutputNeurons];
    
    m_hiddenNeurons = new float*[m_numHiddenLayers];
    for (int i = 0; i < (int)m_numHiddenLayers; i++){
        // Each hidden layer has [m_numHiddenNeurons + 1] neurons (including bias)
        m_hiddenNeurons[i] = new float[m_numHiddenNeurons + 1];
    }

    // Allocate weight arrays
    // Input -> Hidden weights
    m_weightsInputHidden = new float*[m_numInputNeurons + 1];
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++)
    {
        m_weightsInputHidden[i] = new float[m_numHiddenNeurons];
    }

    // Hidden -> Hidden weights
    // We have (m_numHiddenLayers - 1) sets of connections
    m_weightsHidden = new float**[m_numHiddenLayers - 1];
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++){
        m_weightsHidden[i] = new float*[m_numHiddenNeurons + 1];
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++)
        {
            m_weightsHidden[i][j] = new float[m_numHiddenNeurons];
        }
    }

    // Hidden -> Output weights
    m_weightsHiddenOutput = new float*[m_numHiddenNeurons + 1];
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        m_weightsHiddenOutput[i] = new float[m_numOutputNeurons];
    }

    // Assign random weights and set biases
    randomizeConnections();
    setBias();
}

// Copy constructor (rarely used, but present for completeness)
neural::neural(const neural& other)
    : m_numInputNeurons(other.m_numInputNeurons)
    , m_numHiddenNeurons(other.m_numHiddenNeurons)
    , m_numHiddenLayers(other.m_numHiddenLayers)
    , m_numOutputNeurons(other.m_numOutputNeurons)
    , m_connectionInputHidden(other.m_connectionInputHidden)
    , m_connectionHiddenHidden(other.m_connectionHiddenHidden)
    , m_connectionHiddenOutput(other.m_connectionHiddenOutput)
    , m_startTime(other.m_startTime)
    , m_score(other.m_score)
    , m_isAlive(other.m_isAlive)
{
    std::cout << "COPY CONSTRUCTOR INVOKED. This should not happen.\n";

    // Copy neuron arrays
    m_inputNeurons  = new float[m_numInputNeurons + 1];
    std::memcpy(m_inputNeurons, other.m_inputNeurons, sizeof(float) * (m_numInputNeurons + 1));

    m_outputNeurons = new float[m_numOutputNeurons];
    std::memcpy(m_outputNeurons, other.m_outputNeurons, sizeof(float) * (m_numOutputNeurons));

    m_hiddenNeurons = new float*[m_numHiddenLayers];
    for (int i = 0; i < (int)m_numHiddenLayers; i++){
        m_hiddenNeurons[i] = new float[m_numHiddenNeurons + 1];
        // We do NOT copy the actual hidden neuron contents here (only weights).
    }

    // Copy weights: input->hidden
    m_weightsInputHidden = new float*[m_numInputNeurons + 1];
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        m_weightsInputHidden[i] = new float[m_numHiddenNeurons];
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_weightsInputHidden[i][j] = other.m_weightsInputHidden[i][j];
        }
    }

    // Copy weights: hidden->hidden
    m_weightsHidden = new float**[m_numHiddenLayers - 1];
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++){
        m_weightsHidden[i] = new float*[m_numHiddenNeurons + 1];
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++){
            m_weightsHidden[i][j] = new float[m_numHiddenNeurons];
            for (int k = 0; k < (int)m_numHiddenNeurons; k++){
                m_weightsHidden[i][j][k] = other.m_weightsHidden[i][j][k];
            }
        }
    }

    // Copy weights: hidden->output
    m_weightsHiddenOutput = new float*[m_numHiddenNeurons + 1];
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        m_weightsHiddenOutput[i] = new float[m_numOutputNeurons];
        for (int j = 0; j < (int)m_numOutputNeurons; j++){
            m_weightsHiddenOutput[i][j] = other.m_weightsHiddenOutput[i][j];
        }
    }

    setBias();
}

// Destructor
neural::~neural(){
    // Clean up memory
    delete[] m_inputNeurons;
    delete[] m_outputNeurons;

    for (int i = 0; i < (int)m_numHiddenLayers; i++){
        delete[] m_hiddenNeurons[i];
    }
    delete[] m_hiddenNeurons;

    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        delete[] m_weightsInputHidden[i];
    }
    delete[] m_weightsInputHidden;

    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++){
            delete[] m_weightsHidden[i][j];
        }
        delete[] m_weightsHidden[i];
    }
    delete[] m_weightsHidden;

    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        delete[] m_weightsHiddenOutput[i];
    }
    delete[] m_weightsHiddenOutput;
}

// Private methods
void neural::randomizeConnections(){
    // Input->Hidden
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_weightsInputHidden[i][j] = s_distributionRange(s_rng);
        }
    }

    // Hidden->Output
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        for (int j = 0; j < (int)m_numOutputNeurons; j++){
            m_weightsHiddenOutput[i][j] = s_distributionRange(s_rng);
        }
    }

    // Hidden->Hidden
    for (int i = 0; i < (int)m_numHiddenLayers - 1; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons + 1; j++){
            for (int k = 0; k < (int)m_numHiddenNeurons; k++){
                m_weightsHidden[i][j][k] = s_distributionRange(s_rng);
            }
        }
    }
}

void neural::setBias(){
    // The last neuron in the input array is the bias
    m_inputNeurons[m_numInputNeurons] = 1.0f;

    // For each hidden layer, set the last neuron as bias
    for (int i = 0; i < (int)m_numHiddenLayers; i++){
        m_hiddenNeurons[i][m_numHiddenNeurons] = 1.0f;
    }
}

float neural::activationFunction(float value) const{
    // Example: Sigmoid activation
    return 1.0f / (1.0f + std::exp(-value));
}

void neural::resetNeurons(){
    // Reset output neurons
    for (int i = 0; i < (int)m_numOutputNeurons; i++){
        m_outputNeurons[i] = 0.0f;
    }

    // Reset hidden neurons (not including bias)
    for (int i = 0; i < (int)m_numHiddenLayers; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_hiddenNeurons[i][j] = 0.0f;
        }
    }
}

float neural::randomizeMutation() const{

	if (std::uniform_real_distribution<float>(0,1)(s_rng) < MUTATION_PROBABILITY)
    	return s_distributionMutationRange(s_rng);
	else
    	return 1.0f;

}

//funzioni pubbliche ---------------------------------------- >> << <>
float* neural::feedForward(float inputArray[]){
    // 1) Reset hidden and output layers
    resetNeurons();

    // 2) Copy input values
    for (int i = 0; i < (int)m_numInputNeurons; i++){
        m_inputNeurons[i] = inputArray[i];
    }

    // 3) From input layer to first hidden layer
    for (int i = 0; i < (int)m_numHiddenNeurons; i++){
        for (int j = 0; j < (int)m_numInputNeurons; j++){
            m_hiddenNeurons[0][i] += m_inputNeurons[j] * m_weightsInputHidden[j][i];
        }
        // Include bias from input layer
        m_hiddenNeurons[0][i] += m_inputNeurons[m_numInputNeurons] * m_weightsInputHidden[m_numInputNeurons][i];
        m_hiddenNeurons[0][i] = activationFunction(m_hiddenNeurons[0][i]);
    }

    // 4) From one hidden layer to the next
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++){
        for (int neuronIndex = 0; neuronIndex < (int)m_numHiddenNeurons; neuronIndex++){
            for (int nextNeuron = 0; nextNeuron < (int)m_numHiddenNeurons; nextNeuron++){
                m_hiddenNeurons[layer + 1][nextNeuron] += 
                    m_hiddenNeurons[layer][neuronIndex] * m_weightsHidden[layer][neuronIndex][nextNeuron];
            }
            // Add bias from the current hidden layer
            m_hiddenNeurons[layer + 1][neuronIndex] += 
                m_hiddenNeurons[layer][m_numHiddenNeurons] * m_weightsHidden[layer][m_numHiddenNeurons][neuronIndex];
        }
        // Apply activation
        for (int nextNeuron = 0; nextNeuron < (int)m_numHiddenNeurons; nextNeuron++){
            m_hiddenNeurons[layer + 1][nextNeuron] = activationFunction(m_hiddenNeurons[layer + 1][nextNeuron]);
        }
    }

    // 5) From last hidden layer to output
    for (int i = 0; i < (int)m_numOutputNeurons; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_outputNeurons[i] += m_hiddenNeurons[m_numHiddenLayers - 1][j] * m_weightsHiddenOutput[j][i];
        }
        // Add bias from last hidden layer
        m_outputNeurons[i] += 
            m_hiddenNeurons[m_numHiddenLayers - 1][m_numHiddenNeurons] * m_weightsHiddenOutput[m_numHiddenNeurons][i];
        m_outputNeurons[i] = activationFunction(m_outputNeurons[i]);
    }

    return m_outputNeurons;
}

float** neural::getWeightsInputHidden() const{
    return m_weightsInputHidden;
}

float** neural::getWeightsHiddenOutput() const{
    return m_weightsHiddenOutput;
}

float*** neural::getWeightsHidden() const{
    return m_weightsHidden;
}

void neural::mutate(neural& parentNetwork){
    // Retrieve parent's weight pointers
    float**  parentInputHidden    = parentNetwork.getWeightsInputHidden();
    float**  parentHiddenOutput   = parentNetwork.getWeightsHiddenOutput();
    float*** parentHidden         = parentNetwork.getWeightsHidden();

    // Mutate input->hidden
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_weightsInputHidden[i][j] = parentInputHidden[i][j] * randomizeMutation();
        }
    }

    // Mutate hidden->hidden
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++){
        for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
            for (int j = 0; j < (int)m_numHiddenNeurons; j++){
                m_weightsHidden[layer][i][j] = parentHidden[layer][i][j] * randomizeMutation();
            }
        }
    }

    // Mutate hidden->output
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        for (int j = 0; j < (int)m_numOutputNeurons; j++){
            m_weightsHiddenOutput[i][j] = parentHiddenOutput[i][j] * randomizeMutation();
        }
    }

    // Reset start time and mark as alive
    m_startTime = CURRENT_TIME;
    m_isAlive   = true;
}

void neural::mutate(NeuralPrev& parentSnapshot){
    // Retrieve parent's weight pointers
    float**  parentInputHidden    = parentSnapshot.getWeightsInputHidden();
    float**  parentHiddenOutput   = parentSnapshot.getWeightsHiddenOutput();
    float*** parentHidden         = parentSnapshot.getWeightsHidden();

    // Mutate input->hidden
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            m_weightsInputHidden[i][j] = parentInputHidden[i][j] * randomizeMutation();
        }
    }

    // Mutate hidden->hidden
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++){
        for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
            for (int j = 0; j < (int)m_numHiddenNeurons; j++){
                m_weightsHidden[layer][i][j] = parentHidden[layer][i][j] * randomizeMutation();
            }
        }
    }

    // Mutate hidden->output
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        for (int j = 0; j < (int)m_numOutputNeurons; j++){
            m_weightsHiddenOutput[i][j] = parentHiddenOutput[i][j] * randomizeMutation();
        }
    }

    // Reset start time and mark as alive
    m_startTime = CURRENT_TIME;
    m_isAlive   = true;
}

void neural::die(){
    if (m_isAlive){
        m_isAlive = false;
        m_score = CURRENT_TIME - m_startTime;
    }
}

float neural::getScore() const{
    return m_score.count();
}

bool neural::isAlive() const{
    return m_isAlive;
}

// Debug methods
void neural::printNetwork() const{
    // Print network parameters
    std::cout << "Input neurons: "          << m_numInputNeurons    << "\n"
              << "Hidden neurons per layer: " << m_numHiddenNeurons   << "\n"
              << "Hidden layers: "          << m_numHiddenLayers    << "\n"
              << "Output neurons: "         << m_numOutputNeurons   << "\n\n";

    std::cout << "Connections (not always used in code logic):\n"
              << "Input->Hidden count: "    << m_connectionInputHidden    << "\n"
              << "Hidden->Hidden count: "   << m_connectionHiddenHidden   << "\n"
              << "Hidden->Output count: "   << m_connectionHiddenOutput   << "\n\n";

    // Weights
    std::cout << "\n\nWEIGHTS (the last neuron is always bias)\nINPUT->HIDDEN:\n";
    for (int i = 0; i < (int)m_numInputNeurons + 1; i++){
        std::cout << "Neuron i=" << i << ": ";
        for (int j = 0; j < (int)m_numHiddenNeurons; j++){
            std::cout << m_weightsInputHidden[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n\nHIDDEN->HIDDEN:\n";
    for (int layer = 0; layer < (int)m_numHiddenLayers - 1; layer++){
        std::cout << "Between layer " << layer << " and " << layer + 1 << "\n";
        for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
            std::cout << "Neuron i=" << i << ": ";
            for (int j = 0; j < (int)m_numHiddenNeurons; j++){
                std::cout << m_weightsHidden[layer][i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    std::cout << "\n\nLAST HIDDEN->OUTPUT:\n";
    for (int i = 0; i < (int)m_numHiddenNeurons + 1; i++){
        std::cout << "Neuron i=" << i << ": ";
        for (int j = 0; j < (int)m_numOutputNeurons; j++){
            std::cout << m_weightsHiddenOutput[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void neural::printRandomSamples(int count) const{
    for (int i = 0; i < count; i++){
        std::cout << s_distributionRange(s_rng) << "\n";
    }
}