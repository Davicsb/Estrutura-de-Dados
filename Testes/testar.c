#include "Code/compactacao.h"
#include "Code/descompactacao.h"
#include "CUnity/unity.h"

// Da biblioteca CUnity, chamada antes de cada teste ser executado
// Geralmente usada para configurar qualquer pré-requisito que o teste precise (como alocar memória, inicializar variáveis, abrir arquivos, etc.)
// Como não é necessária, pode ser definida como uma função vazia
void setUp() {}

// Da biblioteca CUnity, chamada depois de cada teste ser executado
// Geralmente usada para limpar os recursos configurados em setUp() (como desalocar memória, fechar arquivos, liberar conexões de rede, etc.)
// Também pode ser deixada vazia se não houver necessidade de limpar nada após o teste
void tearDown() {}


// TESTES - COMPACTAÇÃO

// Função que cria uma lista vazia
void test_criarListaVazia(){
    no* lista = criarListaVazia(); // Função que retorna um nó nulo
    TEST_ASSERT(lista == NULL); // Compara bool
}

// Função que retorna o maior entre dois numeoros
void test_max(){
    // Tira o max entre 20 e 10 e compara com o primeiro elemento (20) se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(20, max(20, 10)); // Compara inteiros ("esperado", nossa função)
    // Tira o max entre 10 e 20 e compara com o primeiro elemento (20) se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(20, max(10, 20)); // Compara inteiros ("esperado", nossa função)
}

// Função que retorna o tamanho de um arquivo
void test_tamanhoDoArquivo(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Compara o tamanho do arquivo teste com o esperado (21) se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(21, tamanhoDoArquivo(arquivo)); // Compara inteiros ("esperado", nossa função)
}

// Função que calcula a frequÊncia de cada byte
void test_calcFreq(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Tira o tamanho desse arquivo
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    // Colca o arquivo em um array de unsigned char
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    // Inicia o array de frquencia como vazio
    LLi freq[TAM] = {0};
    // Calcula a frenquencia dos dados lidos
    calcFreq(dados, tamanhoArquivo, freq);
    // Compara a frequência do 'A' (65 em ASCII) com 6 se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(6, freq[65]); // Compara inteiros ("esperado", nossa função)
    // Compara a frequência do 'D' (68 em ASCII) com 3 se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(3, freq[68]); // Compara inteiros ("esperado", nossa função)
}

// Função que caalcula a altura da arvore de Huffman
void test_alturaArore(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Tira o tamanho desse arquivo
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    // Colca o arquivo em um array de unsigned char
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    // Inicia o array de frquencia como vazio
    LLi freq[TAM] = {0};
    // Calcula a frenquencia dos dados lidos
    calcFreq(dados, tamanhoArquivo, freq);
    // Insere as frequências na fila de prioridade
    no *listaFreq = inserir(freq);
    // Transforma a lista de frequência em uma arvore
    listaFreq = arvore(listaFreq);
    // Compara a altura da arvore com 4 se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(4, alturaArvore(listaFreq)); // Compara inteiros ("esperado", nossa função)
}

// Funçaõ que calcula a quantidade de nós na arvore de Huffman
void test_qntsNos(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Tira o tamanho desse arquivo
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    // Colca o arquivo em um array de unsigned char
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    // Inicia o array de frquencia como vazio
    LLi freq[TAM] = {0};
    // Calcula a frenquencia dos dados lidos
    calcFreq(dados, tamanhoArquivo, freq);
    // Insere as frequências na fila de prioridade
    no *listaFreq = inserir(freq);
    // Transforma a lista de frequência em uma arvore
    listaFreq = arvore(listaFreq);
    // Comprara a quantidade de nós da arvore com 11 se for verdadeiro retorna 1 (sucesso no teste)
    TEST_ASSERT_EQUAL(11, qntsNos(listaFreq)); // Compara inteiros ("esperado", nossa função)
}

// Função que pega a arvore de Huffman e coloca ela em pré ordem para o print
void test_pegarArvorePreOrdem(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Tira o tamanho desse arquivo
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    // Colca o arquivo em um array de unsigned char
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    // Inicia o array de frquencia como vazio
    LLi freq[TAM] = {0};
    // Calcula a frenquencia dos dados lidos
    calcFreq(dados, tamanhoArquivo, freq);
    // Insere as frequências na fila de prioridade
    no *listaFreq = inserir(freq);
    // Transforma a lista de frequência em uma arvore
    listaFreq = arvore(listaFreq);
    // Aloca numa string a arvore em pré ordem
    unC *arvorepreordem = pegarArvorePreOrdem(listaFreq);
    // Compara a string "**CB***FEDA" (resultado esperado), com a string que alocamos usando nossa função
    TEST_ASSERT_EQUAL_STRING("**CB***FEDA", arvorepreordem); // Compara strings ("esperado", nossa função)
}

