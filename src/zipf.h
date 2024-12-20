#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <assert.h>

#include"Global_const.h"

class zipf {

private:
    double m_c;                     //costante di normalizzazione
    double* m_sum_probs;            //somma di probabilit� pre calcolata
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

//l'unica funzione (ora non pi�)
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
