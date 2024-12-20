#include"NeuralPassato.h"

//construttori---------------------------------------------------------------------------------------------------------------------------------
NeuralPassato::NeuralPassato(int n, int n1, int n2, int n3) : m_n(n), m_n1(n1), m_n2(n2), m_n3(n3), punteggio(NULL) {

	m_c_imputHidden = new float* [m_n + 1];
	for (int i = 0; i < m_n + 1; i++) { m_c_imputHidden[i] = new float[m_n1]; }

	m_c_hidden = new float** [m_n2 - 1];
	for (int i = 0; i < m_n2 - 1; i++) { m_c_hidden[i] = new float* [m_n1 + 1]; for (int j = 0; j < m_n1 + 1; j++) { m_c_hidden[i][j] = new float[m_n1]; } }

	m_c_hiddenOutput = new float* [m_n1 + 1];
	for (int i = 0; i < m_n1 + 1; i++) { m_c_hiddenOutput[i] = new float[m_n3]; }



}

NeuralPassato::NeuralPassato(const NeuralPassato& altro) :m_n(altro.m_n), m_n1(altro.m_n1), m_n2(altro.m_n2), m_n3(altro.m_n3), punteggio(altro.punteggio) {
	//std::cout << "STAI COPIANDO NEURAL PASSATO \n";

	m_c_imputHidden = new float* [m_n + 1]; //stai copiando i valori delle connessioni tra imput e hidden
	for (int i = 0; i < m_n + 1; i++) { m_c_imputHidden[i] = new float[m_n1]; }
	for (int i = 0; i < m_n + 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = altro.m_c_imputHidden[i][j];
		}
	}

	m_c_hidden = new float** [m_n2 - 1]; //stai copiando i valori delle connessioni tra i vali layer hidden
	for (int i = 0; i < m_n2 - 1; i++) { m_c_hidden[i] = new float* [m_n1 + 1]; for (int j = 0; j < m_n1 + 1; j++) { m_c_hidden[i][j] = new float[m_n1]; } }
	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = altro.m_c_hidden[i][j][k];
			}
		}
	}

	m_c_hiddenOutput = new float* [m_n1 + 1]; //stai copiando i valori delle connessioni tra l'ultimo layer hidden e l'output
	for (int i = 0; i < m_n1 + 1; i++) { m_c_hiddenOutput[i] = new float[m_n3]; }
	for (int i = 0; i < m_n1 + 1; i++) {
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = altro.m_c_hiddenOutput[i][j];
		}
	}
}

NeuralPassato::~NeuralPassato() {

	for (int i = 0; i < m_n + 1; i++) { delete[] m_c_imputHidden[i]; }
	delete[] m_c_imputHidden;

	for (int i = 0; i < m_n2 - 1; i++) { for (int j = 0; j < m_n1 + 1; j++) { delete[] m_c_hidden[i][j]; } delete[] m_c_hidden[i]; }
	delete[] m_c_hidden;

	for (int i = 0; i < m_n1 + 1; i++) { delete[] m_c_hiddenOutput[i]; }
	delete[] m_c_hiddenOutput;

}

bool NeuralPassato::operator < (const NeuralPassato& altro) const {
	return punteggio < altro.punteggio;
}

bool NeuralPassato::operator > (const NeuralPassato& altro) const {
	return punteggio > altro.punteggio;
}
NeuralPassato& NeuralPassato::operator= (const NeuralPassato& altro) {//controlla qua
	punteggio = altro.punteggio;
	m_n = altro.m_n;
	m_n1 = altro.m_n1;
	m_n2 = altro.m_n2;
	m_n3 = altro.m_n3;

	//stai copiando i valori delle connessioni tra imput e hidden

	for (int i = 0; i < m_n + 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = altro.m_c_imputHidden[i][j];
		}
	}

	//stai copiando i valori delle connessioni tra i vali layer hidden
	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = altro.m_c_hidden[i][j][k];
			}
		}
	}

	//stai copiando i valori delle connessioni tra l'ultimo layer hidden e l'output
	for (int i = 0; i < m_n1 + 1; i++) {
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = altro.m_c_hiddenOutput[i][j];
		}
	}

	return *this;
}
//funzioni---------------------------------------------------------------------------------------------
void NeuralPassato::salvaconnessioni(neural& altro) {

	float** altro_imput_hidden = altro.ritorna_pesi_imput_hidden();
	float** altro_hidden_output = altro.ritorna_pesi_hidden_outout();
	float*** altro_hidden = altro.ritorna_pesi_hidden();

	for (int i = 0; i < m_n + 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = altro_imput_hidden[i][j];
		}
	}

	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = altro_hidden[i][j][k];
			}
		}
	}

	for (int i = 0; i < m_n1 + 1; i++) {
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = altro_hidden_output[i][j];
		}
	}
	punteggio = altro.ritorna_punteggio();
}

float** NeuralPassato::ritorna_pesi_imput_hidden() const {
	return m_c_imputHidden;
}

float** NeuralPassato::ritorna_pesi_hidden_outout() const {
	return m_c_hiddenOutput;
}

float*** NeuralPassato::ritorna_pesi_hidden() const {
	return m_c_hidden;
}

float NeuralPassato::rpunteggio() const {
	return punteggio;
}

//questa serve solo per dei controlli
void NeuralPassato::printa() const {

	std::cout << "\n\nPESI: (l'ultimo neurone e' sempre il bias)\nIMPUT->HIDDEN:\n";

	for (int i = 0; i < m_n + 1; i++) {
		std::cout << "Neurone: " << i << " ";
		for (int j = 0; j < m_n1; j++) {
			std::cout << m_c_imputHidden[i][j] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "\n\nHIDDEN->HIDDEN:\n";
	for (int i = 0; i < m_n2 - 1; i++) {
		std::cout << "Tra Layer: " << i << "->" << i + 1 << "\n";
		for (int j = 0; j < m_n1 + 1; j++) {
			std::cout << "Neurone: " << j << " ";
			for (int k = 0; k < m_n1; k++) {
				std::cout << m_c_hidden[i][j][k] << " ";
			}
			std::cout << "\n";
		}
	}
	std::cout << "\n\nUltimo HIDDEN->OUTPUT:\n";
	for (int i = 0; i < m_n1 + 1; i++) {
		std::cout << "Neurone: " << i << " ";
		for (int j = 0; j < m_n3; j++) {
			std::cout << m_c_hiddenOutput[i][j] << " ";
		}
		std::cout << "\n";
	}

}