#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CÓDIGO FILA DE PRIORIDADE SEM HEAPS - Ordenada da maior prioridade para menor

typedef struct node{
    int item;
    struct node *next;
} node;

typedef struct fp{
    node *head;
} fp;

// Cria a fila
fp *CriarFila(){
    fp *NovaFila = (fp*)malloc(sizeof(fp)); // Aloca a memória da fila
    NovaFila->head = NULL; // Aponta para NULL já que não tem nenhum item na fila
    return NovaFila; // Retorna a fila
}

// Verifica se a fila está vazia
int IsEmpty(fp *Fila){
    if (Fila->head == NULL)
    {
        return 1; // Retorna 1 se a fila está vazia
    }
    else{
        return 0; // Retorna 0 se a fila não estiver vazia
    }
}

// Insere uma nova prioridade, da maior para menor
void Enqueue(fp *Fila, int item){
    node *NovoNo = (node*)malloc(sizeof(node)); // Aloca o Nó da fila
    NovoNo->item = item; // Insere o item/prioridade da fila
    if (IsEmpty(Fila)|| item > Fila->head->item ) // Se a fila estiver vazia, ou o primeiro item for o maior inserimos no começo da fila
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

// Conta quantas comparações são feitas para achar um nó especifico
int AcharNo(fp *Fila, int item){
    int comparacoes = 1;
    if (IsEmpty(Fila))
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

void ImprimirFila(fp *Fila){
    node *aux = Fila->head;
    while (aux != NULL)
    {
        printf("%d -> ", aux->item);
        aux = aux->next;
    }
    printf("\n");
}

int TamanhoFila(fp *Fila){
    int tamanho = 0;
    node *aux = Fila->head;
    while (aux != NULL)
    {
        tamanho += 1;
        aux = aux->next;
    }
    return tamanho;
}

int main (){
    fp *FilaPrioridade = CriarFila();
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
            Enqueue(FilaPrioridade, numero); // Coloca na fila
            // ImprimirFila(FilaPrioridade);
        }
        if (strcmp(comando, "FND") == 0) {
            fscanf(arquivo, "%d", &numero); // Passa o endereço de 'numero'
            int comparacoes = AcharNo(FilaPrioridade, numero); // Guarda quantas comparações foram feitas
            int tamanho = TamanhoFila(FilaPrioridade);
            fprintf(arquivosaida, "%d %d\n", comparacoes, tamanho);
        }
    }

    // Fecha os arquivos
    fclose(arquivo);
    fclose(arquivosaida);
    printf("Dados gravados em saida_fila.txt com sucesso!\n");
    
    // Libere a memória da fila
    while (!IsEmpty(FilaPrioridade)) {
        node *temp = FilaPrioridade->head;
        FilaPrioridade->head = FilaPrioridade->head->next;
        free(temp);
    }
    free(FilaPrioridade);

    return 0;
}