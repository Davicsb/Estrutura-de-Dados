#include "CodeCompac/compactacao.h"

int main() {
    
    introducao();

    int opcao;
    scanf("%d", &opcao);
    limpartela();

    if(opcao == 1){
        
        char *nomeDoArquivo = pegarNomeDoArquivo();

        processoParaCompactar(nomeDoArquivo);

        printf("Arquivo compactado com sucesso!");

    } else if(opcao == 2){

        printf("Em construção\n");

    } else {
        limpartela();
        exit(-1);
    }

    return 0;
}