#include "CodeCompac/compactacao.h"
#include "CodeCompac/descompactacao.h"

int main() {
    limpartela();
    introducao();

    int opcao;
    scanf("%d", &opcao);
    limpartela();

    if(opcao == 1){
        
        char *nomeDoArquivo = pegarNomeDoArquivo();

        processoParaCompactar(nomeDoArquivo);

        printf("Arquivo compactado com sucesso!");

    } else if(opcao == 2){
        char *nomeDoArquivo = pegarNomeDoArquivo();

        processoParaDescompactar(nomeDoArquivo);

        printf("Arquivo descompactado com sucesso!");

    } else {
        limpartela();
        exit(-1);
    }

    return 0;
}