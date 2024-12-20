#pragma once
#include"Neuralnet.h"
class neural;
class NeuralPassato {

private:
	size_t m_n;		//numero neuroni di imput
	size_t m_n1;	//numero neuroni in ogni layer hidden
	size_t m_n2;	//numero layer hidden
	size_t m_n3;	//numero neuroni output
		
	float** m_c_imputHidden;	//connessioni tra l'imput layer e il primo hidden [n+1][n1]
	float*** m_c_hidden;		//connessioni tra i neuroni dell'hidden layer [n2-1][n1+1][n1]
	float** m_c_hiddenOutput;	//connessioni tra l'ultimo hidden e l'output [n1+1][n3]


public://cose che costruiscono e demoliscono
	float punteggio;			//punteggio associato a questi pesi
	NeuralPassato(int n, int n1, int n2, int n3);	//ricoda la morale: qua si alloca
	~NeuralPassato();								//qua si dealloca
	NeuralPassato(const NeuralPassato&);			//copiatore, a sto giro lo devi chiamare per forza
	
	NeuralPassato& operator= (const NeuralPassato&);//anche qui va fatta una deep copy
	bool operator < (const NeuralPassato& ) const;  //overlord dell'operatore minore per sortare
	bool operator > (const NeuralPassato&) const;	//stessa cosa ma indovina cosa cambia
public://funzioni public
	void salvaconnessioni(neural&);					//pija una rete e salva tutti i valori delle connessioni
	float** ritorna_pesi_imput_hidden()const;		//ritorna un puntatore ai pesi dell'imput-hidden layer (m_c_imputHidden)
	float** ritorna_pesi_hidden_outout()const;		//ritorna un puntatore ai pesi dell'ultimo hidden-outout layer (m_c_hiddenOutput)
	float*** ritorna_pesi_hidden()const;			//ritorna un puntatore ai pesi del'hidden layer (m_c_hidden)
	void printa()const;
	float rpunteggio() const;						//ritorna il punteggio associato a questi pesi
};

