#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CÓDIGO FILA DE PRIORIDADE SEM HEAPS - Ordenada da maior prioridade para menor
// Parte Sem Heaps
typedef struct node{
    int item;
    struct node *next;
} node;

typedef struct fpSemHeaps{
    node *head;
} fpSemHeaps;

// Cria a fila sem heaps
fpSemHeaps *CriarFilaSemHeaps(){
    fpSemHeaps *NovaFila = (fpSemHeaps*)malloc(sizeof(fpSemHeaps)); // Aloca a memória da fila
    NovaFila->head = NULL; // Aponta para NULL já que não tem nenhum item na fila
    return NovaFila; // Retorna a fila
}

// Verifica se a fila sem heaps está vazia
int IsEmptySemHeaps(fpSemHeaps *Fila){
    if (Fila->head == NULL)
    {
        return 1; // Retorna 1 se a fila está vazia
    }
    else{
        return 0; // Retorna 0 se a fila não estiver vazia
    }
}

// Insere um novo nó de prioridade, da maior para menor, na fila sem heaps
void EnqueueSemHeaps(fpSemHeaps *Fila, int item){
    node *NovoNo = (node*)malloc(sizeof(node)); // Aloca o Nó da fila
    NovoNo->item = item; // Insere o item/prioridade da fila
    if (IsEmptySemHeaps(Fila)|| item > Fila->head->item ) // Se a fila estiver vazia, ou o primeiro item for o maior inserimos no começo da fila
    {
        NovoNo->next = Fila->head; // Aponta para NULL ou o próximo nó
        Fila->head = NovoNo; // A cabeça se torna o primeiro nó já que é o maior
    }
    else
    {
        node *atual = Fila->head; // Cria um nó auxiliar para navegar na fila
        while (atual->next != NULL && atual->next->item > item)
        {
            atual = atual->next; // Enquando o próximo não for o final da fila ou o próximo nó for prioridade maior pula para o próximo nó
        }
        NovoNo->next = atual->next; // O próximo item será o próximo do auxiliar, já que o item do próximo é menor
        atual->next = NovoNo; // O nó maior que o NvoNo vai apontar para ele, que é o próximo maior
    }
}

// Conta quantas comparações são feitas para achar um nó especifico na fila sem heaps
int AcharNoSemHeaps(fpSemHeaps *Fila, int item){
    int comparacoes = 1;
    if (IsEmptySemHeaps(Fila))
    {
        return -1; // Se for o final da fila retorna -1, já que não encontramos o item
    }
    else
    {
        node *atual = Fila->head; // Cria um nó auxiliar para navegar na fila
        while (atual != NULL && atual->item != item) 
        {
            comparacoes += 1; // Quando pulamos o nó fazemos uma comparação
            atual = atual->next; // Enquando o próximo não for o final da fila ou o próximo nó for quem procuramos pula para o próximo nó
        }
        if (atual == NULL)
        {
            return -1; // Se for o final da fila retorna -1, já que não encontramos o item, chegamos no final da lista
        }
        else if (atual->item == item)
        {
            return comparacoes; // Achamos o nó
        }
    }
}


// Imprime uam fila sem heaps
void ImprimirFilaSemHeaps(fpSemHeaps *Fila){
    node *aux = Fila->head; // Cria um nó auxiliar para navegar na fila
    while (aux != NULL)
    {
        printf("%d -> ", aux->item);
        aux = aux->next;
    }
    printf("\n");
} 


// Retorna o tamanho da fila sem heaps que é igual ao tamanho da fila com heaps
int TamanhoFila(fpSemHeaps *Fila){
    int tamanho = 0;
    node *aux = Fila->head; // Cria um nó auxiliar para navegar na fila
    while (aux != NULL)
    {
        tamanho += 1; // Enquanto a fila não acabou aumenta em 1 o tamanho
        aux = aux->next; // Enquanto não é o final da fila passa para o próximo nó
    }
    return tamanho; // Retorna o tamanho da fila
}

int main (){
    fpSemHeaps *FilaPrioridadeSemHeaps = CriarFilaSemHeaps();
    FILE *arquivo = fopen("entrada_fila.txt", "r"); // Ponteiro para o arquivo de entrada
    FILE *arquivosaida = fopen("saida_fila.txt", "w"); // Ponteiro para o arquivo de saída
    char comando[4];
    int numero;
    fprintf(arquivosaida, "CMP TAM\n");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1; // Retorna erro se o arquivo não puder ser aberto
    }

    // Lê os comandos do arquivo até o final
    while (fscanf(arquivo, "%s", comando) != EOF) {
        if (strcmp(comando, "ADD") == 0) {
            fscanf(arquivo, "%d", &numero); // Passa o endereço de 'numero'
            EnqueueSemHeaps(FilaPrioridadeSemHeaps, numero); // Coloca na fila
            // ImprimirFilaSemHeaps(FilaPrioridadeSemHeaps);
        }
        if (strcmp(comando, "FND") == 0) {
            fscanf(arquivo, "%d", &numero); // Passa o endereço de 'numero'
            int comparacoes = AcharNoSemHeaps(FilaPrioridadeSemHeaps, numero); // Guarda quantas comparações foram feitas
            int tamanho = TamanhoFila(FilaPrioridadeSemHeaps);
            // fprintf(arquivosaida, "Numero %d Comp %d\n", numero, comparacoes);
            fprintf(arquivosaida, "%d %d\n", comparacoes, tamanho);
        }
    }

    // Fecha os arquivos
    fclose(arquivo);
    fclose(arquivosaida);
    printf("Dados gravados em saida_fila.txt com sucesso!\n");
    
    // Libere a memória da fila
    while (!IsEmptySemHeaps(FilaPrioridadeSemHeaps)) {
        node *temp = FilaPrioridadeSemHeaps->head;
        FilaPrioridadeSemHeaps->head = FilaPrioridadeSemHeaps->head->next;
        free(temp);
    }
    free(FilaPrioridadeSemHeaps);

    return 0;
}