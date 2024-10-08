#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb"
#define ESCREVER_BINARIO "wb"

typedef struct node no;

struct node {
    void* item;
    int freq;
    no *next;
};

no* criarLista(){
    return NULL;
}

int taVazio(no *head){
    return (head == NULL);
}

//retorna 1 se os dois itens (unsigned char) forem iguais
int unsignedCharEquals(void *item1, void *item2){
    return (*(unsigned char*) item1 == *(unsigned char*) item2);
}

//swapL
//entrada: dois nós de uma lista
//
//função: troca as informações dos dois nós, como se tivesse trocado os dois de lugar
void swapL(no *a, no *b){
    void *lixo = a->item;
    int lixo2 = a->freq;

    a->item = b->item;
    a->freq = b->freq;

    b->item = lixo;
    b->freq = lixo2;
}

//add
//entrada: cabeça da lista encadeada e item a ser adicionado
//saída: endereço do nó que sera a nova cabeça da lista
//
//função: se a lista NÃO estiver vazia, ela vai checar todos os elementos para ver se o item ja foi adicionado antes
//se sim, ela aumenta a frequencia dele e retorna o cabeça dado no parametro
//se não ela cria um novo nó com o item e frequencia 1 e ele se tornara o novo cabeça
no* add(no *head, void *item){

    if(!taVazio(head)){
        no *lixo = head;

        while(lixo){
            if(unsignedCharEquals(lixo->item, item)){
                lixo->freq += 1;
                
                return head;
            }

            lixo = lixo->next;
        }
    }

    no *novoNo = (no*) malloc(sizeof(no));
    novoNo->item = item;
    novoNo->freq = 1;
    novoNo->next = head;

    return novoNo;

}

//implementação do bubble sort para listas encadeadas
//entrada: cabeça da função
//
//função: a variavel aux será auxilar para que o primeiro loop itere por todos os elementos da lista
//o resto será o funcionamento do bubblesort normal, itera todos os elementos da lista e se a frequencia de um for maior q a do proximo, swap
//talvez seja melhor usar outro algoritimo...
void bubbleSort(no *head){
    no *aux = head;
    while(aux){

        no *atual = head;

        while(atual->next != NULL){
            
            if(atual->freq > atual->next->freq){
                swapL(atual, atual->next);
            }

            atual = atual->next;
        }

        aux = aux->next;
    }
}

no* ultimo(no *head){
    no *lixo = head;
    while(lixo->next != NULL){
        lixo = lixo->next;
    }
    return lixo;
}

int main(){
    printf("Qual o nome do arquivo? (inclua sua extensão)\n");

    char nomeArquivo[1024];
    scanf(" %[^\n]", nomeArquivo);

    FILE* arquivo = fopen(nomeArquivo, LER_BINARIO);
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        exit(-1);
    }

    fseek(arquivo, 0, SEEK_END);
    unsigned long tamanhoArquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    unsigned char* dados = (unsigned char*) malloc(tamanhoArquivo);
    if(dados == NULL){
        printf("Falha ao alocar memória.");
        exit(-1);
    }

    fread(dados, 1, tamanhoArquivo, arquivo);

    no *listaFreq = criarLista();
    for (unsigned long i = 0; i < tamanhoArquivo; i++) {
        listaFreq =  add(listaFreq, &dados[i]);
    }

    bubbleSort(listaFreq);
    no *ultimoDaLista = ultimo(listaFreq);

    printf("O byte que menos se repete é 0x%02x (%c) com %d ocorrências\n", *(unsigned char*)listaFreq->item, *(unsigned char*)listaFreq->item, listaFreq->freq);
    printf("O byte que mais se repete é 0x%02x (%c) com %d ocorrências\n", *(unsigned char*)ultimoDaLista->item, *(unsigned char*)ultimoDaLista->item, ultimoDaLista->freq);
    //em %02x o 02 é o numero minimo de caracteres que vai ser printado e o x é o especificador para números hexa

    free(dados);
    fclose(arquivo);
}
