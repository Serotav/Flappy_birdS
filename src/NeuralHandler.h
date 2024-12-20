#pragma once
#include<iostream>
#include<random>
#include<vector>
#include"TrueNeural.h"
#include "zipf.h"
#include "NeuralPassato.h"

class NeuralHandler {

private://vettori di reti e di salvataggi
	std::vector<neural> m_reti;					//vettore contenente le reti neurali
	std::vector<NeuralPassato> m_reti_passate;	//vettore contenente le reti neurali
	size_t m_numero_reti;						//indovina cosa indica
	zipf m_zipf;								//il sacro grall

public: //construttori
	NeuralHandler(size_t n_reti,unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output);
	
	NeuralHandler(NeuralHandler&) = delete;
	NeuralHandler& operator= (const NeuralHandler&) = delete;

	~NeuralHandler(); //questo non l'hai implementato, in teoria non ti serve fare nulla qua

public://funzioni public
	neural* rp_Singolarete(size_t i);	//ritorna il puntatore ad una singola rete
	bool mutato();						//controlla se sono tutti morti, se c'è qualcuno vivo ritorna false, altrimenti fa mutare tutti e ritorna true
	size_t NumeroVivi()const;				//indovina cosa fa

private:
	bool ControllaVivi()const;

public://funzioni inutili
	void printavettore(std::vector<int> &);


};
//cose statiche


//construttori----------------------------------------------------------------------------------------------------------------------------------------------------------
NeuralHandler::NeuralHandler(size_t n_reti, unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output):m_numero_reti(n_reti), m_zipf(n_reti) {

	m_reti.reserve(m_numero_reti);
	m_reti_passate.reserve(m_numero_reti);
	
	
	for (size_t i = 0; i < m_numero_reti; i++) {
		m_reti.emplace_back(n_neuroni_imput, n_neuroni_ogni_hidden, n_layer_hidden, n_neuroni_output);
		m_reti_passate.emplace_back(n_neuroni_imput, n_neuroni_ogni_hidden, n_layer_hidden, n_neuroni_output);
	}


}

NeuralHandler::~NeuralHandler(){}

//funzioni private----------------------------------------------------------------------------------------------------------------------------------------------------------
bool NeuralHandler::ControllaVivi() const{

	for (auto& rete : m_reti) {
		if (rete.ritorna_isvivo())
			return true;
	}
	return false;

}

//funzioni pubbliche----------------------------------------------------------------------------------------------------------------------------------------------------------
neural* NeuralHandler::rp_Singolarete(size_t i){
	if(i<m_reti.size())
		return &m_reti[i]; //questa la devi assegnare ad un neural*, non &neural
	std::cout << "\n Neural Handler: stai cercando di farti ritornare una rete che è fuori dal range, ti becchi null ptr";
	return nullptr;
}

bool NeuralHandler::mutato() {

	if (ControllaVivi())
		return false;

	//std::cout << "\nSCAMBIAMOCI DI POSTO\n";
	std::vector<int> temp; int itemp;
	
	for (int i = 0; i < m_numero_reti; i++) {
		m_reti_passate[i].salvaconnessioni(m_reti[i]);
	}

	std::sort(m_reti_passate.begin(), m_reti_passate.end(), std::greater<>());
	//std::cout << "\nSortati \n";
	//std::cout << "\n1: "<<m_reti_passate[0].rpunteggio()<<"  " << m_reti_passate[10].rpunteggio()<<"\n";
	
	for (auto& rete : m_reti) {
		itemp = m_zipf.Rcasuale();
		temp.push_back(itemp);
		rete.muta(m_reti_passate[itemp]);
		//rete.muta(m_reti_passate[m_zipf.Rcasuale()]);
	}
	//printavettore(temp);
	return true;

}

size_t NeuralHandler::NumeroVivi() const{
	static size_t conta;
	conta = 0;

	for (auto& rete : m_reti) {
		conta += rete.ritorna_isvivo();
	}
	return conta;
}

void NeuralHandler::printavettore(std::vector<int>& v) {
	std::cout << "------VETTORE------\n\n";
	int temp=0;
	for (int i = 0; i < numeroucelli / 5; i++) {
		for (const auto& c : v)
			if (c == i)
				temp++;
		std::cout << "Di: " << i << " sono " << temp<<"\n";
		temp = 0;
	}
	
	
	std::cout << "\n";
	std::cin.get();
}