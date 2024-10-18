#ifndef COMPACTACAO_H
#define COMPACTACAO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb" //"Reading Binary"
#define ESCREVER_BINARIO "ab"
#define TAM 256

typedef struct node no;
typedef long long int LLi;
typedef unsigned char unC;

struct node {
    void* item; //implementação com void*
    void* freq; //frequencia
    no *next, *left, *right;
};

//---utilidades---

void introducao();

//retorna NULL como ponteiro para o no
no* criarListaVazia();

//retorna 1 se os dois itens (unsigned char) forem iguais
int unsignedCharEquals(void *item1, void *item2);

//retorna o maior dos dois itens
int max(int a, int b);

//retorna a altura da arvore (o maior caminho da raíz para uma folha) que será o tamanho máximo de um byte do novo alfabeto
int alturaArvore(no *raiz);

//retorna quantos nós uma arvore tem
int qntsNos(no *raiz);

//faz a pergunta e escaneia o nome do arquivo dado, retornando a string
char* pegarNomeDoArquivo();

//retorna o tamanho do arquivo
LLi tamanhoDoArquivo(FILE *arquivo);

//le os bytes do arquivo e retorna o array dinamico com eles
unC* lerArquivo(FILE *arquivo, LLi tamanhoArquivo);

//da free em todos os nós e seus respectivos itens e freqs
void destruirArvore(no *raiz);

//da free em cada string do dicionario e nele mesmo
void destruirDicionario(char **dicionario);

//---

//---debug---

//printa todos os elementos da lista, indicando seu byte em hexadecimal, seu char (se for printavel) e sua frequencia
void printar(no *head);

//printa todos os nós de uma arvore em pre ordem
void printPreOrder(no *head);

//printa os novos bytes do dicionario formado
void imprimirDicionario(char **dicionario);

//printa os bytes do arquivo compactado pra um arquivo txt
void imprimirDadosNovos(char *dadosNovos);

//---

//preenche um array de tamanho 256 com a frequencia de cada byte no arquivo
void calcFreq(unC *dados, LLi tamanhoArquivo, LLi *freq);

//adiciona o nó em uma lista de ordem crescente
no* add(no *head, no *new);

//vai preencher a lista com a tabela de frequencia
no* inserir(LLi freq[TAM]);

//---codificar---

//recebe uma lista e a partir de chamadas recursivas ele constroi a arvore de Huffman
//o processo será juntar os primeiros dois elementos como filhos de um nó e somar suas frequencias, adicionando esse nó de maneira ordenada na lista novamente
//a recursão acaba quando houver apenas um elemento na lista, que agora será a raíz da arvore
no* arvore(no* head);

//funcionando como o printPreOrdem, apenas colocando cada elemento em uma "string"
void auxArvorePreOrdem(no *head, unC *arvorePre, int *i);

//a função cria um array arvorePre, o preenche com a função auxiliar com os elementos da arvore e o retorna
unC* pegarArvorePreOrdem(no *head);

//vai preencher o dicionario vazio
//"caminho" será uma string vazia "" no inicio
void criarDicionarioCompleto(char **dicionario, no *head, char *caminho, int colunas);

//usando o dicionario, cria-se uma string de dados novos onde será salvo a sequencia de 0s e 1s de cada byte codificado de dados
char* codificar(char **dicionario, unC *dados, LLi tamanhoArquivo);

//---

//--compactar--

//transformar um numero coisa em um número binário em uma string bin
void transformandoCoisaEmBinario(int coisa, char *bin, int tamanhoDeBin);

//escreve uma string de 0s e 1s como bytes em um arquivo
void escreverCoisaEmBinario(FILE* arquivo, char *dados);

//faz os procedimentos para escrever o cabeçario e os dados novos em um arquivo novo
void compactar(char *dadosNovos, unC *arvorePre, int tamanhoArvorePre, char *nomeOriginal);

//---

//---funções principais---

//função que chama todos os processos para a compactação
void processoParaCompactar(char *nomeDoArquivo);

//--

#endif
