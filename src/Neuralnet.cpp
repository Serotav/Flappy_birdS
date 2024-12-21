#include "Neuralnet.h"
//cose statiche -------------------------------------------------
std::random_device neural::dev;
std::mt19937 neural::rng(dev());
std::uniform_real_distribution<float> neural::range(-1, 1);
std::uniform_real_distribution<float> neural::range_mutazioni(1 - RANGE_MUTAZIONI, 1 + RANGE_MUTAZIONI);

//costruttori --------------------------------------------------
neural::neural(unsigned n_neuroni_imput, unsigned n_neuroni_ogni_hidden, unsigned n_layer_hidden, unsigned n_neuroni_output) :
	m_n(n_neuroni_imput), m_n1(n_neuroni_ogni_hidden), m_n2(n_layer_hidden), m_n3(n_neuroni_output), m_c(m_n* m_n1 + m_n1), m_c1((m_n1 + 1)* m_n1), m_c2((m_n1 + 1)* m_n3), m_inizio(ORARIO), m_isvivo(true)
{
	m_n_imput = new float[m_n + 1];
	m_n_output = new float[m_n3];

	m_n_hidden = new float* [m_n2];
	for (int i = 0; i < m_n2; i++) { m_n_hidden[i] = new float[m_n1 + 1]; }

	m_c_imputHidden = new float* [m_n + 1];
	for (int i = 0; i < m_n + 1; i++) { m_c_imputHidden[i] = new float[m_n1]; }

	m_c_hidden = new float** [m_n2 - 1];
	for (int i = 0; i < m_n2 - 1; i++) { m_c_hidden[i] = new float* [m_n1 + 1]; for (int j = 0; j < m_n1 + 1; j++) { m_c_hidden[i][j] = new float[m_n1]; } }

	m_c_hiddenOutput = new float* [m_n1 + 1];
	for (int i = 0; i < m_n1 + 1; i++) { m_c_hiddenOutput[i] = new float[m_n3]; }

	randomizza_connessioni();
	setta_bias();
}

neural::neural(const neural& altro) : m_n(altro.m_n), m_n1(altro.m_n1), m_n2(altro.m_n2), m_n3(altro.m_n3), m_c(altro.m_c), m_c1(altro.m_c1), m_c2(altro.m_c2), m_inizio(altro.m_inizio), m_score(altro.m_score), m_isvivo(altro.m_isvivo)
{
	std::cout << "COPIATO OOOOOOOOOOOO SVEGLIA, CAZZO COPI???n";

	m_n_imput = new float[m_n + 1];
	memcpy(m_n_imput, altro.m_n_imput, sizeof(float) * (m_n + 1));	//stai anche copiando cosa c'� nei neuroni di imput

	m_n_output = new float[m_n3];
	memcpy(m_n_output, altro.m_n_output, sizeof(float) * (m_n3));	//stai anche copiando cosa c'� nei neuroni di output

	m_n_hidden = new float* [m_n2];	//NON, RIPETO NON stai copiando cosa c'� nei neuroni hidden
	for (int i = 0; i < m_n2; i++) { m_n_hidden[i] = new float[m_n1 + 1]; }


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

	setta_bias();
}

neural::~neural() {

	delete[] m_n_imput;
	delete[] m_n_output;

	for (int i = 0; i < m_n2; i++) { delete[] m_n_hidden[i]; }
	delete[] m_n_hidden;

	for (int i = 0; i < m_n + 1; i++) { delete[] m_c_imputHidden[i]; }
	delete[] m_c_imputHidden;

	for (int i = 0; i < m_n2 - 1; i++) { for (int j = 0; j < m_n1 + 1; j++) { delete[] m_c_hidden[i][j]; } delete[] m_c_hidden[i]; }
	delete[] m_c_hidden;

	for (int i = 0; i < m_n1 + 1; i++) { delete[] m_c_hiddenOutput[i]; }
	delete[] m_c_hiddenOutput;

}

