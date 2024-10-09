#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb"
#define ESCREVER_BINARIO "wb"
#define ITEM_PARA_O_NO_PAI NULL
//nas especificações o professor diz q o nó que é pai tem que ser salvo como * no arquivo, vou perguntar pra ele se dentro do programa eu posso usar NULL

typedef struct node no;

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

int unsignedCharEquals(void *item1, void *item2){
    return (*(unsigned char*) item1 == *(unsigned char*) item2);
}

//swapL alterado pra dar swap nos filhos também.
void swapL(no *a, no *b){
    void *lixo = a->item;
    int lixo2 = a->freq;
    no *filhos[2] = {a->left, a->right};

    a->item = b->item;
    a->freq = b->freq;
    a->left = b->left;
    a->right = b->right;

    b->item = lixo;
    b->freq = lixo2;
    b->left = filhos[0];
    b->right = filhos[1];
}

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
    //checagem de erro
    if(novoNo == NULL){
        printf("Falha ao alocar memória na função add\n");
        exit(-1);
    }
    novoNo->item = item;
    novoNo->freq = 1;
    novoNo->left = NULL;
    novoNo->right = NULL;
    novoNo->next = head;

    return novoNo;

}

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

//arvore
//entrada: cabeça de uma lista encadeada
//saída: a raíz de uma arvore de huffman
//função: 
//1: checa se a lista existe, e se há mais de um elemento nela (se falhar ela retorna a cabeça), se houver mais de um elemento ela já ordena a lista também.
//2: se houver mais de um elemento, ela vai criar um novo nó que tera os dois primeiros nós (que são os que tem a menor frequencia) como filhos (juntando suas frequencias) e os subistituira na lista.
//3: como esse novo nó subistituiu a cabeça, a função ocorrerá recursivamente até só ter um nó (que será a raíz da arvore)
no* arvore(no* head){
    if(head){

        if(head->next != NULL){
            bubbleSort(head);

            no *novoNo = (no*) malloc(sizeof(no));
            //checagem de erro
            if(novoNo == NULL){
                printf("Falha ao alocar memória na função arvore\n");
                exit(-1);
            }

            novoNo->item = ITEM_PARA_O_NO_PAI;
            novoNo->freq = head->freq + head->next->freq;
            novoNo->next = head->next->next;

            novoNo->left = head;
            novoNo->right = head->next;

            head = novoNo;

            return arvore(head);
        
        } else {
            return head;
        }

    }

}
//printPreOrder
//entrada: raíz de uma arvore de huffman
//função: imprime em pre ordem a arvore, se o nó não for uma FOLHA (nó que não tem filho) o char será impresso, caso contrário * será impresso.
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

int main(){
    printf("Qual o nome do arquivo? (inclua sua extensão)\n");

    char nomeArquivo[FILENAME_MAX];
    scanf(" %[^\n]", nomeArquivo);

    FILE* arquivo = fopen(nomeArquivo, LER_BINARIO);
    //checagem de erro
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        exit(-1);
    }

    fseek(arquivo, 0, SEEK_END);
    long long tamanhoArquivo = ftell(arquivo);
    //checagem de erro
    if(tamanhoArquivo == 0 || tamanhoArquivo == -1){
        printf("Falha ao pegar o tamanho do arquivo ou arquivo vazio.\n");
        exit(-1);
    }
    fseek(arquivo, 0, SEEK_SET);

    unsigned char* dados = (unsigned char*) malloc(tamanhoArquivo);
    //checagem de erro
    if(dados == NULL){
        printf("Falha ao alocar memória.");
        exit(-1);
    }

    fread(dados, 1, tamanhoArquivo, arquivo);

    no *listaFreq = criarLista();
    for (unsigned long i = 0; i < tamanhoArquivo; i++) {
        listaFreq =  add(listaFreq, &dados[i]);
    }

    listaFreq = arvore(listaFreq);
    printPreOrder(listaFreq);
    printf("\n");

    free(dados);
    fclose(arquivo);
}
