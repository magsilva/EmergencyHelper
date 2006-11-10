#define INFINITO 999999999

/* Grafo armazenado na forma de uma matriz de adjacência. Todas as matrizes, arrays
e afins são contados a partir de 1. */
class Grafo {
private:
	int **Rota;
	double **Custo;
	TPoint *Vertice;
	int **Aresta;
	int NumeroVertices;
	int NumeroArestas;

	int *Centro;
	int NumeroCentros;

	int *CentroEmergencia;
	int NumeroCentrosEmergencia;

public:
	Grafo(char *arquivo);
	~Grafo();

	void aplicarFloyd();

	int *acharMelhorCaminho(int origem, int destino);

  	void gerarCentros(int n);
	int pegarCentro(int i);
	int pegarNumeroCentros();
	int acharCentroMaisProximo(int i);

	void gerarCentrosEmergencia(int n);
	int pegarCentroEmergencia(int i);
	int pegarNumeroCentrosEmergencia();
	int acharCentroEmergenciaMaisProximo(int i);

	TPoint pegarPonto(int i);
	int pegarNumeroVertices();

	int *pegarAresta(int i);
	int pegarNumeroArestas();
};

/* Acha o centro mais proximo do vertice numero i. Retorna 0 se não achar. */
int Grafo::acharCentroMaisProximo(int i) {
	double *centro, menor_custo;
    int mais_proximo;

    if (Centro == NULL) {
    	return 0;
    }

    // Se necessário, aplicar Floyd para obter os custos.
    if (Custo == NULL) {
    	aplicarFloyd();
    }

    menor_custo = INFINITO;
    mais_proximo = 0;
    centro = new double[NumeroCentros + 1];

    // Calculo da distancia entre os centros e o vertice em questao
	for (int count = 1; count <= NumeroCentros; count++) {
		centro[count] = Custo[Centro[count]][i];
    }

    // Escolha do vertice mais proximo do vertice em questao
    for (int count = 1; count <= NumeroCentros; count++) {
       	if (centro[count] < menor_custo) {
           	menor_custo = centro[count];
            mais_proximo = count;
		}
    }
    delete[] centro;
	return Centro[mais_proximo];
}

/* Acha o centro de emergencia mais proximo do vertice numero i. Retorna 0
se não achar. */
int Grafo::acharCentroEmergenciaMaisProximo(int i) {
	double *centro, menor_custo;
	int mais_proximo;

    if (CentroEmergencia == NULL) {
    	return 0;
	}

    // Se necessário, aplicar Floyd para obter os custos.
    if (Custo == NULL) {
    	aplicarFloyd();
    }

    menor_custo = INFINITO;
    mais_proximo = 0;
    centro = new double[NumeroCentrosEmergencia + 1];

    // Calculo da distancia entre os centros e o vertice em questao
	for (int count = 1; count <= NumeroCentrosEmergencia; count++) {
		centro[count] = Custo[i][CentroEmergencia[count]];
    }

    // Escolha do vertice mais proximo do vertice em questao
    for (int count = 1; count <= NumeroCentrosEmergencia; count++) {
      	if (centro[count] < menor_custo) {
           	menor_custo = centro[count];
            mais_proximo = count;
        }
    }
    delete[] centro;
    return CentroEmergencia[mais_proximo];
}