// Função que cria o dicionário do arquivo
void test_criarDicionarioCompleto(){
    // Lê o arquivo teste
    FILE *arquivo = fopen("texto_teste.txt", LER_BINARIO);
    // Tira o tamanho desse arquivo
    LLi tamanhoArquivo = tamanhoDoArquivo(arquivo);
    // Colca o arquivo em um array de unsigned char
    unC *dados = lerArquivo(arquivo, tamanhoArquivo);
    // Inicia o array de frquencia como vazio
    LLi freq[TAM] = {0};
    // Calcula a frenquencia dos dados lidos
    calcFreq(dados, tamanhoArquivo, freq);
    // Insere as frequências na fila de prioridade
    no *listaFreq = inserir(freq);
    // Transforma a lista de frequência em uma arvore
    listaFreq = arvore(listaFreq);
    // Coloca como coluna a altura da arvore (tamanho máximo do maior byte)
    int colunas = alturaArvore(listaFreq) + 1;
    // Cria o dicionário vazio
    char **dicionario = criarDicionarioVazio(colunas);
    // Cria o dicionário completo
    criarDicionarioCompleto(dicionario, listaFreq, "", colunas);
    // Compara o resuldado do dicionario de 'A' (65 em ASCII) com 11 (resultado esperado)
    TEST_ASSERT_EQUAL_STRING("11", dicionario[65]); // Compara strings ("esperado", nossa função)
    // Compara o resuldado do dicionario de 'A' (65 em ASCII) com 11 (resultado esperado)
    TEST_ASSERT_EQUAL_STRING("101", dicionario[68]); // Compara strings ("esperado", nossa função)
}

// Função que adiciona nós da lista
void test_add(){
    // Aloca a memória do nó
    no* newNo = (no*) malloc(sizeof(no));
    // Aloca memória para o item
    newNo->item = malloc(sizeof(unC));
    // Coloca um item para teste
    newNo->item = "byte";
    // Manda next apontar para nulo
    newNo->next = NULL;
    // Manda o esquerdo apontar para nulo
    newNo->left = NULL;
    // Manda o direito apontar para nulo
    newNo->right = NULL;
    // Aloca a memória para cabeça da lista 
    no* head = (no*) malloc(sizeof(no));
    // Manda head apontar para nulo (lista vazia)
    head = NULL;
    // A cabeça agora é a lista que tinhamos + o nó criado
    head = add(head, newNo);
    // Teste a saida esperada com o item do nó que acabamos de inserir
    TEST_ASSERT_EQUAL_STRING("byte", head->item); // Compara strings ("esperado", nossa função)
}

// Função que tranforma um inteiro em um array que representa o número em binário
void test_transformandoCoisaEmBinario(){
    // Aloca o array para escrevermos o número binário
    char binario[5];
    // Transforma 11 em binário
    transformandoCoisaEmBinario(11, binario, 5);
    // Compara 11 em binário (1011) com a string que acabamos de fazer
    TEST_ASSERT_EQUAL_STRING("1011", binario);
    // Segundo teste : transforma 9 em binário
    transformandoCoisaEmBinario(9, binario, 5); // Compara strings ("esperado", nossa função)
    // Compara 9 em binário (1001) com a string que acabamos de fazer
    TEST_ASSERT_EQUAL_STRING("1001", binario); // Compara strings ("esperado", nossa função)
}

// TESTES - DESCOMPACTAÇÃO

// Função que cria um nó 
void test_criarNo(){
    // Lê o arquivo teste compactado
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    // Pega o tamanho do arquivo para lermos ele todo
    LLi tamanho = tamanhoDoArquivo(arquivo);
    // Colocamos num array os dados lidos
    unC *dados = lerArquivo(arquivo, tamanho);
    // Pega o primeiro byte (10100000)
    unC byte = dados[0];
    // Cria o nó transformando o byte em ponteir
    no *newnode = criarNo(criarPonteiro(byte));
    // Compara o byte esperado 10100000 em hexadecial (\xA0), com o que acabamos de colocar
    // Em C o byte está tipicamente relacionado ao seu valor em hexadecimal
    TEST_ASSERT_EQUAL_STRING("\xA0", newnode->item); // Compara strings ("esperado", nossa função)
}

