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

        limpartela();

            if (processoParaCompactar(nomeDoArquivo) == 1)
            {
                printf("> Arquivo compactado com sucesso! <\n");
            }
            else
            {
                printf("> Falha ao abrir \"%s\". Tente novamente! <\n", nomeDoArquivo);
            }
        
        }

        else if(opcao == 2)
        {

        char *nomeDoArquivo = pegarNomeDoArquivo();

        limpartela();

            if (processoParaDescompactar(nomeDoArquivo) == 1)
            {
                
                printf("> Arquivo descompactado com sucesso! <\n");
            }
            else
            {
                printf("> Falha ao abrir \"%s\". Tente novamente! <\n", nomeDoArquivo);
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