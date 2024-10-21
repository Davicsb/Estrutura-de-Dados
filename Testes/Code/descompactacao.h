#ifndef DESCOMPACTACAO_H
#define DESCOMPACTACAO_H

#include "compactacao.h"

typedef short int sInt;

void *criarPonteiro(unC byte);

no *criarNo(void *byte);

LLi readTrash(unC *dados, LLi *index);

LLi readSizeTree(unC *dados, LLi *index);

no* readTree(unC *dados, LLi *index, LLi *treeSize);

int is_on_bit(unsigned char byte, int position);

void writeFile(unC *dados, LLi *index, FILE *arquivoOut, short int trash, LLi sizeFile, no *tree);

int processoParaDescompactar(char *nomeDoArquivo);

#endif