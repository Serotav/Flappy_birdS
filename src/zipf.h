#pragma once
#include <random>
#include <vector>
#include <cmath>
#include "globalConst.h"

class zipf {
private:
    double m_c;         // Normalization constant
    double* m_sum_probs; // Precomputed cumulative probabilities
    double m_z;         // Random draw
    int m_n;            // Number of items
    int m_zipf_value;   // The generated number
    int m_i;            // Looper
    int m_low, m_high, m_mid; // Binary search variables

    // Declare static members but don't define them here
    static std::random_device dev;
    static std::mt19937 rng;
    static std::uniform_real_distribution<double> range;

public:
    zipf(int n);
    ~zipf();

    int randomZipfVal();
};
