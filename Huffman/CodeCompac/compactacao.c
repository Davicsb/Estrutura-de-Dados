#include "compactacao.h"

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
    printf("O que deseja fazer?\n1 - Compactar arquivo\n2 - Descompactar arquivo\n3 - Sair do programa\n> ");
}

void limpartela(){
    system("cls");
}

//retorna NULL como ponteiro para o no
no* criarListaVazia(){
    return NULL;
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
        if(raiz->left == NULL && raiz->right == NULL){ // FOLHA
            if(*(unC*)raiz->item == '*' || *(unC*)raiz->item == '\\'){
                return qntsNos(raiz->left) + qntsNos(raiz->right) + 2; // mais 2 pra contar o contra barra

            } else {
                return qntsNos(raiz->left) + qntsNos(raiz->right) + 1;

            }
        } else {
            return qntsNos(raiz->left) + qntsNos(raiz->right) + 1;

        }
    }

}

//faz a pergunta e escaneia o nome do arquivo dado, retornando a string
char* pegarNomeDoArquivo(){
    printf("Qual o nome completo do arquivo?\n> ");

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

//da free em todos os nós e seus respectivos itens e freqs
void destruirArvore(no *raiz){
    if(raiz){
        if(raiz->item){ // se raíz->item != NULL
            free(raiz->item); //libera o espaço/ponteiro alocado para o void* item
        }

        if(raiz->freq){ //se raíz->item != NULL
            free(raiz->freq); //libera o espaço/ponteiro alocado para o void* item
        }

        destruirArvore(raiz->left);
        destruirArvore(raiz->right);
        free(raiz); // libera o espaço/ponteiro do nó atual
    }
}

//da free em cada string do dicionario e nele mesmo
void destruirDicionario(char **dicionario){
    for(int i = 0; i < TAM; i++){
        free(dicionario[i]); // free em cada string
    }
    free(dicionario); // free nele mesmo
}

//---

//---debug---

//printa todos os elementos da lista, indicando seu byte em hexadecimal, seu char (se for printavel) e sua frequencia
void printar(no *head){
    no *lixo = head;
    while(lixo){
        printf("O byte é 0x%02x (%c) com %d ocorrências\n", *(unC*)lixo->item, *(unC*)lixo->item, *(LLi*)lixo->freq);
        lixo = lixo->next;
    }
}

//printa todos os nós de uma arvore em pre ordem
void printPreOrder(no *head){
    if(head != NULL){
        if(head->left == NULL && head->right == NULL){ // FOLHA
            if(*(unC*)head->item == '*' || *(unC*)head->item == '\\'){
                printf("\\%c", *(unsigned char*)head->item);

            } else {
                printf("%c", *(unsigned char*)head->item);

            }
        } else {
            printf("*");
        }

        printPreOrder(head->left);
        printPreOrder(head->right);
    }

}

//printa os novos bytes do dicionario formado
void imprimirDicionario(char **dicionario){
    for(int i = 0; i < TAM; i++){
        if(dicionario[i][0] != '\0'){
            printf("%3d 0x%02x %s\n", i, i, dicionario[i]);
        }
    }
}

//printa os bytes do arquivo compactado pra um arquivo txt
void imprimirDadosNovos(char *dadosNovos){
    FILE* arquivo = fopen("debug.txt", "w");
    fprintf(arquivo, "%s", dadosNovos);
    fclose(arquivo);
}

//---

//preenche um array de tamanho 256 com a frequencia de cada byte no arquivo
void calcFreq(unC *dados, LLi tamanhoArquivo, LLi *freq){
    for (LLi i = 0; i < tamanhoArquivo; i++) {
        freq[dados[i]] += 1; // na posição equivalente de dados[i] somamos mais um, exemplo se dados[i] == 'a' então freq[97] += 1
    }

}

//adiciona o nó em uma lista de ordem crescente
no* add(no *head, no *new){

    if(head == NULL){ //se a lista estiver vazia
        head = new; //o novo elemento será a nova cabeça

    } else if(*(LLi*)new->freq <= *(LLi*)head->freq){ //se a lista NÃO estiver vazia MAS a frequencia do novo elemento for menor ou igual a frequencia do cabeça
        new->next = head; //o novo elemento terá como proximo o cabeça
        head = new; // e o novo elemento será o novo cabeça

    } else { //se a lista não estiver vazia e a frequencia do novo elemento for maior do que a do cabeça
        no *lixo = head;
        //vamos percorrer a lista até chegar ao fim dela (lixo->next == NULL) ou até a frequencia de um nó da lista for menor que a do novo elemento
        while(lixo->next && *(LLi*)lixo->next->freq < *(LLi*)new->freq){
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

            newNo->freq = malloc(sizeof(LLi)); // aloca espaço de um long long int para o void*
            *(LLi*)newNo->freq = freq[i]; //a frequencia desse nó será a frequecia obtida desse byte

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

            novoNo->freq = malloc(sizeof(LLi));
            *(LLi*)novoNo->freq = *(LLi*)head->freq + *(LLi*)head->next->freq; // a frequencia dele será a soma das frequencias dos dois primeros elementos d alista
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
void auxArvorePreOrdem(no *head, unC *arvorePre, int *i) {
    if (head != NULL) {
        if (head->left == NULL && head->right == NULL) { // FOLHA
            if (*(unC*)head->item == '*' || *(unC*)head->item == '\\') {
                arvorePre[*i] = '\\'; // primeiro, adiciona o caractere de escape
                (*i)++; // incrementa o índice
            }
            arvorePre[*i] = *(unsigned char*)head->item; // adiciona o caractere
            (*i)++; // incrementa o índice novamente
        } else {
            arvorePre[*i] = '*'; // adiciona o caractere para nós internos
            (*i)++; // incrementa o índice
        }

        auxArvorePreOrdem(head->left, arvorePre, i);
        auxArvorePreOrdem(head->right, arvorePre, i);
    }
}

//a função cria um array arvorePre, o preenche com a função auxiliar com os elementos da arvore e o retorna
unC* pegarArvorePreOrdem(no *head){
    unC *arvorePre = (unC*) malloc(qntsNos(head) + 1); // aloca espaço para o arvorePre
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
    char **dicionario = malloc(sizeof(char*) * TAM); // aloca 256 espaços para ponteiros de char

    for(int i = 0; i < TAM; i++){
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

//usando o dicionario, cria-se uma string de dados novos onde será salvo a sequencia de 0s e 1s de cada byte codificado de dados
char* codificar(char **dicionario, unC *dados, LLi tamanhoArquivo) {
    LLi tamanho = 0;
    for (LLi i = 0; i < tamanhoArquivo; i++) {
        tamanho += strlen(dicionario[dados[i]]); // itera por dados somando o tamanho de cada byte novo para pegar o tamanho total da sequencia
    }
    tamanho += 1; // para o terminador nulo

    char *dadosNovos = (char *)malloc(tamanho * sizeof(char)); //aloca o espaço para dados novos
    //checagem de erro
    if (dadosNovos == NULL) {
        printf("Falha na função codificar ao alocar memória para dadosNovos.\n");
        exit(-1);
    }

    char *ptr = dadosNovos; // usamos um ponteiro auxiliar para construir a sequencia, que aponta para o inicio da string
    for (LLi i = 0; i < tamanhoArquivo; i++) { // iteramos por dados de novo
        const char *codigo = dicionario[dados[i]]; // pega a codificação correspondente de dados[i]
        LLi codigoLen = strlen(codigo); // pega o tamanho da sequencia do novo byte

        memcpy(ptr, codigo, codigoLen); // copia a codificação para a posição atual de ptr
        ptr += codigoLen; // ptr é incrementado para o final (atual) da string para a proxima sequencia
    }
    *ptr = '\0'; // adicionamos o \0 no final

    return dadosNovos;
}

//---

//--compactar--

//transformar um numero coisa em um número binário em uma string bin
void transformandoCoisaEmBinario(int coisa, char *bin, int tamanhoDeBin){
    for(int i = 0; i < tamanhoDeBin - 1; i++){ // itera até antes do fim da string setando '0' em todas as posições
        bin[i] = '0';
    }
    bin[tamanhoDeBin - 1] = '\0'; // coloca \0 no final

    int i = tamanhoDeBin - 2; // a manipulação vai começar antes do fim da string, seguindo o padrão da conversão de decimal para binário
    while(coisa > 0 && i >= 0){
        bin[i] = (coisa % 2) + '0'; // bin[i] vai receber o resto da divisão de coisa e 2
        coisa /= 2; // coisa vai ser dividido
        i--; // avançamos até o começo da sequencia
    }
}

//escreve uma string de 0s e 1s como bytes em um arquivo
void escreverCoisaEmBinario(FILE* arquivo, char *dados){
    int i = 0, j = 7; // i será o iterador e j será a posição do bit atual
    unsigned char byte = 0; // == 00000000, criação do byte onde as manipulações serão salvas e depois escritas no arquivo

    while ((dados)[i] != '\0') {

        unsigned char mascara = 1; // 00000001, byte que será alterado para a manipulação

        if ((dados)[i] == '1') {
            mascara = mascara << j; // "empurra" o bit em direção ao começo da sequencia, exemplo: se j == 6 então mascara == 01000000
            byte = byte | mascara; // faz a operação OU entre byte e mascara, como se fosse uma "soma" de bits, setando 1 na posição j de byte
        }
        j--; // avançando a posição em direção ao final

        if (j < 0) { // se j == -1
            fwrite(&byte, sizeof(unsigned char), 1, arquivo); // ele escreve o byte pronto no arquivo e reseta 'byte' e j
            byte = 0;
            j = 7;
        }
        i++;

    }

    if (j != 7) { // se no fim j não foi resetado significa que há algo em 'byte' para ser escrito 
        fwrite(&byte, sizeof(unsigned char), 1, arquivo);
    }

}

//faz os procedimentos para escrever o cabeçario e os dados novos em um arquivo novo
void compactar(char *dadosNovos, unC *arvorePre, int tamanhoArvorePre, char *nomeOriginal){
    char nomeModificado[strlen(nomeOriginal) + 6]; // cria uma string que vai ser o nome do arquivo de saída, o + 6 é para o .huff
    strcpy(nomeModificado, nomeOriginal); //copia o nome original para o nome modificado
    strcat(nomeModificado, ".huff"); // adiciona .huff no final da string

    char cabecario[17] = {0}; //definindo elementos como \0, 17 pois o char \0 final indica o fim da string
    char lixo[4], tamanhoDaArvore[14]; // criando arrays auxiliares para os binários do lixo e tamanho da arvore

    //passo 1: pegar o lixo que vai sobrar e transforma-lo em binario para o cabeçario
    int lixoDoArquivoNovo = 8 - (strlen(dadosNovos) % 8); // esse calculo vai pegar os bits que não foram usados no byte final (lixo)
    if(lixoDoArquivoNovo == 8) lixoDoArquivoNovo = 0; // se strlen(dadosNovos) % 8 == 0 então não há lixo

    transformandoCoisaEmBinario(lixoDoArquivoNovo, lixo, 4); // vai pegar o numero x do lixo e transforma-lo em uma string representando número binário em lixo
    strcat(cabecario, lixo); // concatena lixo em cabeçario (que no caso está vazio)

    //Passo 2: pegar o tamanho da arvore e transformar em binario para finalizar o cabeçario
    transformandoCoisaEmBinario(tamanhoArvorePre, tamanhoDaArvore, 14); // pega o numero x do tamanho e o transforma em uma string representando seu número binário
    strcat(cabecario, tamanhoDaArvore); // concatena o tamanho da arvore no cabeçario, o finalizando

    //Passo 3: Escrever tudo em binário no arquivo novo
    FILE* arquivo = fopen(nomeModificado, ESCREVER_BINARIO); //criando o arquivo "compactado.huff"
    //checagem de erro
    if (arquivo == NULL) {
        printf("Falha na função compactar ao tentar criar um arquivo novo.\n");
        return;
    }

    escreverCoisaEmBinario(arquivo, cabecario); //por meio de manipulações booleanas vai escrever o cabeçario no (começo do) arquivo

    for(int i = 0; i < tamanhoArvorePre; i++){ // iterando até o fim da string
        fwrite(&arvorePre[i], sizeof(unsigned char), 1, arquivo); // escreve os bytes que estão na arvore no arquivo, como eles são unsigned chars não nescessita manipulações
    }

    escreverCoisaEmBinario(arquivo, dadosNovos); // por fim escreve as sequencias de 0s e 1s de dados novos no arquivo

    fclose(arquivo); // fecha o arquivo para não corromper
}

//---

//---funções principais---

//função que chama todos os processos para a compactação
void processoParaCompactar(char *nomeDoArquivo){

    FILE *arquivo = fopen(nomeDoArquivo, LER_BINARIO); //pega o ponteiro do arquivo no modo de leitura binaria
    //checagem de erro
    if (arquivo == NULL) {
        printf("Falha ao abrir o arquivo.\n");
        exit(-1);
    }

    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo); // pega quantos bytes tem no arquivo como tamanho
    unC *dados = lerArquivo(arquivo, tamanhoArquivo); // faz a leitura dos bytes e os salva em dados

    LLi freq[TAM] = {0}; // cria um array para contar a frequencia de cada byte
    calcFreq(dados, tamanhoArquivo, freq); // preenche o array com cada frequencia

    no *listaFreq = inserir(freq); // com o array de frenquencias preenchemos uma lista com cada byte e sua frequencia
    listaFreq = arvore(listaFreq); // com a lista fazemos a arvore de Huffman

    unC *arvorePre = pegarArvorePreOrdem(listaFreq); // pega a arvore em uma "string" em pre ordem

    int colunas = alturaArvore(listaFreq) + 1; // pega o caminho máximo que um novo byte pode ter na criação do dicionario

    char **dicionario = criarDicionarioVazio(colunas); // cria uma matriz de strings dicionario, com 256 linhas e x colunas
    criarDicionarioCompleto(dicionario, listaFreq, "", colunas); // preenche o dicionario com a arvore de Huffman

    char *dadosNovos = codificar(dicionario, dados, tamanhoArquivo); // codifica os bytes de dados com o dicionario em uma string de 0s e 1s

    compactar(dadosNovos, arvorePre, qntsNos(listaFreq), nomeDoArquivo); // escreve os dados novos em binário junto com o cabeçário

    //libera a memória alocada e fecha o arquivo
    free(nomeDoArquivo);
    free(dados);
    free(arvorePre);
    destruirArvore(listaFreq);
    destruirDicionario(dicionario);
    free(dadosNovos);

    fclose(arquivo);
}

//--