/* Construtura do grafo. Inicializa-o com os dados contido no arquivo (cujo nome
é passado como parâmetro */
Grafo::Grafo(char *arquivo) {
	FILE *fp;

    // Abrindo o arquivo com o grafo
    fp = fopen(arquivo, "r");
    fscanf(fp, "%d %d", &NumeroVertices, &NumeroArestas);

    // Cria as estruturas que vão armazenar os vértices e arestas do grafo.
    Aresta = new int*[NumeroArestas+1];
    for (int i = 1; i <= NumeroArestas; i++) {
    	Aresta[i] = new int[2];
    }
    Vertice = new TPoint[NumeroVertices + 1];


    // Preenche a matriz de vertice.
	for (int count = 1; count <= NumeroVertices; count++) {
    	int n, x, y;
        fscanf(fp,"%d%d%d", &n, &x, &y);
        Vertice[n].x = x;
        Vertice[n].y = y;
    }

    // Lê as informações sobre as arestas.
	for (int count = 1; count <= NumeroArestas; count++) {
		int n, origem, destino;
        fscanf(fp, "%d%d%d", &n, &origem, &destino);
        Aresta[n][0] = origem;
        Aresta[n][1] = destino;
	}

    // Inicialização das estruturas não essenciais.
	Custo = NULL;
	Rota = NULL;
    Centro = NULL;
    CentroEmergencia = NULL;

	// Não precisamos mais do arquivo, vamos fechá-lo
	fclose(fp);
}

/* Destrutura do grafo. Libera toda a memória utilizada. */
Grafo::~Grafo() {
	if (Custo != NULL) {
		for (int i = 1; i <= NumeroVertices; i++) {
			delete[] Custo[i];
        }
        delete[] Custo;
    }

    if (Rota != NULL) {
		for (int i = 1; i <= NumeroVertices; i++) {
        	delete[] Rota[i];
        }
	    delete[] Rota;
    }

    if (Vertice != NULL) {
	    delete[] Vertice;
    }

    if (Aresta != NULL) {
	    for (int i = 1; i <= NumeroArestas; i++) {
    		delete[] Aresta[i];
	    }
    	delete[] Aresta;
    }

	if (Centro != NULL) {
		delete[] Centro;
    }

	if (CentroEmergencia != NULL) {
		delete[] CentroEmergencia;
    }
}

/* Aplica o algoritmo de Floyd, calculando-se assim os custos e as rotas entre todos
os vértices do grafo. */
void Grafo::aplicarFloyd() {

	// Se existia uma estrutura de custo anterior, retorna sem fazer nada.
	if (Custo != NULL || Rota != NULL) {
    	return;
    }

    // Cria uma nova matriz de custo.
	Custo = new double*[NumeroVertices+1];
	for (int i = 1; i <= NumeroVertices; i++) {
		Custo[i] = new double[NumeroVertices+1];
    }

 	// Preenche a matriz de custo com valores infinitos.
	for (int i = 1; i <= NumeroVertices; i++) {
		for (int j = 1; j <= NumeroVertices; j++) {
			Custo[i][j] = INFINITO;
        }
    }

    // Calcula o custo entre os vértices que estão relacionados com outros vértices.
	for (int i = 1; i <= NumeroArestas; i++) {
    	int origem, destino;
        origem = Aresta[i][0];
        destino = Aresta[i][1];
        Custo[origem][destino] = sqrt(pow((Vertice[origem].x - Vertice[destino].x), 2) + pow((Vertice[origem].y - Vertice[destino].y), 2));
    }

    // Cria uma nova matriz de rotas
	Rota = new int*[NumeroVertices+1];
	for (int i = 1; i <= NumeroVertices; i++)
                Rota[i] = new int[NumeroVertices+1];

	// Calcula da tabela de rota inicial. Para o vertice i cujo custo para
	// ir ao vertice j é diferente do valor máximo, sabe-se que existe uma
	// rota, portanto nós a colocamos na matriz de rotas.
	for (int i = 1; i <= NumeroVertices; i++) {
		for (int j = 1; j <= NumeroVertices; j++) {
			if (Custo[i][j] < INFINITO) {
				Rota[i][j] = j;
            } else {
				Rota[i][j] = 0;
            }
        }
	}

	// Calculo da matriz de custo e de rotas final, através de Floyd.
	for (int k = 1; k <= NumeroVertices; k++) {
		for (int i = 1; i <= NumeroVertices; i++) {
			for (int j = 1; j <= NumeroVertices; j++) {
				if  ((Custo[i][k] + Custo[k][j]) < Custo[i][j]) {
					Custo[i][j] = Custo[i][k] + Custo[k][j];
				 	Rota[i][j] = Rota[i][k];
				}
            }
        }
    }
}

