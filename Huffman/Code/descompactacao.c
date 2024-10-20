#include "descompactacao.h"

no *criarNo(void *byte){
    no* newNode = (no*)malloc(sizeof(no));
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->freq = NULL;
    newNode->item = byte;
    return newNode;
}

LLi readTrash(unC *dados, LLi *index){
    //Ler primeiro byte 
    unC byte = dados[*index];

    //fread(&byte,sizeof(unC),1,arquivo);
    //Nesse byte vai os 3 primeiros são o lixo e os 5 sao parte do tamanho da arvore.

    //Como nao queremos ler o tamanho da arvore neste instante empurramos o lixo para o começo do byte. Usando o >> 5.
    unC trash = byte >> 5;
    // T = trash (lixo), S = Size (tamanho). Sendo nosso byte composto por TTTSSSSS.
    //Assim ficando 00000TTT.

    //voltamos o ponteiro para o inicio do arquivo.
    //fseek(arquivo, 0, SEEK_SET);

    //Retornamos o valor de Trash (00000TTT), que voltara como um inteiro, representando o lixo do ultimo byte do arquivo.
    return trash;
}

LLi readSizeTree(unC *dados, LLi *index){
    //Ler os dois primeiros bytes. Byte1 = TTTSSSSS e Byte2 = SSSSSSSS
    unC byte1 = dados[*index];
    (*index)++;

    unC byte2 = dados[*index];
    (*index)++;

    //Fazemos operacoes com o byte1 para tirar o lixo que esta nele.
    byte1 = byte1 << 3; //=> SSSSS000.
    byte1 = byte1 >> 3; //=> 000SSSSS.
    
    //Criamos uma variavel de short int.
    sInt SizeTree = byte1;
    //('Porque short int?', o int guarda 32 bits, unsigned char guarda 8, short int guarda 16 bits).
    //Como o tamanho é composto por 2 bytes, optamos por usar short int.
    //Poderiamos sim usar int, mas apenas 16 bits seriam utilizados, onde os outros 16 apenas ocupariam espaço.
    //unsigned char é inviavel neste caso.

    // Size tree comecaria como 00000000 000SSSSS. 
    SizeTree = SizeTree << 8; //=> 000SSSSS 00000000.
    SizeTree = SizeTree | byte2; //=> 000SSSSS SSSSSSSS.

    return SizeTree;
}

void *criarPonteiro(unC byte) {
    unC *pointer = malloc(sizeof(unC));
    *pointer = byte;
    return (void*) pointer;
}

no* readTree(unC *dados, LLi *index, LLi *treeSize){
    //Criamos um byte auxiliar e a nossa arvore.
    unC byte;
    no* tree = NULL;

    //Criamos uma condicao de parada para a recursao.
    if(*treeSize > 0){
        //Lemos o primeiro byte da arvore e subtraimos um de seu tamanho.
        //fread(&byte,sizeof(unC),1,arquivo);
        byte = dados[*index];
        (*index)++;

        *treeSize -= 1;
        //Se esse byte for '//' o proximo devera ser uma folha.
        if(byte == '\\'){
            //lemos o proximo byte, o que queremos adicionar na arvore e subtraimos um de seu tamanho.
            //fread(&byte,sizeof(unC),1,arquivo);
            byte = dados[*index];
            (*index)++;

            *treeSize -= 1;

            //Adcionamos esse no a nosa arvore e retornamos essa arvore.
            tree = criarNo(criarPonteiro(byte));
            return tree;
        }

        //Criamos um no nessa arvore.
        tree = criarNo(criarPonteiro(byte));

        //Caso o byte for um '*', sem antes ter um '//', navegamos primeiramente a esquerda da nosa arvore e depois a direita. De forma recursiva.
        if(byte == '*'){
            tree->left = readTree(dados, index, treeSize);
            tree->right = readTree(dados, index, treeSize);
        }

        return tree;
    }

    return tree;
}

