#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb" //"Reading Binary"
#define ESCREVER_BINARIO "wb" //"Writing Binary"
#define TAM 256

typedef struct node no;
typedef long long int LLi;
typedef unsigned char unC;

struct node {
    void* item; //implementação com void*
    int freq;
    no *next, *left, *right;
};

//---utilidades---

void introducao(){
    printf("  _    _        __  __                          _____          _ _             \n");
    printf(" | |  | |      / _|/ _|                        / ____|        | (_)            \n");
    printf(" | |__| |_   _| |_| |_ _ __ ___   __ _ _ __   | |     ___   __| |_ _ __   __ _ \n");
    printf(" |  __  | | | |  _|  _| '_ ` _ \\ / _` | '_ \\  | |    / _ \\ / _` | | '_ \\ / _` |\n");
    printf(" | |  | | |_| | | | | | | | | | | (_| | | | | | |___| (_) | (_| | | | | | (_| |\n");
    printf(" |_|  |_|\\__,_|_| |_| |_| |_| |_|\\__,_|_| |_|  \\_____\\___/ \\__,_|_|_| |_|\\__, |\n");
    printf("                                                                          __/ |\n");
    printf("                                                                         |___/ \n");
    printf("Implementação da codificação de Huffman apresentada pelos integrantes: Davi Celestino, Humberto Barros e João Tenório.\n");
    printf("UNIVERSIDADE FEDERAL DE ALAGOAS\n");
    printf("1 - Compactar arquivo\n2 - Descompactar arquivo\n3 - Sair do programa\n");
}

//retorna NULL como ponteiro para o no
no* criarListaVazia(){
    return NULL;
}

//retorna 1 se os dois itens (unsigned char) forem iguais
int unsignedCharEquals(void *item1, void *item2){
    return (*(unC*) item1 == *(unC*) item2); // como itens void* não podem ser comparados diretamente precisamos fazer o casting com *(tipo*)
}

//retorna o maior dos dois itens
int max(int a, int b){
    if(a > b){
        return a;
    } else {
        return b;
    }
}

//retorna a altura da arvore (o maior caminho da raíz para uma folha) que será o tamanho máximo de um byte do novo alfabeto
int alturaArvore(no *raiz){
    if(raiz == NULL){
        return -1;
    } else {
        int left = alturaArvore(raiz->left);
        int right = alturaArvore(raiz->right);

        return max(left, right) + 1;
    }
}

//retorna quantos nós uma arvore tem
int qntsNos(no *raiz){
    if(raiz == NULL){
        return 0;
    } else {
        return qntsNos(raiz->left) + qntsNos(raiz->right) + 1;
    }

}

//faz a pergunta e escaneia o nome do arquivo dado, retornando a string
char* pegarNomeDoArquivo(){
    printf("Qual o nome do arquivo? (inclua sua extensão)\n");

    char *nomeArquivo = calloc(FILENAME_MAX, sizeof(char));
    scanf(" %[^\n]", nomeArquivo);

    return nomeArquivo;
}

//retorna o tamanho do arquivo
LLi tamanhoDoArquivo(FILE *arquivo){
    
    fseek(arquivo, 0, SEEK_END); //move o ponteiro pro final
    LLi tamanhoArquivo = ftell(arquivo); //pega o tamanho
    fseek(arquivo, 0, SEEK_SET); //volta pro começo

    return tamanhoArquivo;
}

//le os bytes do arquivo e retorna o array dinamico com eles
unC* lerArquivo(FILE *arquivo, LLi tamanhoArquivo){
    unC *dados = (unC*) malloc(tamanhoArquivo); // aloca o espaço com base no tamanho do arquivo
    //checagem de erro
    if(dados == NULL){
        printf("Falha na função lerArquivo ao tentar alocar memória para dados.\n");
        exit(-1);
    }

    fread(dados, 1, tamanhoArquivo, arquivo); // le o arquivo

    return dados;
}

//---

//---debug---

//printa todos os elementos da lista, indicando seu byte em hexadecimal, seu char (se for printavel) e sua frequencia
void printar(no *head){
    no *lixo = head;
    while(lixo){
        printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unC*)lixo->item, *(unC*)lixo->item, lixo->freq);
        lixo = lixo->next;
    }
}

