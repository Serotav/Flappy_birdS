#pragma once
#include<iostream>
#include<random>
#include<vector>
#include<algorithm>

#include"Neuralnet.h"
#include"zipf.h"
#include"NeuralPassato.h"

/*
The NeuralHandler class is a class that manages ALL the neural networks, it has a vector of neural networks and a vector of past neural networks(used for the mutation step).
It has a function that checks if all the neural networks are dead, if they are all dead it mutates them all.
*/
class NeuralHandler {

private://vettori di reti e di salvataggi
	std::vector<neural> m_neuralNets;					//vettore contenente le reti neurali
	std::vector<NeuralPassato> m_reti_passate;	//vettore contenente le reti neurali
	size_t m_numero_reti;						//indovina cosa indica
	zipf m_zipf;								//il sacro grall

public: //construttori
	NeuralHandler(size_t n_reti,unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output);
	
	NeuralHandler(NeuralHandler&) = delete;
	NeuralHandler& operator= (const NeuralHandler&) = delete;

	~NeuralHandler(); //questo non l'hai implementato, in teoria non ti serve fare nulla qua

public://funzioni public
	neural* give_brain(size_t i);	//ritorna il puntatore ad una singola rete
	bool tryMutate();						//controlla se sono tutti morti, se c'� qualcuno vivo ritorna false, altrimenti fa mutare tutti e ritorna true
	size_t GetAliveCount()const;				//indovina cosa fa

private:
	bool IsAnyNeuronAlive()const;

public://funzioni inutili
	void logVectorCounts(std::vector<int> &);


};


//construttori----------------------------------------------------------------------------------------------------------------------------------------------------------
NeuralHandler::NeuralHandler(size_t n_reti, unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output):m_numero_reti(n_reti), m_zipf(n_reti) {

	m_neuralNets.reserve(m_numero_reti);
	m_reti_passate.reserve(m_numero_reti);
	
	
	for (size_t i = 0; i < m_numero_reti; i++) {
		m_neuralNets.emplace_back(n_neuroni_imput, n_neuroni_ogni_hidden, n_layer_hidden, n_neuroni_output);
		m_reti_passate.emplace_back(n_neuroni_imput, n_neuroni_ogni_hidden, n_layer_hidden, n_neuroni_output);
	}


}

NeuralHandler::~NeuralHandler(){}

//funzioni private----------------------------------------------------------------------------------------------------------------------------------------------------------
bool NeuralHandler::IsAnyNeuronAlive() const{

	for (auto& rete : m_neuralNets) {
		if (rete.ritorna_isvivo())
			return true;
	}
	return false;

}

//funzioni pubbliche----------------------------------------------------------------------------------------------------------------------------------------------------------
neural* NeuralHandler::give_brain(size_t i){
	if(i<m_neuralNets.size())
		return &m_neuralNets[i]; // assign this to a nerual*
	std::cout << "\n WARNING: NeuralHandler::give_brain() out of range\n";
	return nullptr;
}

bool NeuralHandler::tryMutate() {

	if (IsAnyNeuronAlive())
		return false;

	//std::cout << "\n PERFORMING THE MUTATION\n";
	std::vector<int> temp; int itemp;
	
	for (int i = 0; i < m_numero_reti; i++) {
		m_reti_passate[i].salvaconnessioni(m_neuralNets[i]);
	}

	std::sort(m_reti_passate.begin(), m_reti_passate.end(), std::greater<>());
	//std::cout << "\nSortati \n";
	//std::cout << "\n1: "<<m_reti_passate[0].rpunteggio()<<"  " << m_reti_passate[10].rpunteggio()<<"\n";
	
	for (auto& rete : m_neuralNets) {
		itemp = m_zipf.Rcasuale();
		temp.push_back(itemp);
		rete.muta(m_reti_passate[itemp]);
		//rete.muta(m_reti_passate[m_zipf.Rcasuale()]);
	}
	//printavettore(temp);
	return true;

}

size_t NeuralHandler::GetAliveCount() const{
	size_t count = 0;

	for (auto& neuralNet : m_neuralNets) {
		count += neuralNet.ritorna_isvivo();
	}
	return count;
}

void NeuralHandler::logVectorCounts(std::vector<int>& v) {
	std::cout << "------VECTOR------\n\n";
	int temp=0;
	for (int i = 0; i < numberOfBirds / 5; i++) {
		for (const auto& c : v)
			if (c == i)
				temp++;
		std::cout << "OF: " << i << " I'M " << temp<<"\n";
		temp = 0;
	}
	
	
	std::cout << "\n";
	std::cin.get();
}