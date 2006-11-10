#define INFINITO 999999999

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

        void calcularRotas();
        void calcularCustos();

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

int Grafo::acharCentroMaisProximo(int i) {
	double *centro, menor_custo;
        int mais_proximo;

        menor_custo = INFINITO;
        mais_proximo = 0;
        centro = new double[NumeroCentros];
	for (int count = 0; count < NumeroCentros; count++) {
		centro[count] = Custo[Centro[count]][i];
        }

        for (int count = 0; count < NumeroCentros; count++) {
        	if (centro[count] < menor_custo) {
                	menor_custo = centro[count];
                        mais_proximo = count;
                }
        }
        delete[] centro;
        return Centro[mais_proximo];
}


int Grafo::acharCentroEmergenciaMaisProximo(int i) {
	double *centro, menor_custo;
        int mais_proximo;

        menor_custo = INFINITO;
        mais_proximo = 0;
        centro = new double[NumeroCentrosEmergencia];
	for (int count = 0; count < NumeroCentrosEmergencia; count++) {
		centro[count] = Custo[i][CentroEmergencia[count]];
        }

        for (int count = 0; count < NumeroCentrosEmergencia; count++) {
        	if (centro[count] < menor_custo) {
                	menor_custo = centro[count];
                        mais_proximo = count;
                }
        }
        delete[] centro;
        return CentroEmergencia[mais_proximo];
}

Grafo::Grafo(char *arquivo) {
        FILE *fp;

        // Abrindo o arquivo com o grafo
        fp = fopen(arquivo, "r");
        fscanf(fp, "%d %d", &NumeroVertices, &NumeroArestas);

        // Cria as estruturas que vão armazenar os vértices e arestas do grafo.
        Aresta = new int*[NumeroArestas+1];
        for (int i = 1; i <= NumeroArestas; i++)
                 Aresta[i] = new int[2];
        Vertice = new TPoint[NumeroVertices+1];


        // Preenche a matriz de vertice
	for (int count = 1; count <= NumeroVertices; count++) {
                int n, x, y;
                fscanf(fp,"%d%d%d", &n, &x, &y);
                Vertice[n].x = x;
                Vertice[n].y = y;
        }

        // Lê as informações sobre as arestas. Subtraimos um para que fique tudo
        // no lugar certo dentro da matriz.
        for (int count = 1; count <= NumeroArestas; count++) {
                int n, origem, destino;
                fscanf(fp, "%d%d%d", &n, &origem, &destino);
                Aresta[n][0] = origem;
                Aresta[n][1] = destino;
                n++;
        }

        Custo = NULL;
        Rota = NULL;
        Centro = NULL;
        CentroEmergencia = NULL;

        // Não precisamos mais do arquivo, vamos fechá-lo
	fclose(fp);
}

Grafo::~Grafo() {
	if (Custo != NULL) {
	        for (int i = 1; i <= NumeroVertices; i++)
        	         delete[] Custo[i];
	        delete[] Custo;
        }

        if (Rota != NULL) {
	        for (int i = 1; i <= NumeroVertices; i++)
        	         delete[] Rota[i];
	        delete[] Rota;
        }

        delete[] Vertice;

        for (int i = 1; i <= NumeroArestas; i++)
                 delete[] Aresta[i];
        delete[] Aresta;

	if (Centro != NULL)
	        delete[] Centro;

        if (CentroEmergencia != NULL)
	        delete[] CentroEmergencia;
}

void Grafo::calcularCustos() {
        Custo = new double*[NumeroVertices+1];
        for (int i = 1; i <= NumeroVertices; i++)
                 Custo[i] = new double[NumeroVertices+1];

       	// Preenche a matriz de custo com valores infinitos
	for (int i = 1; i <= NumeroVertices; i++)
		for (int j = 1; j <= NumeroVertices; j++)
			Custo[i][j] = INFINITO;

	for (int i = 1; i <= NumeroArestas; i++) {
                int origem, destino;
                origem = Aresta[i][0];
                destino = Aresta[i][1];
              	Custo[origem][destino] = sqrt(pow((Vertice[origem].x - Vertice[destino].x), 2) + pow((Vertice[origem].y - Vertice[destino].y), 2));
        }
}