//printa todos os nós de uma arvore em pre ordem
void printPreOrder(no *head){
    if(head != NULL){

        if(head->item){ //se head->item não for NULL ele vai printar seu char
            printf("%c", *(unsigned char*)head->item);
            //printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unsigned char*)head->item, *(unsigned char*)head->item, head->freq);

        } else { //caso contrario ele apenas printa '*'
            printf("*");
        }

        printPreOrder(head->left);
        printPreOrder(head->right);
    }
}

//printa os novos bytes do dicionario formado
void imprimirDicionario(char **dicionario){
    for(int i = 0; i < 256; i++){
        if(dicionario[i][0] != '\0'){
            printf("%3d %c %s\n", i, i, dicionario[i]);
        }
    }
}

//---

//adiciona o nó em uma lista de ordem crescente
no* add(no *head, no *new){

    if(head == NULL){ //se a lista estiver vazia
        head = new; //o novo elemento será a nova cabeça

    } else if(new->freq <= head->freq){ //se a lista NÃO estiver vazia MAS a frequencia do novo elemento for menor ou igual a frequencia do cabeça
        new->next = head; //o novo elemento terá como proximo o cabeça
        head = new; // e o novo elemento será o novo cabeça

    } else { //se a lista não estiver vazia e a frequencia do novo elemento for maior do que a do cabeça
        no *lixo = head;
        //vamos percorrer a lista até chegar ao fim dela (lixo->next == NULL) ou até a frequencia de um nó da lista for menor que a do novo elemento
        while(lixo->next && lixo->next->freq < new->freq){
            lixo = lixo->next;
        }

        new->next = lixo->next; //o novo elemento terá como proximo o proximo do lixo
        lixo->next = new; // e o proximo atual do "lixo" será o novo elemento

    }

    return head; // retorna o cabeça

}

//vai preencher a lista com a tabela de frequencia
no* inserir(LLi freq[TAM]){
    no *head = criarListaVazia();

    for(int i = 0; i < TAM; i++){ //inicia um loop de 256 iterações, para cada char da tabela ASCII, sendo o i cada char de 0 a 255

        if(freq[i] > 0){ // se a frequencia de um char for maior que zero, significa que ele está presente no arquivo

            no* newNo = (no*) malloc(sizeof(no)); //aloca espaço para um novo nó
            //checagem de erro
            if(newNo == NULL){
                printf("Falha na função inserir ao tentar alocar memória para newNo.\n");
                exit(-1);
            }

            newNo->item = malloc(sizeof(unC)); //aloca espaço de um unsigned char para o void*
            //checagem de erro
            if(newNo->item == NULL){
                printf("Falha na função inserir ao tentar alocar memória para newNo->item.");
                exit(-1);
            }

            *(unC*)newNo->item = (unC)i; // o item recebera o i convertido para unsigned char
            newNo->freq = freq[i]; //a frequencia desse nó será a frequecia obtida desse byte

            //e os ponteiros serão NULL
            newNo->next = NULL;
            newNo->left = NULL;
            newNo->right = NULL;

            head = add(head, newNo); // por fim, o novo nó será adicionado na tabela de maneira ordenada
        }
    }

    return head; //retorna a lista
}

//---codificar---

//recebe uma lista e a partir de chamadas recursivas ele constroi a arvore de Huffman
//o processo será juntar os primeiros dois elementos como filhos de um nó e somar suas frequencias, adicionando esse nó de maneira ordenada na lista novamente
//a recursão acaba quando houver apenas um elemento na lista, que agora será a raíz da arvore
no* arvore(no* head){
    if(head){

        if(head->next != NULL){// se a lista tiver mais de um elemento

            no *novoNo = (no*) malloc(sizeof(no)); //aloca espaço para um novo nó
            //checagem de erro
            if(novoNo == NULL){
                printf("Falha na função arvore ao tentar alocar memória para novoNo.\n");
                exit(-1);
            }

            novoNo->item = NULL; //o item desse nó, que vai ser pai dos dois primeiros elementos da lista, não terá um byte/char
            novoNo->freq = head->freq + head->next->freq; // a frequencia dele será a soma das frequencias dos dois primeros elementos d alista
            novoNo->next = NULL; // não tera proximo pois ele vai ser adicionado na lista ainda

            novoNo->left = head; // seu filho a esquerda será o primeiro elemento da lista
            novoNo->right = head->next; // seu filho a direita será o segundo elemento da lista

            head = head->next->next; // a lista tera como cabeça o terceiro elemento (como se tivesse desinfilando o primeiro e segundo elemento)

            head = add(head, novoNo); // esse novo nó será adicionado de maneira ordenada na lista
            return arvore(head); // chamada recursiva (que ao final retornará a arvore feita)
        
        } else {
            return head; //fim da chamada recursiva
        }

    }
}

