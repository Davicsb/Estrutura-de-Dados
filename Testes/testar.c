#include "Code/compactacao.h"
#include "Code/descompactacao.h"
#include "CUnity/unity.h"

void setUp() {};
void tearDown() {};


// COMPACTAÇÃO

void test_criarListaVazia(){
    no* lista = criarListaVazia();
    TEST_ASSERT(lista == NULL);
}

void test_max(){
    TEST_ASSERT_EQUAL(20, max(20, 10));
    TEST_ASSERT_EQUAL(20, max(10, 20));
}

void test_tamanhoDoArquivo(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    TEST_ASSERT_EQUAL(21, tamanhoDoArquivo(arquivo));
}

void test_calcFreq(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    TEST_ASSERT_EQUAL(6, freq[65]);
    TEST_ASSERT_EQUAL(3, freq[68]);
}

void test_alturaArore(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    TEST_ASSERT_EQUAL(4, alturaArvore(listaFreq));
}

void test_qntsNos(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    TEST_ASSERT_EQUAL(11, qntsNos(listaFreq));
}

void test_pegarArvorePreOrdem(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    unC *arvorepreordem = pegarArvorePreOrdem(listaFreq);
    TEST_ASSERT_EQUAL_STRING("**CB***FEDA", arvorepreordem);
}

void test_criarDicionarioCompleto(){
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    int colunas = alturaArvore(listaFreq) + 1;
    char **dicionario = criarDicionarioVazio(colunas);
    criarDicionarioCompleto(dicionario, listaFreq, "", colunas);
    TEST_ASSERT_EQUAL_STRING("11", dicionario[65]);
    TEST_ASSERT_EQUAL_STRING("101", dicionario[68]);
}

void test_add(){
    no* newNo = (no*) malloc(sizeof(no));
    newNo->item = malloc(sizeof(unC));
    newNo->item = "byte";
    newNo->next = NULL;
    newNo->left = NULL;
    newNo->right = NULL;
    no* head = (no*) malloc(sizeof(no));
    head = NULL;
    head = add(head, newNo);
    TEST_ASSERT_EQUAL_STRING("byte", head->item);
}

void test_transformandoCoisaEmBinario(){
    char binario[5];
    transformandoCoisaEmBinario(11, binario, 5);
    TEST_ASSERT_EQUAL_STRING("1011", binario);
    transformandoCoisaEmBinario(9, binario, 5);
    TEST_ASSERT_EQUAL_STRING("1001", binario);
}

// DESCOMPACTAÇÃO

void test_criarNo(){
    unC byte = 00000001;
    no *newnode = criarNo(criarPonteiro(byte));
    TEST_ASSERT_EQUAL_STRING("\x01", newnode->item);
}

void test_readTrash(){
    FILE *arquivo = fopen("texto_teste.txt.huff", "r");
    LLi index = 0;
    LLi tamanho = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanho);
    LLi lixo = readTrash(dados, &index);
    TEST_ASSERT_EQUAL(5, lixo);
}

void test_readSizeTree(){
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    LLi index = 0;
    LLi tamanho = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanho);
    LLi tamanhoArvore = readSizeTree(dados, &index);
    TEST_ASSERT_EQUAL(11, tamanhoArvore);
}

void test_readTree(){
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    LLi index = 0;
    LLi tamanho = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanho);
    LLi tamanhoArvore = readSizeTree(dados, &index);
    no *arvore = readTree(dados, &index, &tamanhoArvore);
    unC *arvorepre = pegarArvorePreOrdem(arvore);
    TEST_ASSERT_EQUAL_STRING("**CB***FEDA", arvorepre);
}

void test_is_on_bit(){
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    LLi index = 0;
    LLi tamanho = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanho);
    unC byte = dados[index];
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 0));
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 1));
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 2));
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 3));
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 4));
    TEST_ASSERT_EQUAL(1, is_on_bit(byte, 5));
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 6));
    TEST_ASSERT_EQUAL(1, is_on_bit(byte, 7));
}

int main(){
    UNITY_BEGIN();
    printf("\nInciando testes (compactacao).\n\n");
    RUN_TEST(test_criarListaVazia);
    RUN_TEST(test_max);
    RUN_TEST(test_tamanhoDoArquivo);
    RUN_TEST(test_calcFreq);
    RUN_TEST(test_alturaArore);
    RUN_TEST(test_qntsNos);
    RUN_TEST(test_pegarArvorePreOrdem);
    RUN_TEST(test_criarDicionarioCompleto);
    RUN_TEST(test_add);
    RUN_TEST(test_transformandoCoisaEmBinario);

    //DESCOMPACTAÇÃO
    printf("\nInciando testes (descompactacao).\n\n");
    RUN_TEST(test_criarNo);
    RUN_TEST(test_readTrash);
    RUN_TEST(test_readSizeTree);
    RUN_TEST(test_readTree);
    RUN_TEST(test_is_on_bit);
    UNITY_END();
    return 0;
}