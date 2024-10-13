#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb"
#define ESCREVER_BINARIO "wb"
#define TAM 256

typedef struct node no;
typedef long long int LLi;
typedef unsigned char unC;

struct node {
    void* item;
    int freq;
    no *next, *left, *right;
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

no* add(no *head, no *new){

    if(taVazio(head)){
        head = new;

    } else if(new->freq <= head->freq){
        new->next = head;
        head = new;

    } else {
        no *lixo = head;
        while(lixo->next && lixo->next->freq < new->freq){
            lixo = lixo->next;
        }
        new->next = lixo->next;
        lixo->next = new;

    }

    return head;

}

void inserir(no **head, LLi freq[TAM]){
    for(int i = 0; i < TAM; i++){
        if(freq[i] > 0){
            no* newNo = (no*) malloc(sizeof(no));
            
            newNo->item = malloc(sizeof(unC));
            *(unC*)newNo->item = (unC)i;
            newNo->freq = freq[i];
            newNo->next = NULL;
            newNo->left = NULL;
            newNo->right = NULL;

            *head = add(*head, newNo);
        }
    }
}

void printar(no *head){
    no *lixo = head;
    while(lixo){
        printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unC*)lixo->item, *(unC*)lixo->item, lixo->freq);
        lixo = lixo->next;
    }
}

void printPreOrder(no *head){
    if(head != NULL){

        if(head->left == NULL && head->right == NULL){
            printf("%c", *(unsigned char*)head->item);
            //printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unsigned char*)head->item, *(unsigned char*)head->item, head->freq);
        } else {
            printf("*");
            printPreOrder(head->left);
            printPreOrder(head->right);
        }

    }
}

no* arvore(no* head){
    if(head){

        if(head->next != NULL){

            no *novoNo = (no*) malloc(sizeof(no));
            //checagem de erro
            if(novoNo == NULL){
                printf("Falha ao alocar memória na função arvore\n");
                exit(-1);
            }

            novoNo->item = NULL;
            novoNo->freq = head->freq + head->next->freq;
            novoNo->next = NULL;

            novoNo->left = head;
            novoNo->right = head->next;

            head = head->next->next;

            head = add(head, novoNo);
            return arvore(head);
        
        } else {
            return head;
        }

    }
}

no* desenfilar(no *head){
    no *lixo = NULL;
    if(head){
        lixo = head;
        head = lixo->next;
        lixo->next = NULL;
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
    LLi tamanhoArquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    unsigned char* dados = (unsigned char*) malloc(tamanhoArquivo);
    if(dados == NULL){
        printf("Falha ao alocar memória.");
        exit(-1);
    }

    fread(dados, 1, tamanhoArquivo, arquivo);
    LLi freq[TAM] = {0};

    no *listaFreq = criarLista();
    for (LLi i = 0; i < tamanhoArquivo; i++) {
        freq[dados[i]] += 1;
    }

    inserir(&listaFreq, freq);

    printar(listaFreq);
    listaFreq = arvore(listaFreq);
    printPreOrder(listaFreq);
    printf("\n");

    free(dados);
    fclose(arquivo);
}