//funzioni private ------------------------------------------- >> << <>
void neural::randomizza_connessioni() {

	for (int i = 0; i < m_n + 1; i++) { //qua sta randomizzando le connessioni tra il layer di imput ed il primo hiddenlayer, i neuroni di imput sono m_n +1 (bias), i neuroni in goni strato hidden sono m_n1
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = range(rng);
		}
	}

	for (int i = 0; i < m_n1 + 1; i++) { //qua sta randomizzando le connessioni tra l'ultimo layer hidden e l'output, i neuroni nell'hidden sono m_n1 +1 (bias), i neuroni dell' output l sono m_n3
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = range(rng);
		}
	}

	for (int i = 0; i < m_n2 - 1; i++) { //qua sta randomizzando le connessioni all'interno dei layer hidden, i "layer" di connessioni sono m_n2 - 1 (numero strati -1), ogni strato ha m_n1 + 1 (bias), ed ognuno di questi fa una connessione con ognuno dei neuroni nello strato dopo m_n1
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = range(rng);
			}
		}
	}
}

void neural::setta_bias() {

	m_n_imput[m_n] = 1;

	for (int i = 0; i < m_n2; i++) {
		m_n_hidden[i][m_n1] = 1;
	}

}

float neural::activation_function(float numero) const {


	return 1 / (1 + exp(-numero)); // en.wikipedia.org/wiki/Logistic_function

}

void neural::azzera_neuroni() {

	//azzera i neuroni di output
	for (int i = 0; i < m_n3; i++) {
		m_n_output[i] = 0;
	}

	//azzera i nueroni hidden
	for (int i = 0; i < m_n2; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_n_hidden[i][j] = 0;
		}
	}

}

float neural::casualizza_mutazione() const {

	if (std::uniform_real_distribution<float>(0,1)(rng) < PROB_MUTAZIONE)
    	return range_mutazioni(rng);
	else
    	return 1.0f;

}

//funzioni pubbliche ---------------------------------------- >> << <>

float* neural::lavora(float new_imput[]) {

	azzera_neuroni();

	// assegna ai neuroni di imput, gli imput
	for (int i = 0; i < m_n; i++) {

		m_n_imput[i] = new_imput[i];
	}

	//dai neuroni di imput al primo strato hidden, la nuova idea � che sommiamo tutti i valori dei neuroni precedenti moltiplicati per i rispettivi pesi, 
	//e poi decidiamo se il neurone si attiva o meno
	for (int i = 0; i < m_n1; i++) {
		for (int j = 0; j < m_n; j++) {
			m_n_hidden[0][i] += m_n_imput[j] * m_c_imputHidden[j][i];
		}
		m_n_hidden[0][i] += m_n_imput[m_n] * m_c_imputHidden[m_n][i];
		m_n_hidden[0][i] = activation_function(m_n_hidden[0][i]);
	}

	// da uno strato hidden all'altro
	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			for (int k = 0; k < m_n1; k++) {
				//std::cout << "\n neurone " << m_n_hidden[i + 1][j] << " += " << m_n_hidden[i][k] <<" * " << m_c_hidden[i][k][j] <<  "\n";

				m_n_hidden[i + 1][j] += m_c_hidden[i][k][j] * m_n_hidden[i][k];

			}
			//std::cout << "\n neurone " << m_n_hidden[i + 1][j] << " += " << m_n_hidden[i][m_n1] << " * " << m_c_hidden[i][m_n1][j] << "\n";
			m_n_hidden[i + 1][j] += m_n_hidden[i][m_n1] * m_c_hidden[i][m_n1][j];
			m_n_hidden[i + 1][j] = activation_function(m_n_hidden[i + 1][j]);
		}
	}

	//dall'ultumo strato hidden all' output
	for (int i = 0; i < m_n3; i++) {
		for (int j = 0; j < m_n1; j++) {

			m_n_output[i] += m_n_hidden[m_n2 - 1][j] * m_c_hiddenOutput[j][i];
		}

		m_n_output[i] += m_n_hidden[m_n2 - 1][m_n1] * m_c_hiddenOutput[m_n1][i];
		m_n_output[i] = activation_function(m_n_output[i]);
	}

	return m_n_output;

}

float** neural::ritorna_pesi_imput_hidden() const {
	return m_c_imputHidden;
}

float** neural::ritorna_pesi_hidden_outout() const {
	return m_c_hiddenOutput;
}

