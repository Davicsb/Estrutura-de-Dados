//lendo e escrevendo arquivos em binário
//o programa é simples, só pega os bytes de um arquivo e copia eles para um novo arquivo .supla
//o objetivo é apenas para testes.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LER_BINARIO "rb"
#define ESCREVER_BINARIO "wb"

//define o modo para a variavel FILE, r para ler (read) e w para escrever (write)
//o 'b'no final é um modificador, que faz com que o r e w funcionem apenas em binario
//^^^^^ eu deveria detalhar mais

void mudar(char *nomeArquivo, char *nomeMudado){

    int i;
    for(i = 0; i < strlen(nomeArquivo); i++){
        nomeMudado[i] = nomeArquivo[i];

        if(nomeArquivo[i] == '.'){
            break;
        }

    }

    nomeMudado[i + 1] = 's';
    nomeMudado[i + 2] = 'u';
    nomeMudado[i + 3] = 'p';
    nomeMudado[i + 4] = 'l';
    nomeMudado[i + 5] = 'a';
    nomeMudado[i + 6] = '\0';
}

int main(){

    printf("Qual o nome do arquivo? (inclua sua extensão)\n");

    char nomeArquivo[1024];
    fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
    nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;
    //talvez eu nao use o fgets por causa desse \n no final q tem q tirar

    //printf("%s\n", nomeArquivo);
    
    FILE* arquivo = fopen(nomeArquivo, LER_BINARIO);
    if(arquivo == NULL){
        printf("Falha ao abrir o arquivo.\n");

        exit(-1);
    }

    //cria o nome do arquivo de saída, (nome do arquivo).supla <-- extensão provisoria hehehehehehe
    char nomeMudado[1024];
    mudar(nomeArquivo, nomeMudado);

    FILE* saida = fopen(nomeMudado, ESCREVER_BINARIO);
    if (saida == NULL) {
        printf("Falha ao criar o arquivo de saída\n");
        fclose(arquivo);
        exit(-1);
    }

    //move o ponteiro do pro final do arquivo
    fseek(arquivo, 0, SEEK_END);

    //pega o tamanho do arquivo
    unsigned long tamanhoArquivo = ftell(arquivo);
    //^^^ será que eu devo colocar unsigned?

    //move o ponteiro pro inicio de novo
    fseek(arquivo, 0, SEEK_SET);

    //printf("O arquivo tem %ld bytes", tamanhoArquivo);

    //vai alocar o espaço suficiente pra o arquivo inteiro
    //agora será q lista encadeada seria melhor ou nem?
    unsigned char* dados = (unsigned char*) malloc(tamanhoArquivo);

    if(dados == NULL){
        printf("Falha ao alocar memória.");
        exit(-1);
    }

    //le os dados do arquivo
    fread(dados, 1, tamanhoArquivo, arquivo);

    //copia para a saída (nome do arquivo).supla
    //como o arquivo é só uma cópia com uma extensão diferente, você pode alterar o .supla pra a extensão original e o arquivo vai ser o mesmo
    fwrite(dados, 1, tamanhoArquivo, saida);

    free(dados);
    fclose(arquivo);
    fclose(saida);

    return 0;
}
