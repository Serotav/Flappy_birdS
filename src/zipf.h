#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <assert.h>
#include"Global.h"
/*
std::string random_number() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<unsigned long long> range(0, );
}
*/


class zipf {

private:
    double m_c;                     //costante di normalizzazione
    double* m_sum_probs;            //somma di probabilità pre calcolata
    double m_z;                     //numero casuale
    int m_n;                        //numero di oggetti nella distribuzione
    int m_zipf_value;               //numero generato da ritornare
    int m_i;                        //looper
    int m_low, m_high, m_mid;       //variabili per la ricerca binaria

    static std::random_device dev; //oggetti per generare un signor numero casuale
    static std::mt19937 rng;
    static std::uniform_real_distribution<double> range;
    static std::uniform_real_distribution<double> range_nonzipf;


public:
    //costruttori
    zipf(int n);
    ~zipf();

    //funzione che ritorna il numero casuale secondo la distribuzione
    int Rcasuale();
    int CasualeNonzipf();

};
std::random_device zipf::dev;
std::mt19937 zipf::rng(dev());
std::uniform_real_distribution<double> zipf::range(0, 1);
std::uniform_real_distribution<double> zipf::range_nonzipf(0, numeroucelli);
    

//cose che costruiscono e decostruiscono
zipf::zipf(int n): m_n(n), m_c(0)
{

    for (m_i = 1; m_i <= n; m_i++) {
        m_c = m_c + (1.0 / m_i);
    }
    m_c = 1.0 / m_c;

    m_sum_probs = new double[m_n + 1];
    m_sum_probs[0] = 0;
    for (m_i = 1; m_i <= n; m_i++) {
        m_sum_probs[m_i] = m_sum_probs[m_i - 1] + m_c / m_i;
    }

}

zipf::~zipf() {
    delete[] m_sum_probs;
    m_sum_probs = nullptr;

}

//l'unica funzione (ora non più)
int zipf::Rcasuale() {

    do{
        m_z = range(rng);
    } while ((m_z == 0) || (m_z == 1));

    m_low = 1, m_high = m_n, m_mid;
    do {
        m_mid = floor((m_low + m_high) / 2);
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


    return m_zipf_value-1;
}

int zipf::CasualeNonzipf() {
    return range_nonzipf(rng);
}
/*
int zipff(int n)
{
    static int first = true;      // Static first time flag
    static double c = 0;          // Normalization constant
    static double* sum_probs;     // Pre-calculated sum of probabilities
    double z;                     // Uniform random number (0 < z < 1)
    int zipf_value;               // Computed exponential value to be returned
    int    i;                     // Loop counter
    int low, high, mid;           // Binary-search bounds

    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<double> range(0, 1);

    // Compute normalization constant on first call only
    if (first == true)
    {
        for (i = 1; i <= n; i++)
            c = c + (1.0 / i);
         c = 1.0 / c;
               
        //sum_probs = malloc((n + 1) * sizeof(*sum_probs));
        sum_probs = new double[n + 1];
        sum_probs[0] = 0;
        for (i = 1; i <= n; i++) {
            sum_probs[i] = sum_probs[i - 1] + c / i;
            std::cout << "sum di :" << i <<" " << sum_probs[i] << "\n";
        }
        first = false;
    }

    // Pull a uniform random number (0 < z < 1)
    do
    {
        z = range(rng);
    } while ((z == 0) || (z == 1));

    // Map z to the value
    low = 1, high = n, mid;
    do {
        mid = floor((low + high) / 2);
        if (sum_probs[mid] >= z && sum_probs[mid - 1] < z) {
            zipf_value = mid;
            break;
        }
        else if (sum_probs[mid] >= z) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    } while (low <= high);

    
    return zipf_value;
}*/