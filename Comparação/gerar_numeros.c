#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// GERA O TXT PARA ADIONAR 501 NUMEROS NAS FILAS

int main(){
    FILE *arquivo = fopen("entrada_fila.txt", "w"); // Abre o aqrquivo 'entrada_fila.txt' no modo 'write' - escrever

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) 
    {
        printf("Erro ao abrir o arquivo!\n"); // Identifica que houve um erro ao abrir o arquivo
        return 1; // Retorna erro se o arquivo não puder ser aberto
    }

    int min = 0; // Coloca que o número minimo do sorteio é 0
    int max = 10000; // Coloca que o número máximo do sorteio é 10.000

    srand(time(NULL)); // Sementeia o gerador de núemors para ser diferente a cada execução
    
    for (int i = 0; i <= 500; i++)
    {
        int num_aleatorio_inserido = rand() % (max - min + 1) + min; // Sorteia um número entre 0 e o 10.000 para inserir na fila

        fprintf(arquivo, "%d\n", num_aleatorio_inserido); // Printa o para inserir na fila
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Dados gravados em entrada_fila.txt com sucesso!\n"); // Parâmetro para sabermos se o código rodou com sucesso
    
    return 0; // Programa finalizado com sucesso
}