/* Cria n centros de emergência */
void Grafo::gerarCentrosEmergencia(int n) {
	double *esr; // excentricidade de entrada e saída

	// Se os centros de emergência já foram calculados, apagar o resultado anterior.
	if (CentroEmergencia != NULL) {
		delete[] CentroEmergencia;
    }

	// Calcula os dados necessários para a criação dos centros de emergência.
	if (Custo == NULL) {
       	aplicarFloyd();
	}

	// Aloca memória para armazenar o valor da excentricidade de entrada e saída
    // de cada vértice.
    esr = new double[NumeroVertices + 1];

    // Guarda o número de centros de emergência que vão ser gerados.
    NumeroCentrosEmergencia = n;

    // Cria a estrutura para armazenar os centros de emergência que vão ser
    // localizados.
    CentroEmergencia = new int[NumeroCentrosEmergencia + 1];

	// Calcula a excentricidade de entrada e saída de cada vértice do grafo.
    double maior = 0;
    for (int i = 1; i <= NumeroVertices; i++) {
	  	maior = Custo[i][1] + Custo[1][i];
        for (int j = 1; j <= NumeroVertices; j++) {
	        double soma = Custo[i][j] + Custo[j][i];
            if (soma > maior) {
               	maior = soma;
			}
        }
        esr[i] = maior;
    }

	// Acha o centro de emergência do grafo.
    double menor = esr[1];
    for (int i = 1; i <= NumeroVertices; i++) {
		if (esr[i] < menor) {
           	menor = esr[i];
           	CentroEmergencia[1] = i;
        }
    }

    // Libera a estrutura utilizada para armazenar as excentricidades.
    delete[] esr;

    // Gera os demais centros de emergência de maneira quase aleatória.
    for (int i = 2, x = 1; i <= NumeroCentrosEmergencia; i++, x += (NumeroVertices / NumeroCentrosEmergencia)) {
		do {
			randomize();
			CentroEmergencia[i] = x + (rand() % (NumeroVertices / NumeroCentrosEmergencia));
		} while (CentroEmergencia[i] == x && CentroEmergencia[i] != CentroEmergencia[1]);
    }
}

/* Cria n centros */
void Grafo::gerarCentros(int n) {
	double  maior, // varíável auxiliar no cálculo das excentricidades
    		*es, // excentricidade de saída
    		*er; // excentricidade de retorno

	// Se os centros já foram calculados anteriormente, descarta os valores anteriores.
    if (Centro != NULL) {
		delete[] Centro;
    }

	// Caso necessário, calcula os dados requeridos para o cálculo dos centros.
	if (Custo == NULL) {
       	aplicarFloyd();
	}

    // Guarda o número de centros a serem gerados.
	NumeroCentros = n;

    // Cria a estrutura que armazenará os centros a serem localizados.
	Centro = new int[NumeroCentros + 1];

    // Inicializa a estrutura que armazenará as excentricidades de saída.
	es = new double[NumeroVertices + 1];

    // Calcula a excentricidade de saída de todos os vértices do grafo.
    for (int i = 1; i <= NumeroVertices; i++) {
		maior = Custo[i][1];
        for (int j = 1; j <= NumeroVertices; j++) {
    		if (Custo[i][j] > maior) {
            	maior = Custo[i][j];
            }
        }
        es[i] = maior;
    }

    // Inicializa a estrutura que armazenará as excentricidades de retorno.
	er = new double[NumeroVertices + 1];

    // Calcula a excentricidade de retorno de todos os vértices de grafo.
	for (int i = 1; i <= NumeroVertices; i++) {
	   	maior = Custo[1][i];
		for (int j = 1; j <= NumeroVertices; j++) {
			if (Custo[i][j] > maior) {
				maior = Custo[i][j];
            }
        }
		er[i] = maior;
    }


    // Calculo do centro do grafo.
    double menorS = es[1];
	double menorR = er[1];
    for (int i = 1; i <= NumeroVertices; i++) {
		if (es[i] < menorS && er[i] < menorR) {
           	menorS = es[i];
        	menorR = er[i];
            Centro[1] = i;
        }
    }
	// Não vamos precisar mais destas estruturas, então vamos removê-las.
    delete[] er;
    delete[] es;

    for (int i = 2, x = 1; i <= NumeroCentros; i++, x += (NumeroVertices / NumeroCentros)) {
		do {
			randomize();
			Centro[i] = x + (rand() % (NumeroVertices / NumeroCentros));
		} while (Centro[i] == x && Centro[i] != Centro[1]);
	}
}