void Grafo::calcularRotas() {
	if (Custo == NULL)
        	return;

        Rota = new int*[NumeroVertices+1];
        for (int i = 1; i <= NumeroVertices; i++)
                Rota[i] = new int[NumeroVertices+1];

        // Calcula da tabela de rota inicial. Para o vertice i cujo custo para
        // ir ao vertice j é diferente do valor máximo, sabe-se que existe uma
        // rota, portanto nós a colocamos na matriz de rotas.
	for (int i = 1; i <= NumeroVertices; i++) {
		for (int j = 1; j <= NumeroVertices; j++) {
			if (Custo[i][j] < INFINITO)
				Rota[i][j] = j;
                        else
				Rota[i][j] = 0;
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

void Grafo::gerarCentrosEmergencia(int n) {
        // Inicio do calculo do centro de emergencia.
        double *esr, maior = 0;

        if (Custo == NULL)
        	return;

        esr = new double[NumeroVertices + 1];
        NumeroCentrosEmergencia = n;
        CentroEmergencia = new int[NumeroCentrosEmergencia];

        for (int i = 1; i <= NumeroVertices; i++) {
	   	maior = Custo[i][1] + Custo[1][i];
                for (int j = 1; j <= NumeroVertices; j++) {
	    	        double soma = Custo[i][j] + Custo[j][i];
                        if (soma > maior)
                            	maior = soma;
                }
                esr[i] = maior;
        }

       	double menor = esr[1];
        for (int i = 1; i <= NumeroVertices; i++) {
	        if (esr[i] < menor) {
                       	menor = esr[i];
               		CentroEmergencia[0] = i;
                }
        }

        for (int i = 1, x = 1; i < NumeroCentrosEmergencia; i++) {
        	x += (NumeroVertices / NumeroCentrosEmergencia);
		do {
			randomize();
			CentroEmergencia[i] = x + (rand() % (NumeroVertices / NumeroCentrosEmergencia));
		} while (CentroEmergencia[i] == x);
        }
        delete[] esr;
}

void Grafo::gerarCentros(int n) {
        double *es, *er, maior = 0;

        if (Custo == NULL)
        	return;

        NumeroCentros = n;
        Centro = new int[NumeroCentros];

        es = new double[NumeroVertices + 1];
        for (int i = 1; i <= NumeroVertices; i++) {
	   	maior = Custo[i][1];
                for (int j = 1; j <= NumeroVertices; j++) {
                        if (Custo[i][j] > maior)
                            	maior = Custo[i][j];
                }
                es[i] = maior;
        }

        er = new double[NumeroVertices + 1];
        for (int i = 1; i <= NumeroVertices; i++) {
	   	maior = Custo[1][i];
                for (int j = 1; j <= NumeroVertices; j++) {
                        if (Custo[i][j] > maior)
                            	maior = Custo[i][j];
                }
                er[i] = maior;
        }


       	double menorS = es[1];
	double menorR = er[1];
        for (int i = 1; i <= NumeroVertices; i++) {
		if (es[i] < menorS && er[i] < menorR) {
                      	menorS = es[i];
                        menorR = er[i];
                	Centro[0] = i;
                }
        }

        for (int i = 1, x = 1; i < NumeroCentros; i++) {
        	x += (NumeroVertices / NumeroCentros);
		do {
			randomize();
			Centro[i] = x + (rand() % (NumeroVertices / NumeroCentros));
		} while (Centro[i] == x);
        }

        delete[] er;
        delete[] es;

}

int Grafo::pegarCentro(int i) {
	if (Centro == NULL)
        	return 0;
        return Centro[i];
}


int Grafo::pegarNumeroCentros() {
	return NumeroCentros;
}

int Grafo::pegarCentroEmergencia(int i) {
	if (CentroEmergencia == NULL)
        	return 0;
        return CentroEmergencia[i];
}

int Grafo::pegarNumeroCentrosEmergencia() {
        return NumeroCentrosEmergencia;
}

TPoint Grafo::pegarPonto(int i) {
        return Vertice[i];
}

int *Grafo::acharMelhorCaminho(int origem, int destino) {
	int *Caminho, count, vertice_anterior;
        Caminho = new int[NumeroVertices+1];


	count = 0;
        Caminho[count] = origem;
        do {
        	vertice_anterior = Caminho[count];
                count++;
                Caminho[count] = Rota[vertice_anterior][destino];
        } while (Caminho[count] != destino);
        return Caminho;
}

int *Grafo::pegarAresta(int i) {
        int *temp;
        temp = new int[2];
        temp[0] = Aresta[i][0];
        temp[1] = Aresta[i][1];
        return temp;
}

int Grafo::pegarNumeroArestas() {
        return NumeroArestas;
}

int Grafo::pegarNumeroVertices() {
        return NumeroVertices;
}

