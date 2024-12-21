#include "zipf.h"
#include <cassert>  

std::random_device zipf::dev;
std::mt19937 zipf::rng(zipf::dev());
std::uniform_real_distribution<double> zipf::range(0.0, 1.0);
std::uniform_real_distribution<double> zipf::range_nonzipf(0.0, numberOfBirds);

zipf::zipf(int n)
    : m_n(n), m_c(0)
{
    for (m_i = 1; m_i <= m_n; m_i++) {
        m_c += (1.0 / m_i);
    }
    m_c = 1.0 / m_c;

    m_sum_probs = new double[m_n + 1];
    m_sum_probs[0] = 0;
    for (m_i = 1; m_i <= m_n; m_i++) {
        m_sum_probs[m_i] = m_sum_probs[m_i - 1] + m_c / m_i;
    }
}

zipf::~zipf() {
    delete[] m_sum_probs;
    m_sum_probs = nullptr;
}

int zipf::randomZipfVal() {
    // Draw random number, ensure it's not 0 or 1
    do {
        m_z = range(rng);
    } while ((m_z == 0) || (m_z == 1));

    m_low = 1;
    m_high = m_n;

    // Binary search in the cumulative array
    do {
        m_mid = floor((m_low + m_high) / 2.0);
        if (m_sum_probs[m_mid] >= m_z && m_sum_probs[m_mid - 1] < m_z) {
            m_zipf_value = m_mid;
            break;
        }
        else if (m_sum_probs[m_mid] >= m_z) {
            m_high = m_mid - 1;
        }
        else {
            m_low = m_mid + 1;
        }
    } while (m_low <= m_high);

    // Zero-based index
    return m_zipf_value - 1;
}


