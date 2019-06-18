/*
 * arvore.h
 *
 *  Created on: 23/09/2009
 *      Author: wagner
 */

#include "util.h"

typedef struct vertice {
	char* chave;
	struct vertice* esq;
	struct vertice* dir;
} Vertice;

typedef struct itemVetor {
	char* chave;
	int peso;
	int custo;
} EstruturaItemVetor;

typedef struct vetorCondensado{
	EstruturaItemVetor* vetor;
	int tamanho;
} EstruturaVetorCondensado;

Vertice* raiz;
int pesoArvore = 0;
int alturaArvore = 0;

void criaArvore() {
	raiz = malloc(sizeof(Vertice));
	raiz->esq = NULL;
	raiz->dir = NULL;

	if (!raiz) {
		printf("\n\n\tMemória insuficiente!\n\n");
		exit(1);
	}
}

void imprimeArvore(Vertice* atual) {
	if (!atual) {
		return;
	}
	imprimeArvore(atual->esq);
	printf("\n\tTermo: \t%s\n\n", atual->chave);
	imprimeArvore(atual->dir);
}

int calculaCustoTermo(char* termo, Vertice* atual){
	int resultado = strcmp(termo, atual->chave);
	if(resultado == 0){
		return 1;
	} else if (resultado == 1){
		return 1 + calculaCustoTermo(termo, atual->dir);
	} else {
		return 1 + calculaCustoTermo(termo, atual->esq);
	}
}

int calculaCustoArvore(EstruturaVetorCondensado EVetorCondensado) {
	int custoArvore = 0;
	int contador;

	for (contador = 0; contador < EVetorCondensado.tamanho; contador++){
		EVetorCondensado.vetor[contador].custo = calculaCustoTermo(EVetorCondensado.vetor[contador].chave, raiz);
		custoArvore += (EVetorCondensado.vetor[contador].peso * EVetorCondensado.vetor[contador].custo);
	}
	return custoArvore;
}

void calculaProfundidade(Vertice* atual, int alturaAtual) {
	if (!atual) {
		return;
	}
	if (alturaAtual > alturaArvore) {
		alturaArvore = alturaAtual;
	}
	calculaProfundidade(atual->esq, alturaAtual++);
	calculaProfundidade(atual->dir, alturaAtual++);
}

void insere(char* valor) {
	Vertice* novo = malloc(sizeof(Vertice));
	novo->chave = valor;
	novo->dir = NULL;
	novo->esq = NULL;

	if (!raiz) {
		raiz = novo;
		return;
	}
	Vertice* atual = raiz; // começo pela raiz
	Vertice* pai; // guardo o pai de cada um
	while (1) {
		pai = atual;
		if (strcmp(novo->chave, atual->chave) == -1) { // vai para esquerda?
			atual = atual->esq;
			if (!atual) {
				pai->esq = novo;
				return;
			}
		} else if (strcmp(novo->chave, atual->chave) == 1) {
			atual = atual->dir; // vai para direita?
			if (!atual) { // fim da linha?
				pai->dir = novo; // insere aqui
				return;
			}
		} else {
			return;
		}
	}
}

void criaArvoreBalanceada(EstruturaVetorCondensado EVetorCondensado, int inicio, int fim) {
	if (fim < inicio) {
		return;
	}
	int meio = (inicio + fim) / 2;
	insere((char*) EVetorCondensado.vetor[meio].chave);
	criaArvoreBalanceada(EVetorCondensado, inicio, meio - 1);
	criaArvoreBalanceada(EVetorCondensado, meio + 1, fim);
}

EstruturaVetorCondensado ordenaDecrescentePorPeso(EstruturaVetorCondensado EVetorCondensado, int tamanhoVetor){
	int contador1, contador2, maximo;
	EstruturaItemVetor* auxiliar = (EstruturaItemVetor*)malloc(sizeof(EstruturaItemVetor));

	for (contador1 = 0; contador1 < tamanhoVetor - 1; contador1++) {
		maximo = contador1;
		for (contador2 = maximo + 1; contador2 < tamanhoVetor; contador2++) {
			if (EVetorCondensado.vetor[maximo].peso < EVetorCondensado.vetor[contador2].peso) {
				maximo = contador2;
			}
		}
		if (maximo != contador1) {
			*auxiliar = EVetorCondensado.vetor[contador1];
			EVetorCondensado.vetor[contador1] = EVetorCondensado.vetor[maximo];
			EVetorCondensado.vetor[maximo] = *auxiliar;
		}
	}
	return EVetorCondensado;
}

void heuristicaArvoreDePesquisaOtima(EstruturaVetorCondensado EVetorCondensado) {
	int contador;
	EVetorCondensado = ordenaDecrescentePorPeso(EVetorCondensado,	EVetorCondensado.tamanho);

	for (contador = 0; contador < EVetorCondensado.tamanho; contador++) {
		insere(EVetorCondensado.vetor[contador].chave);
	}

	return;
}

EstruturaVetorCondensado condensaVetorOrdenado(char** vetorOrdenado, EstruturaArquivo EArquivo) {
	int auxiliar1;
	int auxiliar2;
	int auxiliar3 = 0;

	EstruturaVetorCondensado EVetorCondensado;
	EVetorCondensado.vetor = (EstruturaItemVetor*) malloc(EArquivo.quantidadeLinhas * sizeof(EstruturaItemVetor));

	for (auxiliar1 = 0; auxiliar1 < EArquivo.quantidadeLinhas; ) {
		for (auxiliar2 = auxiliar1;(auxiliar2 < EArquivo.quantidadeLinhas) && (strcmp(vetorOrdenado[auxiliar1], vetorOrdenado[auxiliar2]) == 0) ; auxiliar2++);
		EVetorCondensado.vetor[auxiliar3].chave = vetorOrdenado[auxiliar1];
		EVetorCondensado.vetor[auxiliar3].peso = auxiliar2 - auxiliar1;
		auxiliar3++;
		auxiliar1 = auxiliar2;
	}
	EVetorCondensado.tamanho = auxiliar3;
	return EVetorCondensado;
}

void ArvoreDePesquisaOtimaForcaBruta(EstruturaVetorCondensado EVetorCondensado){
	printf("\n\n\tAlgoritmo não implementado.\n\n");
	exit(1);
}



