//funcionando como o printPreOrdem, apenas colocando cada elemento em uma "string"
void auxArvorePreOrdem(no *head, unC *arvorePre, int *i){
    if(head != NULL){

        if(head->item){
            arvorePre[*i] = *(unsigned char*)head->item;

        } else {
            arvorePre[*i] = '*';
        }
        (*i)++; //aumentando o ponteiro, assim a "string" será percorrida normalmente

        auxArvorePreOrdem(head->left, arvorePre, i);
        auxArvorePreOrdem(head->right, arvorePre, i);
    }
}

//a função cria um array arvorePre, o preenche com a função auxiliar com os elementos da arvore e o retorna
unC* pegarArvorePreOrdem(no *head){
    unC *arvorePre = (unC*) malloc(qntsNos(head) + 1); // aloca espaço para o arvorePre, a alocação tem + 1 para o char '\0' que indicará o final
    //checagem de erro
    if(arvorePre == NULL){
        printf("Falha na função pegarArvorePreOrdem ao tentar alocar memória para arvorePre.\n");
        exit(-1);
    }

    int iterador = 0; // cria um "i" para a função auxiliar o usar
    auxArvorePreOrdem(head, arvorePre, &iterador);
    arvorePre[iterador] = '\0'; // coloca \0 no final

    return arvorePre;
}

//cria uma matriz de strings dinamicamente usando um inteiro "colunas" que será o maior tamanho possivel de sequencia de 0 e 1 do dicionario novo
char** criarDicionarioVazio(int colunas){
    char **dicionario = malloc(sizeof(char*) * 256); // aloca 256 espaços para ponteiros de char

    for(int i = 0; i < 256; i++){
        dicionario[i] = calloc(colunas, sizeof(char)); // para cada espaço alocado ele vai alocar uma string com o calloc (para que ela esteja vazia), as colunas será o tamanho máximo que a string pode ter
    }
    
    return dicionario; // retorna o dicionario vazio
}

//vai preencher o dicionario vazio
//"caminho" será uma string vazia "" no inicio
void criarDicionarioCompleto(char **dicionario, no *head, char *caminho, int colunas){
    char left[colunas + 1], right[colunas + 1];  // cria duas strings para cada direção

    if(head->left == NULL && head->right == NULL){ //chegou em uma FOLHA (nó que não tem nenhum filho)
        strcpy(dicionario[*(unsigned char*)head->item], caminho); //copia a string no dicionario na posição do char que head tem como item
        //por exemplo se head->item == 'a', então a expressão ficaria strcpy(dicionario[97], caminho)

    } else {
        //o caminho atual será copiado para as duas strings criadas
        strcpy(left, caminho);
        strcpy(right, caminho);

        //seguindo a lógica da codificação, toda vez que andarmos para esquerda adicionamos um 0, e para direita 1
        strcat(left, "0");
        strcat(right, "1");

        //por fim a chamada recursiva acontece
        criarDicionarioCompleto(dicionario, head->left, left, colunas); //estamos indo para head->left, por isso usamos a string left como o novo caminho
        criarDicionarioCompleto(dicionario, head->right, right, colunas); //estamos indo para head->right, por isso usamos a string right como o novo caminho
    }

}