/* Retorna o i-nésimo centro do grafo. Retorna 0 se não existir. */
int Grafo::pegarCentro(int i) {

	// Antes de qualquer coisa, i tem de ser maior que 0
    if (i < 1) {
    	return 0;
    }

	// Se os centros não foram calculados, calculemo-os.
	if (Centro == NULL) {
		gerarCentros(i+1);
    }

    // Vamos verificar se existe o i-nésimo centro. Caso não exista, retorna zero.
    if (i <= NumeroCentros) {
    	return Centro[i];
    } else {
    	return 0;
    }
}


/* Retorna o total de centros do grafo que temos calculados. */
int Grafo::pegarNumeroCentros() {
	return NumeroCentros;
}

/* Retorna o i-nésimo centro de emergência do grafo. Retorna 0 se não existir. */
int Grafo::pegarCentroEmergencia(int i) {

	// Antes de qualquer coisa, i tem de ser maior que 0.
    if (i < 1) {
    	return 0;
    }

	// Se os centros de emergência não foram calculados, calculemo-os.
	if (CentroEmergencia == NULL) {
		gerarCentrosEmergencia(i+1);
    }

    // Vamos verificar se existe o i-nésimo centro. Caso não exista, retorna zero.
    if (i <= NumeroCentrosEmergencia) {
    	return CentroEmergencia[i];
    } else {
    	return 0;
    }
}

/* Retorna o total de centros de emergência do grafo que temos calculados. */
int Grafo::pegarNumeroCentrosEmergencia() {
    return NumeroCentrosEmergencia;
}

/* Retorna as coordenadas do i-nésimo vértice. Retorna um ponto com coordenadas
negativas se não existir.*/
TPoint Grafo::pegarPonto(int i) {
	// Se for um vértice inexistente, retorna NULL.
  	if (i > NumeroVertices || i < 1) {
    	TPoint p;
        p.x = -1;
        p.y = -1;
        return p;
    }

	return Vertice[i];
}

/* Acha o melhor caminho entre o vértice origem e o destino. É necessário que
seja desalocada a memória ocupada por *Caminho posteriormente. */
int *Grafo::acharMelhorCaminho(int origem, int destino) {
	int *Caminho, vertice_anterior;

    // Aloca memória suficiente para armazenar o caminho máximo a ser percorrido.
	Caminho = new int[NumeroVertices+1];

	int count = 0;
	Caminho[count] = origem;
    do {
    	vertice_anterior = Caminho[count];
        count++;
        Caminho[count] = Rota[vertice_anterior][destino];
    } while (Caminho[count] != destino);
    return Caminho;
}

/* Retorna a i-nésimo aresta. É necessário que seja desalocada a memória ocupada
por *aresta posteriormente. Se i for uma aresta inexistente, retorna NULL. */
int *Grafo::pegarAresta(int i) {
	// Se for uma aresta inexistente, retorna NULL.
	if (i > NumeroArestas || i < 1) {
		return NULL;
    }

	int *aresta;
   	aresta = new int[2];
	aresta[0] = Aresta[i][0];
	aresta[1] = Aresta[i][1];
	return aresta;
}

int Grafo::pegarNumeroArestas() {
	return NumeroArestas;
}

int Grafo::pegarNumeroVertices() {
	return NumeroVertices;
}
