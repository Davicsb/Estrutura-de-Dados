#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// GERA O TXT PARA ADIONAR 1000 NUEMROS NAS FILAS E SORTEIA NÚMEORS PARA PROCURA

int main(){
    FILE *arquivo = fopen("entrada_fila.txt", "w");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    int min = 0; // Coloca que o número mpinimo do sorteio é 0
    srand(time(NULL)); // Sementeia o gerador de núemors para ser diferente a cada execução
    for (int i = 0; i <= 1000; i++)
    {
        int num_aleatorio = rand() % (i - min + 1) + min; // Sorteia um número entre 0 e o tamanho da fila
        if (i == 1000)
        {
            fprintf(arquivo, "ADD %d\nFND %d", i, num_aleatorio); // Tira o \n da ultima linha
        }
        else
        {
        fprintf(arquivo, "ADD %d\nFND %d\n", i, num_aleatorio); // Manda adiocionar números de 0 até 1000 e depois manda procurar um número no intervalo
        }
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Dados gravados em entrada_fila.txt com sucesso!\n");
    
    return 0;
}