char* codificar(char **dicionario, unC *dados, LLi tamanhoArquivo){
    LLi tamanho = 0;
    for(LLi i = 0; i < tamanhoArquivo; i++){ //vai iterar pelo arquivo todo
        tamanho += strlen(dicionario[dados[i]]); //ele vai somar o tamanho da sequencia de 0 e 1 nova do byte do arquivo
        //exemplo: se o byte atual de dados[i] fosse 'a' então ele vai pegar o tamanho sequencia nova do 'a' no dicionario e somar em tamanho
    }
    tamanho += 1; //como dados novos é uma string de 0 e 1, precisa ter o '\0' no final
    //
    
    char *dadosNovos = calloc(tamanho, sizeof(char)); //aloca um espaço vazio com o calloc para a string dadosNovos que vai segurar a sequencia de 0 e 1 para o arquivo codificado
    for(LLi i = 0; i < tamanhoArquivo; i++){ //vai iterar pelo arquivo todo novamente
        strcat(dadosNovos, dicionario[dados[i]]); //mas agora ele vai concatenar cada sequencia em dadosNovos
    }

    return dadosNovos; //retorna dadosNovos
}

//---

//--compactar--

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

void escreverCoisaEmBinario(FILE* arquivo, char *dados){
    int i = 0, j = 7;
    unsigned char byte = 0; // 00000000

    while ((dados)[i] != '\0') {

        unsigned char mascara = 1; // 00000001

        if ((dados)[i] == '1') {
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

void compactar(char *dadosNovos, unC *arvorePre, int tamanhoArvorePre){
    char cabecario[17] = {0}; //17 pois o final sera \0
    char lixo[4], tamanhoDaArvore[14];

    //passo 1: pegar o lixo que vai sobrar e transforma-lo em binario para o cabeçario
    int lixoDoArquivoNovo = 8 - (strlen(dadosNovos) % 8);
    if(lixoDoArquivoNovo == 8) lixoDoArquivoNovo = 0;

    printf("Lixo do Arquivo: %d\n", lixoDoArquivoNovo);
    transformandoCoisaEmBinario(lixoDoArquivoNovo, lixo, 4);
    strcat(cabecario, lixo);

    //Passo 2: pegar o tamanho da arvore e transformar em binario para finalizar o cabeçario
    transformandoCoisaEmBinario(tamanhoArvorePre, tamanhoDaArvore, 14);
    strcat(cabecario, tamanhoDaArvore);
    printf("Tamanho cabeçário: %ld\n", strlen(cabecario));
    printf("Cabecario = %s\n", cabecario);

    //Passo 3: Escrever tudo em binário no arquivo novo
    FILE* arquivo = fopen("compactado.huff", "wb");
    //checagem de erro
    if (arquivo == NULL) {
        printf("Falha na função compactar ao tentar criar um arquivo novo.\n");
        return;
    }

    escreverCoisaEmBinario(arquivo, cabecario);

    int i = 0;
    while(arvorePre[i] != '\0'){
        fwrite(&arvorePre[i], sizeof(unsigned char), 1, arquivo);
        i++;
    }

    escreverCoisaEmBinario(arquivo, dadosNovos);

    fclose(arquivo);
}

//---

int main() {
    introducao();

    int opcao;
    scanf("%d", &opcao);

    if(opcao == 1){
        char *nomeDoArquivo = pegarNomeDoArquivo();

        FILE* arquivo = fopen(nomeDoArquivo, LER_BINARIO);
        //checagem de erro
        if (arquivo == NULL) {
            printf("Falha ao abrir o arquivo.\n");
            exit(-1);
        }

        LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
        unC *dados = lerArquivo(arquivo, tamanhoArquivo);

        LLi freq[TAM] = {0};
        for (LLi i = 0; i < tamanhoArquivo; i++) {
            freq[dados[i]] += 1;
        }

        no *listaFreq = inserir(freq);
        listaFreq = arvore(listaFreq);

        unC *arvorePre = pegarArvorePreOrdem(listaFreq);
        int colunas = alturaArvore(listaFreq) + 1;

        char **dicionario = criarDicionarioVazio(colunas);
        criarDicionarioCompleto(dicionario, listaFreq, "", colunas);

        char *dadosNovos = codificar(dicionario, dados, tamanhoArquivo);

        compactar(dadosNovos, arvorePre, qntsNos(listaFreq));
        
        fclose(arquivo);


    } else if(opcao == 2){
        printf("Em construção\n");

    } else {
        exit(-1);

    }

    return 0;
}