void writeFile(unC *dados, LLi *index, FILE *arquivoOut, short int trash, LLi sizeFile, no *tree) {
    //Criamos um byte e uma arvore auxiliar.
    unC byte;
    no *aux = tree;

    //Começamos a ler byte a byte indo ate o anterior ao ultimo.
    while(*index < sizeFile - 1){
        //lemos o byte atual.
        /*fread(&byte, sizeof(unC), 1, arquivoIn);
        fread(&byte, sizeof(unC), 1, arquivoIn);

        fread(&byte, sizeof(unC), 1, arquivoIn);*/
        byte = dados[*index];
        (*index)++;
        
        //começamos a ler bit a bit deste byte.
        for(int j = 7; j >= 0; j--) {
            //Setamos um byte com um unico bit. 
            unC set = 1 << j; //Ex.: j = 6. 01000000.

            //Vemos se ha algum bit na mesma posição. Ex.: Byte = 10101101.
            //01000000
            //&
            //11001101
            //=
            //01000000
            if((set & byte) != 0) {
                //Se = 1.
                aux = aux->right;
            } else {
                //Se = 0.
                aux = aux->left;
            }

            //Quando chegamos a uma folha a escrevemos e resetamos a arvore.
            if(aux->right == NULL && aux->left == NULL) {
                unC *c = (unC*)aux->item; /**/
                char bin[9];
                transformandoCoisaEmBinario(*(int*)c, bin, 9);
                printf("coisa: %s\n");
                fwrite(c, sizeof(unC), 1, arquivoOut);  
                aux = tree;
            }
        }
    }

    //Fazemos a mesma coisa no ultimo byte, apenas ignorando o lixo.
    for(int i = 7; i >= trash; i--){
        unC set = 1 << i;

        if((set & byte) != 0) {
            aux = aux->right;
        } else {
            aux = aux->left;
        }

        if(aux->right == NULL && aux->left == NULL) {
            unC *c = (unC*)aux->item;
            fwrite(c, sizeof(unC), 1, arquivoOut);  
            aux = tree;
        }
    }
}

void imprimirArvore(no *raiz) {
    if (raiz == NULL) {
        return;
    }

    // Imprime o item do nó atual
    if (raiz->item != NULL) {
        printf("%c", *(char*)raiz->item);  // Ajuste conforme o tipo do item
    } else {
        printf("*");  // Nó especial, como o '*' na árvore de Huffman
    }

    // Recursivamente imprime os filhos esquerdo e direito
    imprimirArvore(raiz->left);
    imprimirArvore(raiz->right);
}

int processoParaDescompactar(char *nomeDoArquivo){
    FILE *arquivo = fopen(nomeDoArquivo, LER_BINARIO);
    LLi index = 0;

    if (arquivo == NULL) {
        return 0;
    }

    LLi tamanho = tamanhoDoArquivo(arquivo);
    printf("Tamanho: %lld\n", tamanho);

    unC *dados = lerArquivo(arquivo, tamanho);

    LLi lixo = readTrash(dados, &index);
    //printf("Lixo: %lld, index: %lldd\n", lixo);

    LLi tamanhoArvore = readSizeTree(dados, &index);
    //printf("Arvre: %lld\n", tamanhoArvore);

    no *arvore = readTree(dados, &index, &tamanhoArvore);
    //imprimirArvore(arvore);



    char * nomeDoArquivoNovo = calloc(FILENAME_MAX, sizeof(char));

    int tam = 0;
    while (nomeDoArquivo[tam] != '\0')
    {
        tam++;
    }
    
    
    for (int i = 0; i < strlen(nomeDoArquivo) - 4; i++)
    {
        nomeDoArquivoNovo[i] = nomeDoArquivo[i];
    }


    FILE *arquivosaida = fopen(nomeDoArquivoNovo, ESCREVER_BINARIO);

    writeFile(dados, &index, arquivosaida, lixo, tamanho, arvore);
    return 1;
}
