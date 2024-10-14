#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//---util---
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
//---

//retorna NULL como ponteiro pro nó
no* criarLista(){
    return NULL;
}

//retorn 1 se o cabeça do nó for NULL
int taVazio(no *head){
    return (head == NULL);
}

//retorna 1 se os dois itens (unsigned char) forem iguais
int unsignedCharEquals(void *item1, void *item2){
    return (*(unsigned char*) item1 == *(unsigned char*) item2);
}

//retorna o maior dos dois itens
int max(int a, int b){
    if(a > b){
        return a;
    } else {
        return b;
    }
}

//retorna a altura da arvore (o maior caminho da raíz para uma folha) que será o tamanho máximo de um char no novo alfabeto
int tamanhoArvore(no *raiz){
    if(raiz == NULL){
        return -1;
    } else {
        int left = tamanhoArvore(raiz->left);
        int right = tamanhoArvore(raiz->right);

        return max(left, right) + 1;
    }
}

int qntsNos(no *bt){

    if(bt == NULL){
        return 0;
    } else {
        return qntsNos(bt->left) + qntsNos(bt->right) + 1;
    }

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

void printPreOrder(no *head, unC *arvorePre, int *i){
    if(head != NULL){

        if(head->item){
            printf("%c", *(unsigned char*)head->item);
            arvorePre[*i] = *(unsigned char*)head->item;
            //printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unsigned char*)head->item, *(unsigned char*)head->item, head->freq);
        } else {
            printf("*");
            arvorePre[*i] = '*';
        }

        (*i)++;

        printPreOrder(head->left, arvorePre, i);
        printPreOrder(head->right, arvorePre, i);
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

//funções dicionario

char** criarDicionarioVazio(int colunas){
    char **dicionario = malloc(sizeof(char*) * 256);

    for(int i = 0; i < 256; i++){
        dicionario[i] = calloc(colunas, sizeof(char));
    }
    
    return dicionario;
}

void criarDicionarioCompleto(char **dicionario, no *head, char *caminho, int colunas){
    char left[colunas + 1], right[colunas + 1];

    if(head->left == NULL && head->right == NULL){
        strcpy(dicionario[*(unsigned char*)head->item], caminho);

    } else {
        strcpy(left, caminho);
        strcpy(right, caminho);

        strcat(left, "0");
        strcat(right, "1");

        criarDicionarioCompleto(dicionario, head->left, left, colunas);
        criarDicionarioCompleto(dicionario, head->right, right, colunas);
    }

}

void imprimirDicionario(char **dicionario){
    for(int i = 0; i < 256; i++){
        if(dicionario[i][0] != '\0'){
            printf("%3d %c %s\n", i, i, dicionario[i]);
        }
    }
}

//

//codificar

char* codificar(char **dicionario, unC *dados, LLi tamanhoArquivo){
    //pegar o tamanho total do arquivo novo
    LLi tamanho = 0;
    for(LLi i = 0; i < tamanhoArquivo; i++){
        tamanho += strlen(dicionario[dados[i]]);
    }
    tamanho += 1;
    //
    
    char *dadosNovos = calloc(tamanho, sizeof(char));
    for(LLi i = 0; i < tamanhoArquivo; i++){
        strcat(dadosNovos, dicionario[dados[i]]);
    }

    return dadosNovos;
}

//

//compactar
//ideia
//começar com char cabecario[16 + bytes do printPreOrder da arvore]

void transformandoCoisaEmBinario(int coisa, char *bin, int tamanhoDeBin){
    for(int i = 0; i < tamanhoDeBin - 1; i++){
        bin[i] = '0';
    }
    bin[tamanhoDeBin - 1] = '\0';

    int i = tamanhoDeBin - 2;
    while(coisa > 0 && i >= 0){
        bin[i] = (coisa % 2) + '0';
        coisa /= 2;
        i--;
    }
}

void escreverCoisaEmBinario(FILE* arquivo, void *dados, char *tipo){

    int i = 0, j = 7;
    unsigned char byte = 0; // 00000000
    
    if(strcmp(tipo, "char") == 0){
        while (((char*)dados)[i] != '\0') {

            unsigned char mascara = 1; // 00000001

            if (((char*)dados)[i] == '1') {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0) {
                fwrite(&byte, sizeof(unsigned char), 1, arquivo);
                byte = 0;
                j = 7;
            }
            i++;

        }

        if (j != 7) {
            fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        }

    } else if(strcmp(tipo, "unsigned char") == 0){
        while (((unsigned char*)dados)[i] != '\0') {

            unsigned char mascara = 1; // 00000001

            if (((unsigned char*)dados)[i] == '1') {
                mascara = mascara << j;
                byte = byte | mascara;
            }
            j--;

            if (j < 0) {
                fwrite(&byte, sizeof(unsigned char), 1, arquivo);
                byte = 0;
                j = 7;
            }
            i++;

        }

        if (j != 7) {
            fwrite(&byte, sizeof(unsigned char), 1, arquivo);
        }

    }

}

// Compactar arquivo
void compactar(char *dadosNovos, unC *arvorePre, int tamanhoArvorePre) {
    char cabecario[17] = {0};  // Inicializando o cabeçário
    char lixo[4] = {0};
    char tamanhoArrvore[14] = {0};

    // Passo 1: lixo
    int lixoArquivo = 8 - (strlen(dadosNovos) % 8);
    if (lixoArquivo == 8) {
        lixoArquivo = 0;
    }
    printf("Lixo do Arquivo: %d\n", lixoArquivo);
    transformandoCoisaEmBinario(lixoArquivo, lixo, 4);
    strcat(cabecario, lixo);

    // Passo 2: arvorePre
    transformandoCoisaEmBinario(tamanhoArvorePre, tamanhoArrvore, 14);
    strcat(cabecario, tamanhoArrvore);
    printf("Tamanho cabeçário: %ld\n", strlen(cabecario));

    // Processo de escrever
    FILE* arquivo = fopen("salve.sus", "wb");
    if (!arquivo) {
        printf("Falha ao criar o arquivo.\n");
        return;
    }

    // Escrever cabeçário
    escreverCoisaEmBinario(arquivo, cabecario, "char");

    // Escrever arvorePre
    int i = 0;
    while (arvorePre[i] != '\0') {
        fwrite(&arvorePre[i], sizeof(unsigned char), 1, arquivo);
        i++;
    }

    // Escrever dados novos
    escreverCoisaEmBinario(arquivo, dadosNovos, "unsigned char");

    fclose(arquivo);
}
//

int main(){
    printf("Qual o nome do arquivo? (inclua sua extensão)\n");

    char nomeArquivo[TAM];
    scanf(" %[^\n]", nomeArquivo);

    FILE* arquivo = fopen(nomeArquivo, LER_BINARIO);
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        exit(-1);
    }

    fseek(arquivo, 0, SEEK_END); //vai pro final
    LLi tamanhoArquivo = ftell(arquivo); //pega o tamanho
    fseek(arquivo, 0, SEEK_SET); //volta pro começo

    unC *dados = (unC*) malloc(tamanhoArquivo);
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

    unC *arvorePre = (unC*) malloc(qntsNos(listaFreq) + 1);
    if(arvorePre == NULL){
        printf("Falha ao alocar memória.");
        exit(-1);
    }

    int iterador = 0;

    printPreOrder(listaFreq, arvorePre, &iterador);
    arvorePre[iterador + 1] = '\0';

    printf("\n");

    int colunas = tamanhoArvore(listaFreq) + 1;

    char **dicionario = criarDicionarioVazio(colunas);
    criarDicionarioCompleto(dicionario, listaFreq, "", colunas);
    imprimirDicionario(dicionario);

    char *dadosNovos = codificar(dicionario, dados, tamanhoArquivo);
    printf("%s\n", dadosNovos);

    compactar(dadosNovos, arvorePre, qntsNos(listaFreq));

    free(dados);
    fclose(arquivo);
}