// Função que lê o lixo do arquivo (três primeiros bytes)
void test_readTrash(){
    // Lê o arquivo teste compactado
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    // Incia o index como 0 para ser o arquvio do inicio
    LLi index = 0;
    // Pega o tamanho do arquivo para lermos ele todo
    LLi tamanho = tamanhoDoArquivo(arquivo);
    // Colocamos num array os dados lidos
    unC *dados = lerArquivo(arquivo, tamanho);
    // Lê o lixo do arquivo
    LLi lixo = readTrash(dados, &index);
    // Compara o lixo esperado (5) com o obtido pela nossa função
    TEST_ASSERT_EQUAL(5, lixo); // Compara inteiros ("esperado", nossa função)
}

// Função que lê o tamanho da arvore do arquivo (três primeiros bytes)
void test_readSizeTree(){
    // Lê o arquivo teste compactado
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    // Incia o index como 0 para ser o arquvio do inicio
    LLi index = 0;
    // Pega o tamanho do arquivo para lermos ele todo
    LLi tamanho = tamanhoDoArquivo(arquivo);
    // Colocamos num array os dados lidos
    unC *dados = lerArquivo(arquivo, tamanho);
    // Lê o tamanho da arvore
    LLi tamanhoArvore = readSizeTree(dados, &index);
    // Compara o tamanho da arvore esperado (11) com o obtido pela nossa função
    TEST_ASSERT_EQUAL(11, tamanhoArvore);  // Compara inteiros ("esperado", nossa função)
}

// Lê a arvore de pré ordem para uma arvore de volta e usa a fução da compactação para comparar a saida
void test_readTree(){
    // Lê o arquivo teste compactado
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    // Incia o index como 0 para ser o arquvio do inicio
    LLi index = 0;
    // Pega o tamanho do arquivo para lermos ele todo
    LLi tamanho = tamanhoDoArquivo(arquivo);
    // Colocamos num array os dados lidos
    unC *dados = lerArquivo(arquivo, tamanho);
    // Lê o tamanho da arvore
    LLi tamanhoArvore = readSizeTree(dados, &index);
    // Tranforma a arvore de volta
    no *arvore = readTree(dados, &index, &tamanhoArvore);
    // Aloca ela numa string para comparação
    unC *arvorepre = pegarArvorePreOrdem(arvore);
    // Compara a arvore esperada "**CB***FEDA" com a obtida
    TEST_ASSERT_EQUAL_STRING("**CB***FEDA", arvorepre); // Compara strings ("esperado", nossa função)
}

// Função que compara se aquela posição do byte é 0 ou 1 (retorna 1 se for 1, retorna 0 se for 0)
void test_is_on_bit(){
    // Lê o arquivo teste compactado
    FILE *arquivo = fopen("texto_teste.txt.huff", LER_BINARIO);
    // Pega o tamanho do arquivo para lermos ele todo
    LLi tamanho = tamanhoDoArquivo(arquivo);
    // Colocamos num array os dados lidos
    unC *dados = lerArquivo(arquivo, tamanho);
    // Pega o priemiro byte do arquivo (10100000)
    unC byte = dados[0];
    // Testa cada bit do byte lido (do final para o começo do byte)
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 0)); // Vê se o ultimo byte (posição 0) é 0
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 1)); // Vê se o penultimo byte (posição 1) é 0
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 2)); // Vê se o antepenultimo byte (posição 2) é 0
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 3)); // Vê se o byte posição 3 é 0
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 4)); // Vê se o byte posição 4 é 0
    TEST_ASSERT_EQUAL(1, is_on_bit(byte, 5)); // Vê se o byte posição 5 é 1
    TEST_ASSERT_EQUAL(0, is_on_bit(byte, 6)); // Vê se o byte posição 6 é 0
    TEST_ASSERT_EQUAL(1, is_on_bit(byte, 7)); // Vê se o primeiro byte (posição 7) é 1
}

int main(){
    UNITY_BEGIN(); // Inicia o framework do CUnity, preparando para os testes
    
    printf("\nInciando testes (compactacao).\n\n");

    // Chama todos os teste da compactação
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
    // Chama todos os testes da descompactação
    RUN_TEST(test_criarNo);
    RUN_TEST(test_readTrash);
    RUN_TEST(test_readSizeTree);
    RUN_TEST(test_readTree);
    RUN_TEST(test_is_on_bit);

    UNITY_END(); // Finaliza a execução dos testes e apresenta os resultados obtidos
    return 0;
}