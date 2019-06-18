/*
 * main.c
 *
 *  Created on: 23/09/2009
 *      Author: wagner
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <pthread.h>
#include "arvore.h"

int ALG_ARVORE_BALANC = 1,
	ALG_ARVORE_OTIMA_ALE = 2,
	HEUR_ARVORE_QUASE_OTIMA = 3;

void imprimeResultado(EstruturaVetorCondensado EVetorCondensado, char* arquivoSaida) {
	int contador = 0;
	char c;
	FILE *arquivo;

	arquivo = fopen(arquivoSaida, "w");
	if (!arquivo) {
		printf("Erro na abertura do arquivo. Fim de programa.");
		exit(1);
	}

	printf("\n\tQuantidade de termos: \t%d", EVetorCondensado.tamanho);
	printf("\n\tCusto da arvore: \t%d", pesoArvore);
	printf("\n\tAltura da arvore: \t%d\n", alturaArvore);
	
	printf("\n\n\tDigite Enter pra continuar: ");
	scanf("%c", &c);
	printf("\n");

	for (; contador < EVetorCondensado.tamanho; contador++) {
		printf("\n\t%s", EVetorCondensado.vetor[contador].chave);
		printf(" \t\t%d", EVetorCondensado.vetor[contador].peso);
		printf(" %d", EVetorCondensado.vetor[contador].custo);
	}
	printf("\n");

	/*
	 * Imprime no arquivo de saída
	 * */
	fprintf(arquivo, "\n\tQuantidade de termos: \t%d", EVetorCondensado.tamanho);
	fprintf(arquivo, "\n\tCusto da arvore: \t%d", pesoArvore);
	fprintf(arquivo, "\n\tAltura da arvore: \t%d\n", alturaArvore);
	for (contador = 0; contador < EVetorCondensado.tamanho; contador++) {
		fprintf(arquivo, "\n\t%s", EVetorCondensado.vetor[contador].chave);
		fprintf(arquivo, " \t\t%d", EVetorCondensado.vetor[contador].peso);
		fprintf(arquivo, " %d", EVetorCondensado.vetor[contador].custo);
	}
}
int main(int argc, char **argv) {

	int tipoAlgoritmo;
	int parametro;
	int contador;

	EstruturaArquivo EArquivo;
	char* arquivoEntrada;
	char* arquivoSaida;
	char** vetorTermos;

	FILE *arquivo;

	while ((parametro = getopt(argc, argv, "t:i:s:")) != -1) {
		switch (parametro) {
		case 't':
			tipoAlgoritmo = atoi(optarg);
			break;
		case 'i':
			arquivoEntrada = optarg;
			break;
		case 's':
			arquivoSaida = optarg;
			break;
		default:
			arquivo = fopen(arquivoSaida, "w");
			if (!arquivo) {
				printf("Erro na abertura do arquivo. Fim de programa.");
				exit(1);
			}

			printf("\n\n\tOs parâmetros aceitos são:");
			printf("\n\t\t-t: O que deseja fazer?"
				"\n\t\t\t1: Gerar de arvore de pesquisa balanceada"
				"\n\t\t\t2: Gerar de arvore ótima com algoritmo de tentativa-e-erro"
				"\n\t\t\t3: Gerar de arvore proxima da otima com heuristica"
				"\n\t\t-i: Nome do arquivo de entrada"
				"\n\t\t-s: Nome do arquivo de saida");

			fprintf(arquivo, "\n\n\tOs parâmetros aceitos são:");
			fprintf(arquivo, "\n\t\t-t: O que deseja fazer?"
				"\n\t\t\t1: Gerar de arvore de pesquisa balanceada"
				"\n\t\t\t2: Gerar de arvore ótima com algoritmo de tentativa-e-erro"
				"\n\t\t\t3: Gerar de arvore proxima da otima com heuristica"
				"\n\t\t-i: Nome do arquivo de entrada"
				"\n\t\t-s: Nome do arquivo de saida");

			return 1;
		}
	}

	EArquivo = dadosArquivo(arquivoEntrada);

	vetorTermos = (char**) malloc(EArquivo.quantidadeLinhas * sizeof(char*));

	for (contador = 0; contador < EArquivo.quantidadeLinhas; contador++) {
		vetorTermos[contador] = (char*) calloc(EArquivo.maiorLinha+1, sizeof(char));
	}

	vetorTermos = leArquivo(arquivoEntrada, vetorTermos);

	vetorTermos = ordena(EArquivo.quantidadeLinhas, EArquivo.maiorLinha,
			vetorTermos);

	EstruturaVetorCondensado EVetorCondensado = condensaVetorOrdenado(
			vetorTermos, EArquivo);

	if (tipoAlgoritmo == ALG_ARVORE_BALANC) {
		criaArvoreBalanceada(EVetorCondensado, 0, EVetorCondensado.tamanho - 1);
	} else if (tipoAlgoritmo == ALG_ARVORE_OTIMA_ALE) {
		ArvoreDePesquisaOtimaForcaBruta(EVetorCondensado);
	} else if (tipoAlgoritmo == HEUR_ARVORE_QUASE_OTIMA) {
		heuristicaArvoreDePesquisaOtima(EVetorCondensado);
	} else {
		arquivo = fopen(arquivoSaida, "w");
		if (!arquivo) {
			printf("Erro na abertura do arquivo. Fim de programa.");
			exit(1);
		}
		printf("\n\n\tTipo de algoritmo inválido\n\n");
		fprintf(arquivo, "\n\n\tTipo de algoritmo inválido\n\n");
	}

	pesoArvore = calculaCustoArvore(EVetorCondensado);
	calculaProfundidade(raiz, 1);

	imprimeResultado(EVetorCondensado, arquivoSaida);

	return 0;
}
