#include "CodeCompac/compactacao.h"
#include "CUnity/unity.h"

void setUp() {};
void tearDown() {};

void test_criarListaVazia(){
    no* lista = criarListaVazia();
    TEST_ASSERT(lista == NULL);
}

void test_max(){
    TEST_ASSERT_EQUAL(20, max(20, 10));
    TEST_ASSERT_EQUAL(20, max(10, 20));
}

void test_tamanhoDoArquivo(){
    FILE *arquivo = fopen("texto_teste.txt", "r");
    TEST_ASSERT_EQUAL(21, tamanhoDoArquivo(arquivo));
}

void test_calcFreq(){
    FILE *arquivo = fopen("texto_teste.txt", "r");
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    TEST_ASSERT_EQUAL(6, freq[65]);
    TEST_ASSERT_EQUAL(3, freq[68]);
}

void test_alturaArore(){
    FILE *arquivo = fopen("texto_teste.txt", "r");
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    TEST_ASSERT_EQUAL(4, alturaArvore(listaFreq));
}

void test_qntsNos(){
    FILE *arquivo = fopen("texto_teste.txt", "r");
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    LLi freq[TAM] = {0};
    calcFreq(dados, tamanhoArquivo, freq);
    no *listaFreq = inserir(freq);
    listaFreq = arvore(listaFreq);
    TEST_ASSERT_EQUAL(11, qntsNos(listaFreq));
}

void test_criarDicionarioCompleto(){
    FILE *arquivo = fopen("texto_teste.txt", "r");
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

int main(){
    printf("Inciando testes.\n\n");
    UNITY_BEGIN();
    RUN_TEST(test_criarListaVazia);
    RUN_TEST(test_max);
    RUN_TEST(test_tamanhoDoArquivo);
    RUN_TEST(test_calcFreq);
    RUN_TEST(test_alturaArore);
    RUN_TEST(test_qntsNos);
    RUN_TEST(test_criarDicionarioCompleto);
    RUN_TEST(test_add);
    RUN_TEST(test_transformandoCoisaEmBinario);
    UNITY_END();
    return 0;
}