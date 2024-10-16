#include <stidio.h>

typedef unsigned char unC;

int readTrash(FILE *arquivo){
    //Ler primeiro byte 
    unC byte;
    fread(&byte,sizeof(unC),1,arquivo);
    //Nesse byte vai os 3 primeiros são o lixo e os 5 sao parte do tamanho da arvore.

    //Como nao queremos ler o tamanho da arvore neste instante empurramos o lixo para o começo do byte. Usando o >> 5.
    unC trash = byte >> 5;
    // T = trash (lixo), S = Size (tamanho). Sendo nosso byte composto por TTTSSSSS.
    //Assim ficando 00000TTT.

    //voltamos o ponteiro para o inicio do arquivo.
    fseek(arquivo, 0, SEEK_SET);

    //Retornamos o valor de Trash (00000TTT), que voltara como um inteiro, representando o lixo do ultimo byte do arquivo.
    return trash;
}
