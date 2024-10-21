#include "descompactacao.h"

/* DEBUG

// Imprimir a arvore de Huffman
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

*/

// Cria um nó para a arvore com o byte passado
no *criarNo(void *byte){
    no* newNode = (no*)malloc(sizeof(no));
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->freq = NULL;
    newNode->item = byte;
    return newNode;
}

// Lê o lixo do arquivo (primeiros 3 bits do cabeçalho)
LLi readTrash(unC *dados, LLi *index){
    //Ler primeiro byte 
    unC byte = dados[*index];

    //Como nao queremos ler o tamanho da arvore neste instante empurramos o lixo para o começo do byte. Usando o >> 5.
    unC trash = byte >> 5;
    // T = trash (lixo), S = Size (tamanho da arvore). Sendo nosso byte composto por TTTSSSSS.
    //Assim ficando 00000TTT.

    //Retornamos o valor de Trash (00000TTT), que voltara como um inteiro, representando o lixo do ultimo byte do arquivo.
    return trash;
}

//Lê o tamanho da arvore (parte do primeiro byte do arquivo e segundo byte)
LLi readSizeTree(unC *dados, LLi *index){
    //Lê os dois primeiros bytes. Byte1 = TTTSSSSS e Byte2 = SSSSSSSS
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

// Transforma o byte de unC para um ponteiro para void
void *criarPonteiro(unC byte) {
    unC *pointer = malloc(sizeof(unC)); // aloca o espaço do ponteiro
    *pointer = byte; 
    return (void*) pointer;
}

no* readTree(unC *dados, LLi *index, LLi *treeSize){
    //Criamos um byte auxiliar e a nossa arvore que começa vazia (apontando para nulo)
    unC byte;
    no* tree = NULL;

    //Criamos uma condicao de parada para a recursao.
    if(*treeSize > 0){
        //Lemos o primeiro byte da arvore e subtraimos um de seu tamanho.
        byte = dados[*index];
        (*index)++;

        *treeSize -= 1;
        //Se esse byte for '\\' o proximo devera ser uma folha.
        if(byte == '\\'){
            //lemos o proximo byte, o que queremos adicionar na arvore e subtraimos um de seu tamanho.
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

// verifica se aquele byte naquela posição é 1, se for retorna 1;
int is_on_bit(unsigned char byte, int position) {
    return (byte & (1 << position)) != 0;
}

void writeFile(unC *dados, LLi *index, FILE *arquivoOut, short int trash, LLi sizeFile, no *tree) {
    // Criamos um byte axiliar e um ponteiro para navegarmos tranquilamente na arvore
    unC byte;
    no *current = tree;

    // Processa todos os bytes do arquivo comprimido, exceto o último que contém lixo
    while (*index < sizeFile) {
        // Pega o primeiro byte
        byte = dados[*index];
        (*index)++;

        // Processa cada bit do byte lido
        for (int j = 7; j >= 0; j--) {
            // Verifica se ainda há lixo a ser ignorado
            if (*index == sizeFile && j < trash) {
                break; // Ignora os bits de lixo
            }

            // Navega na árvore de Huffman
            // Se o bit for 1 navega pra arvore para direita
            if (is_on_bit(byte, j) == 1)
            {
                current = current->right;
            }
            // Se o bit for 0 navega para arvore para esquerda
            else
            {
                current = current->left;
            }

            // Ao encontrar uma folha, escreve o byte correspondente
            if (current->left == NULL && current->right == NULL) {
                fwrite(current->item, sizeof(unC), 1, arquivoOut);
                current = tree; // Reseta para a raiz da árvore
            }
        }
    }
    
}

// Função que chama todos os processos para a descompactação
int processoParaDescompactar(char *nomeDoArquivo){
    // Abre o arquivo de entrada para descompactar
    FILE *arquivo = fopen(nomeDoArquivo, LER_BINARIO);

    // inicia o index como 0 para lermos o arquivo do começo
    LLi index = 0;

    // Verifica se o arquivo foi lido corretamente, se não retorna 0
    if (arquivo == NULL) {
        return 0;
    }

    // Calcula o tamanho do arquivo
    LLi tamanho = tamanhoDoArquivo(arquivo);

    // Lê os bytes 
    unC *dados = lerArquivo(arquivo, tamanho);

    // Lê quanto do final do arquivo é lixo
    LLi lixo = readTrash(dados, &index);

    // Lê o tamanho da arvore de Huffman
    LLi tamanhoArvore = readSizeTree(dados, &index);

    // Lê a arvore de Huffman
    no *arvore = readTree(dados, &index, &tamanhoArvore);

    // Retira o .huff do arquivo compactado
    // Aloca o espaço para o nome novo do arquivo
    char * nomeDoArquivoNovo = calloc(FILENAME_MAX, sizeof(char));

    // Calcula o tamanho do nome do arquivo descompactado
    int tam = 0;
    while (nomeDoArquivo[tam] != '\0')
    {
        tam++;
    }
    
    // Vai até o limite do nome do arquivo antes do .huff
    for (int i = 0; i < strlen(nomeDoArquivo) - 4; i++)
    {
        nomeDoArquivoNovo[i] = nomeDoArquivo[i];
    }

    // Abre o arquivo no modo de escrever binario
    FILE *arquivosaida = fopen(nomeDoArquivoNovo, ESCREVER_BINARIO);

    // Imprime no arquivo, o arquivo descompactado
    writeFile(dados, &index, arquivosaida, lixo, tamanho, arvore);

    destruirArvore(arvore);
    fclose(arquivosaida);
    fclose(arquivo);
    return 1;
}