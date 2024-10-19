#ifndef DESCOMPACTACAO_H
#define DESCOMPACTACAO_H

#include "compactacao.h"

typedef short int sInt;

void *criarPonteiro(unC byte);

no *criarNo(void *byte);

int readTrash(FILE *arquivo);

LLi readSizeTree(FILE *arquivo);

no* readTree(FILE *arquivo, LLi *treeSize);

void writeFile(FILE *arquivoIn, FILE *arquivoOut, short int trash, LLi sizeFile, no *tree);

void processoParaDescompactar(char *nomeDoArquivo);

#endif