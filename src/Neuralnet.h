#pragma once
#include<iostream>
#include<random>
#include<chrono>
#include <cstring>
#include "NeuralPassato.h"
#define RANGE_MUTAZIONI 0.50	//di quanto, al massimo, un parametro deve mutare 0.05 vuol dire che pu� aumentare o diminuire in un range tra -5% e + 5%
#define PROB_MUTAZIONE	0.30	//probabillit� che una mutazione avvenga

#define ORARIO					std::chrono::high_resolution_clock::now()	// ritorna l'orario quasi esatto quando chiamato
#define DURATA					std::chrono::duration<float>				//oggetto che contiene una durata
#define PUNTO_TEMPPORALE		std::chrono::system_clock::time_point		//contiene un punto temporale

//>> << <>
class NeuralPassato;

class neural {

private:	//parametri network
	size_t m_n;		//numero neuroni di imput
	size_t m_n1;	//numero neuroni in ogni layer hidden
	size_t m_n2;	//numero layer hidden
	size_t m_n3;	//numero neuroni output

	size_t m_c;		//numero connessioni tra n ed n1 c = n*n1+n1
	size_t m_c1;	//numero connessioni tra un hidden layer e l'altro c1= (n1+1)*n1
	size_t m_c2;	//numeroconnessioni tra l'ultimo hidden layewr e l'output layer c2= (n1+1)*n3;

	float* m_n_imput;	//neuroni di imput, sono [n+1]
	float* m_n_output;	//neuroni di output, sono [n3]
	float** m_n_hidden;	//neuroni dell'hidden layer, sono [n2][n1+1]

	float** m_c_imputHidden;	//connessioni tra l'imput layer e il primo hidden [n+1][n1]
	float*** m_c_hidden;		//connessioni tra i neuroni dell'hidden layer [n2-1][n1+1][n1]
	float** m_c_hiddenOutput;	//connessioni tra l'ultimo hidden e l'output [n1+1][n3]

	PUNTO_TEMPPORALE m_inizio;	//tempo da cui si inizia a contare il punteggio (il punteggio � il tempo che sta rete rimane in vita)
	DURATA			 m_score;	//punteggio della rete, ricordati che � un tempo in secondi
	bool m_isvivo;				//descrive se � vivo o meno

	static std::random_device dev;
	static std::mt19937 rng; //https://isocpp.org/files/papers/n3551.pdf
	static std::uniform_real_distribution<float> range;
	static std::uniform_real_distribution<float> range_mutazioni;

public: //costruttori
	neural(unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output);

	~neural();

	neural(const neural&); //questo � il construttore copia, in teoria non dovrebbe essere mai chiamato, l'hai costruito solo per fare vettori, SE VIENE CHIAMATO SPERO PER TE CHE CI SIA UN BUON MOTIVO
	neural& operator= (const neural&) = delete;

private://funzini private
	void setta_bias();						//setta tutti i neuroni bias a 1
	void azzera_neuroni();					//setta il valore di tutti i neuroni a 0, tranne i bias (grazie a cazzo)
	float activation_function(float) const;	//� la funzione di attivazione, prende il valore all'interno di un neurone e ne restituisce uno tra -1 e 1
	float casualizza_mutazione()const;				//decide se qualcosa muta o no e di quanto

public: //funzioni pubbliche	
	void randomizza_connessioni();					//assegna ad ogni connessione un valore casuale tra -1 e 1 (questi compresi)
	float* lavora(float[]);							//prende in imput, tutti i valori di impput, li fa passare attraverso il network e ritorna un puntatore all'array di neuroni di output
	void muta(neural&);								//fa mutare le connessioni usando solo un'altro network come base
	void muta(NeuralPassato&);						//fa mutare le connessioni usando un neural passato come base
	float** ritorna_pesi_imput_hidden()const;		//ritorna un puntatore ai pesi dell'imput-hidden layer (m_c_imputHidden)
	float** ritorna_pesi_hidden_outout()const;		//ritorna un puntatore ai pesi dell'ultimo hidden-outout layer (m_c_hiddenOutput)
	float*** ritorna_pesi_hidden()const;			//ritorna un puntatore ai pesi del'hidden layer (m_c_hidden)
	void die();								//quando viene chiamata questa si determina il punteggio (m_score) di questa rete
	float ritorna_punteggio()const;						//ritorna il punteggio della rete 
	bool ritorna_isvivo()const;

public: //cose inutili
	void printa() const;
	void casual(int numero) const;

};
