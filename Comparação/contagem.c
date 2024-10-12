#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// CÓDIGO QUE FAZ A CONTAGEM DE COMPARAÇÕES DA INSERÇÃO NA FILA DE PRIORIDADE COM HEAPS E SEM HEAPS
// AMBAS AS FILAS ORDENAM DA MAIOR PRIORIDADE PARA MENOR

// PARTE SEM HEAP
typedef struct node{
    int item; // Item/prioridade
    struct node *next; // Prómo nó com outra prioridade
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
int EnqueueSemHeaps(fpSemHeaps *Fila, int item){
    int comparacoes = 0; // Inicializa contagem de comparações

    node *NovoNo = (node*)malloc(sizeof(node)); // Aloca o Nó da fila
    NovoNo->item = item; // Insere o item/prioridade da fila

    if (IsEmptySemHeaps(Fila)|| item > Fila->head->item ) // Se a fila estiver vazia, ou o primeiro item for o maior inserimos no começo da fila
    {
        NovoNo->next = Fila->head; // Aponta para NULL ou o próximo nó
        Fila->head = NovoNo; // A cabeça se torna o primeiro nó já que é o maior

        return comparacoes; // Retorna o número de comparações
    }
    else
    {
        node *atual = Fila->head; // Cria um nó auxiliar para navegar na fila
        while (atual->next != NULL && atual->next->item > item) // Navega na fila enquando o próximo não for o final da fila ou o próximo nó for prioridade maior
        {
            comparacoes ++; // Contabiliza a comparação
            atual = atual->next; // Enquando o próximo não for o final da fila ou o próximo nó for prioridade maior pula para o próximo nó
        }
        NovoNo->next = atual->next; // O próximo item será o próximo do auxiliar, já que o item do próximo é menor
        atual->next = NovoNo; // O nó maior que o NvoNo vai apontar para ele, que é o próximo maior

        return comparacoes; // Retorna o número de comparações
    }
}

/*
// Imprime uam fila sem heaps - Usada na conferencia do código
void ImprimirFilaSemHeaps(fpSemHeaps *Fila){
    node *aux = Fila->head; // Cria um nó auxiliar para navegar na fila
    while (aux != NULL)
    {
        printf("%d -> ", aux->item);
        aux = aux->next;
    }
    printf("\n");
}
*/

/*
// Retorna o tamanho da fila sem heaps que é igual ao tamanho da fila com heaps - Usada na conferencia do código
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
*/

// PARTE COM HEAPS
typedef struct fpComHeaps {
    int tamanho; // Parametro para navegar na fila
    int itens[1000]; // Itens/prioridades
} fpComHeaps;

// Inicia a Heap
fpComHeaps *CriarFilaComHeaps() {
    fpComHeaps *NovaHeap = (fpComHeaps*)malloc(sizeof(fpComHeaps));
    NovaHeap->tamanho = 0; // Inicializa o tamanho
    return NovaHeap;
}

// Cálculo do índice do pai 
int IndicePai(fpComHeaps *Fila, int indice) {
    return indice / 2;
}

// Função swap simples
void swap(int *Item1, int *Item2) {
    int temp = *Item1; // Armazena o valor de Item1 em temp
    *Item1 = *Item2;    // Atribui o valor de Item2 a Item1
    *Item2 = temp;      // Atribui o valor armazenado em temp a Item2
}

// Enqueue Com Heaps - Retorna a quantidade de comparações feitas para dar enqueue
int EnqueueComHeaps(fpComHeaps *Fila, int item) {
    Fila->itens[++Fila->tamanho] = item; // Incrementa o tamanho e depois adiciona o item

    int IndiceChave = Fila->tamanho; // Indice do item recém colocaddo
    int IPai = IndicePai(Fila, IndiceChave); // Calcula o Indice do pai, que deve ser maior

    int comparacoes = 0; // Inicializa contagem de comparações

    // Mover o item para cima no heap, se necessário
    while (IPai >= 1 && Fila->itens[IndiceChave] > Fila->itens[IPai]) // Verifica se o Indice do pai está no limite e se o item adicionado é maior que o pai
    { 
        swap(&Fila->itens[IndiceChave], &Fila->itens[IPai]); // Troca o item com o pai, já que é maior o item recém colocado

        IndiceChave = IPai; // O indice do item colocado agora é o indice de onde o pai estava
        IPai = IndicePai(Fila, IndiceChave); // Calcular novo pai

        comparacoes++; // Contabiliza a comparação
    }
    
    return comparacoes; // Retorna o número de comparações
}

/*
// Imprimir Fila Com Heaps - Usada na conferencia do código
void ImprimirFilaComHeaps(heap *Fila) {
    for (int i = 1; i <= Fila->tamanho; i++) { // Mantém o loop a partir de 1 para filas que começam em 1
        printf("%d -> ", Fila->itens[i]);
    }
    printf("\n");
}
*/

int main (){
    fpSemHeaps *FilaPrioridadeSemHeaps = CriarFilaSemHeaps(); // Chama a função de criar a fila sem heaps
    fpComHeaps *FilaPrioridadeComHeaps = CriarFilaComHeaps(); // Chama a função de criar a fila com heaps

    FILE *ArquivoEntrada = fopen("entrada_fila.txt", "r"); // Abre o aqrquivo 'entrada_fila.txt' no modo 'read' - ler
    FILE *ArquivoSaida = fopen("saida_fila.txt", "w"); // Abre o aqrquivo 'saida.txt' no modo 'write' - escrever

    int numero; // Declara a variável que vai receber os números a serem colocados na fila

    fprintf(ArquivoSaida, "SEM HEAPS / COM HEAPS / TAM\n"); // "Cabeçalho" do arquivo

    if (ArquivoEntrada == NULL) 
    {
        printf("Erro ao abrir o arquivo!\n"); // Identifica que houve um erro ao abrir o arquivo
        return 1; // Retorna erro se o arquivo não puder ser aberto
    }

    while (fscanf(ArquivoEntrada, "%d", &numero) != EOF) // Lê os números do arquivo até o final
    {
        int comparacaoSemHeaps = EnqueueSemHeaps(FilaPrioridadeSemHeaps, numero); // Enfilera e retorna a quantidade de comparações sem heaps

        int comparacaoComHeaps = EnqueueComHeaps(FilaPrioridadeComHeaps, numero); // Enfilera e retorna a quantidade de comparações com heaps
       
        fprintf(ArquivoSaida, "%d %d %d\n", comparacaoSemHeaps, comparacaoComHeaps, FilaPrioridadeComHeaps->tamanho); // Imprime no arquivo as comparações e o tamanho da fila
    }

    // Fecha os arquivos
    fclose(ArquivoEntrada);
    fclose(ArquivoSaida);

    printf("Dados gravados em saida_fila.txt com sucesso!\n"); // Parâmetro para sabermos se o código rodou com sucesso
    
    // Libera os nós da fila de prioridade sem heaps
    while (!IsEmptySemHeaps(FilaPrioridadeSemHeaps)) // Libera os nós até a fila ficar vazia
    {
        node *temp = FilaPrioridadeSemHeaps->head; // Aponta para o primeiro nó
        FilaPrioridadeSemHeaps->head = FilaPrioridadeSemHeaps->head->next; // Avança nos nós enquanto temp está apontando para o anterior
        free(temp); // Libera a memória do nó
    }

    // Libera a memória da própria fila sem heaps
    free(FilaPrioridadeSemHeaps);

    // Libera a memória da própria fila com heaps
    free(FilaPrioridadeComHeaps);

    return 0; // Programa finalizado com sucesso
}