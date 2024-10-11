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
    int NumIns[1000];
    int min = 0; // Coloca que o número minimo do sorteio é 0
    int max = 10000;
    srand(time(NULL)); // Sementeia o gerador de núemors para ser diferente a cada execução
    for (int i = 0; i <= 1000; i++)
    {
        int num_aleatorio_inserido = rand() % (max - min + 1) + min; // Sorteia um número entre 0 e o 10.000 para inserir na fila
        NumIns[i] = num_aleatorio_inserido;
        int num_aleatorio_procurado = rand() % (i - min + 1) + min; // Sorteia uma posição do array para procurar aquele numero
        fprintf(arquivo, "ADD %d\nFND %d\n", num_aleatorio_inserido, NumIns[num_aleatorio_procurado]);
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Dados gravados em entrada_fila.txt com sucesso!\n");
    
    return 0;
}