float*** neural::ritorna_pesi_hidden() const {
	return m_c_hidden;
}

void neural::muta(neural& altro) {

	float** altro_imput_hidden = altro.ritorna_pesi_imput_hidden();
	float** altro_hidden_output = altro.ritorna_pesi_hidden_outout();
	float*** altro_hidden = altro.ritorna_pesi_hidden();

	//muta pesi imput_hidden
	for (int i = 0; i < m_n + 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = altro_imput_hidden[i][j] * casualizza_mutazione();
		}
	}

	//muta pesi hidden
	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = altro_hidden[i][j][k] * casualizza_mutazione();
			}
		}
	}

	//muta pesi hidden_output
	for (int i = 0; i < m_n1 + 1; i++) {
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = altro_hidden_output[i][j] * casualizza_mutazione();
		}
	}
	//prendiamo un'altro punto temporale di inizio per valutare la rete
	m_inizio = ORARIO;
	m_isvivo = true;
}

void neural::muta(NeuralPassato& altro) {
	float** altro_imput_hidden = altro.ritorna_pesi_imput_hidden();
	float** altro_hidden_output = altro.ritorna_pesi_hidden_outout();
	float*** altro_hidden = altro.ritorna_pesi_hidden();

	//muta pesi imput_hidden
	for (int i = 0; i < m_n + 1; i++) {
		for (int j = 0; j < m_n1; j++) {
			m_c_imputHidden[i][j] = altro_imput_hidden[i][j] * casualizza_mutazione();
		}
	}

	//muta pesi hidden
	for (int i = 0; i < m_n2 - 1; i++) {
		for (int j = 0; j < m_n1 + 1; j++) {
			for (int k = 0; k < m_n1; k++) {
				m_c_hidden[i][j][k] = altro_hidden[i][j][k] * casualizza_mutazione();
			}
		}
	}

	//muta pesi hidden_output
	for (int i = 0; i < m_n1 + 1; i++) {
		for (int j = 0; j < m_n3; j++) {
			m_c_hiddenOutput[i][j] = altro_hidden_output[i][j] * casualizza_mutazione();
		}
	}
	//prendiamo un'altro punto temporale di inizio per valutare la rete
	m_inizio = ORARIO;
	m_isvivo = true;


}

void neural::die() {
	if (m_isvivo) {
		m_isvivo = false;
		m_score = ORARIO - m_inizio;
	}
}

float neural::ritorna_punteggio() const {
	return m_score.count();
}

bool neural::ritorna_isvivo()const {
	return m_isvivo;
}

//funzioni inutili --------------------------------------
void neural::printa() const {
	//Parametri -----
	std::cout << "Numero neuroni di imput " << m_n << "\n" << "Numero neuroni in ogni layer hidden " << m_n1 << "\n" << "Numero layer hidden " << m_n2 << "\n" << "Numero neuroni output " << m_n3 << "\n\n" << "Numero connessioni tra n ed n1 " << m_c << "\n" << "Numero connessioni tra un hidden layer e l'altro " << m_c1 << "\n" << "Numeroconnessioni tra l'ultimo hidden layewr e l'output layer " << m_c2 << "\n\n";

	//Pesi -----
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

	//Valori neuroni -----
	std::cout << "\n\nContenuto neuroni:\nIMPUT:\n";

	for (int i = 0; i < m_n; i++) {
		std::cout << "Neurone: " << i << " " << m_n_imput[i] << "\n";

	}

	std::cout << "\n\n0 HIDDEN:\n";

	for (int i = 0; i < m_n1; i++) {
		std::cout << "Neurone: " << i << " " << m_n_hidden[0][i] << "\n";

	}

	std::cout << "\n\nHIDDEN da 1 in poi:\n";
	for (int i = 0; i < m_n2 - 1; i++) {// da uno strato hidden all'altro
		std::cout << "\nLayer" << i + 1 << "\n";
		for (int j = 0; j < m_n1; j++) {
			std::cout << "Neurone: " << j << ": ";
			std::cout << m_n_hidden[i + 1][j] << "\n";
		}


	}


}

void neural::casual(int numero) const {

	for (int i = 0; i < numero; i++) {

		std::cout << range(rng) << "\n";
	}
}