#include "neuralHandler.h"


NeuralHandler::NeuralHandler(size_t numNetworks,
                             unsigned numInputNeurons,
                             unsigned numHiddenNeuronsPerLayer,
                             unsigned numHiddenLayers,
                             unsigned numOutputNeurons)
    : m_numNetworks(numNetworks)
    , m_zipfDistribution(numNetworks)
{
    m_neuralNets.reserve(m_numNetworks);
    m_pastNetworks.reserve(m_numNetworks);

    // Create networks and their corresponding "past" slots
    for (size_t i = 0; i < m_numNetworks; i++)
    {
        m_neuralNets.emplace_back(numInputNeurons,
                                  numHiddenNeuronsPerLayer,
                                  numHiddenLayers,
                                  numOutputNeurons);

        m_pastNetworks.emplace_back(numInputNeurons,
                                    numHiddenNeuronsPerLayer,
                                    numHiddenLayers,
                                    numOutputNeurons);
    }
}

NeuralHandler::~NeuralHandler()
{
}

bool NeuralHandler::isAnyNetworkAlive() const {
    // If any network isAlive() is true, return true
    for (auto& net : m_neuralNets)
    {
        if (net.isAlive())
        {
            return true;
        }
    }
    return false;
}

neural* NeuralHandler::getBrain(size_t i){
    if (i < m_neuralNets.size())
    {
        return &m_neuralNets[i];
    }
    std::cout << "\nWARNING: NeuralHandler::getBrain() out of range\n";
    return nullptr;
}

bool NeuralHandler::tryMutate(){
    // If at least one network is still alive, we cannot mutate yet
    if (isAnyNetworkAlive())
    {
        return false;
    }

    // Save the connections (weights) from each current network into m_pastNetworks
    for (size_t i = 0; i < m_numNetworks; i++)
    {
        m_pastNetworks[i].saveConnections(m_neuralNets[i]);
    }

    // Sort past networks by descending score (using operator>)
    std::sort(m_pastNetworks.begin(), m_pastNetworks.end(), std::greater<>());

    // For each live network, pick a parent (index) according to Zipf distribution, then mutate
    std::vector<int> debugIndices;
    debugIndices.reserve(m_numNetworks);

    for (auto& net : m_neuralNets)
    {
        int randomParentIndex = m_zipfDistribution.randomZipfVal();
        debugIndices.push_back(randomParentIndex);

        // Mutate this network using the chosen parent's weights
        net.mutate(m_pastNetworks[randomParentIndex]);
    }

    return true;
}

size_t NeuralHandler::getAliveCount() const{
    size_t aliveCount = 0;
    for (auto& net : m_neuralNets)
    {
        // net.isAlive() returns true/false; true is implicitly 1 when added
        aliveCount += net.isAlive();
    }
    return aliveCount;
}

void NeuralHandler::logVectorCounts(std::vector<int>& v){
    std::cout << "------VECTOR CONTENT------\n\n";
    int count = 0;

    // This example assumes 'numberOfBirds' is some global constant or included from Global_const.h
    // Adjust the range as needed if you want to log up to a certain value
    for (int i = 0; i < numberOfBirds / 5; i++)
    {
        for (const auto& val : v)
        {
            if (val == i)
            {
                count++;
            }
        }
        std::cout << "Index: " << i << " appears " << count << " times\n";
        count = 0;
    }

    std::cout << "\n";
    std::cin.get();
}
