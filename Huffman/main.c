#include "Code/compactacao.h"
#include "Code/descompactacao.h"

int main() {
    limpartela();
    
    while (1)
    {
        introducao();

        int opcao;
        scanf("%d", &opcao);
        limpartela();

        if(opcao == 1)
        {
        
        char *nomeDoArquivo = pegarNomeDoArquivo();

            if (processoParaCompactar(nomeDoArquivo) == 1)
            {
                limpartela();
                printf("Arquivo compactado com sucesso!\n");
            }
            else
            {
                printf("Falha ao abrir o arquivo. Tente novamente!\n");
            }
        
        }

        else if(opcao == 2)
        {
        char *nomeDoArquivo = pegarNomeDoArquivo();

            if (processoParaDescompactar(nomeDoArquivo) == 1)
            {
                limpartela();
                printf("Arquivo descompactado com sucesso!\n");
            }
            else
            {
                printf("Falha ao abrir o arquivo. Tente novamente!\n");
            }

        }

        else 
        {
            limpartela();
            break;
        }
    }

    return 0;
}