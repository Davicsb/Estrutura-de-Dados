#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_CELULAS 100 // Defina um número máximo de células
#define LINHAS 7 // Número de linhas do labirinto
#define COLUNAS 7 // Número de colunas do labirinto

// Estrutura nó com todos os dados relevantes
typedef struct {
    int linha; // Identifica a linha do elemento
    int coluna; // Identifica a coluna do elemento
    int caminhavel; // Indica se a célula é caminhável (1 para sim, 0 para não)
    int f; // f = g + h
    int g; // Custo do início até essa célula
    int h; // Estimativa (heurística) do custo dessa célula até o destino
} No;

typedef struct {
    No* nos[MAX_CELULAS]; // Fila de prioridade com nós
    int tamanho; // Tamanho da fila
} FilaPrioridade;

// FUNÇÕES DA FILA DE PRIORIDADE

// Inicializa a fila
void inicializarFila(FilaPrioridade* fila) {
    fila->tamanho = 0; // Inicia o tamanho da fila como 0
}

// Identifica se a fila está vazia
int estaVazia(FilaPrioridade* fila) {
    return fila->tamanho == 0; // Se o tamanho for 0 retorna 1 (fila vazia), se não retorna 0 (fila não vazia)
}

// Insere um elemento na fila
void enfileirar(FilaPrioridade* fila, No* no) {
    if (fila->tamanho < MAX_CELULAS) {
        fila->nos[fila->tamanho] = no; // Insere o novo nó
        fila->tamanho++; // Atualiza o tamanho da fila

        // Ordena a fila de acordo com o valor de f (menor f primeiro)
        for (int i = fila->tamanho - 1; i > 0; i--) // Começa do final da fila
        {
            if (fila->nos[i]->f < fila->nos[i - 1]->f) // Se o 'f' do elemento mais para perto do inicio da fila for maior troca
            {
                No* temp = fila->nos[i]; // Guarda o nó numa variavel temporária
                fila->nos[i] = fila->nos[i - 1]; // Atribui o valor de um na outra
                fila->nos[i - 1] = temp; // Atribui o outro valor pela temporária que se perdeu
            } 
            else 
            {
                break; // Quebra o for se o outro elemento for menor (ordenado)
            }
        }
    }
}

// Retorna o elemento com menor prioridade (menor valor 'f')
No* desenfileirar(FilaPrioridade* fila) {
    if (!estaVazia(fila)) 
    {
        // Retorna o nó com o menor valor de f (que é o primeiro)
        No* menorNo = fila->nos[0];

        // Ajusta a fila removendo o primeiro elemento
        for (int i = 0; i < fila->tamanho - 1; i++) {
            fila->nos[i] = fila->nos[i + 1]; // Traz todos os elemntos da fila para começar no inicio
        }

        fila->tamanho--; // Atualiza o tamanho da fila
        return menorNo; // Retorna o menor nó
    }
    return NULL; // Retorna NULL se a fila estiver vazia
}

// FUNÇÕES A*

// Calcula o valor 'h' da celula
int calcularH(No* no, No* destino) {
    int h = abs(no->linha - destino->linha) + abs(no->coluna - destino->coluna); // Módulo da linha do nó - linha do destino + Módulo da coluna do nó - coluna do destino
    return h;
}

// Função A* que calcula o caminho
// Recebe o labirinto, a célula de inicio, a célula do destino, o mapa que vai ser retornado para resolver o labirinto, o comprimento do labirinto que vai mudar na main
void aEstrela(No labirinto[LINHAS][COLUNAS], No* inicio, No* destino, No* caminho[MAX_CELULAS], int* caminho_comprimento) {
    FilaPrioridade listaAberta; // Cria a lista aberta (uma fila de prioridade que retorna o menor valor 'f')
    inicializarFila(&listaAberta); // Inicia a lista aberta

    int listaFechada[LINHAS][COLUNAS] = {0}; // Lista fechada, marcando células visitadas (0 não visitada / 1 visitada)
    No* mapaCaminho[LINHAS][COLUNAS]; // Mapeia a célula atual para a célula anterior (achar o caminho de volta)

    // Inicializa os nós do labirinto
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            labirinto[i][j].g = INT_MAX; // Inicia a variavel como infinito para conseguirmos caparar depois
            labirinto[i][j].f = INT_MAX; // Inicia a variavel como infinito para conseguirmos caparar depois
            labirinto[i][j].h = INT_MAX; // Inicia a variavel como infinito para conseguirmos caparar depois
            mapaCaminho[i][j] = NULL; // Como ainda não navegamos pelo labirinto todos apontam para NULL
        }
    }

    // Inicializa a célula inicial
    inicio->g = 0; // Quantidade de 'passos' do inicio para o inicio
    inicio->h = calcularH(inicio, destino); // Calcula o valor 'h' da celula
    inicio->f = inicio->g + inicio->h; // f = g da celula + h da célula

    // Adiciona a célula inicial à lista aberta, para calcularmos seus vizinho
    enfileirar(&listaAberta, inicio);

    while (!estaVazia(&listaAberta)) // Se não houver mais itens na lista aberta não existe caminho para o destino
    {
        No* atual = desenfileirar(&listaAberta); // Remove o nó com o menor valor de f da lista aberta
        listaFechada[atual->linha][atual->coluna] = 1; // Marca o nó atual como visitado

        if (atual->linha == destino->linha && atual->coluna == destino->coluna) // Se a célula atual é o destino, reconstrua o caminho
        {
            No* c = destino; // caminho no destino
            *caminho_comprimento = 0; // Comprimento inicial do caminho de volta = 0

            while (c != NULL) // Enquanto o caminho não estiver apontando pra NULL (partida)
            {
                caminho[(*caminho_comprimento)++] = c; // Mapa para a partida
                c = mapaCaminho[c->linha][c->coluna]; // c pega o valor que essa celula aponta (valor da celula anterior desse caminho)
            }

            return; // Retorna com o mapa para a partida completo
        }

        // Verifica direções possíveis (Norte, Sul, Oeste, Leste)
        int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            int linha_vizinha = atual->linha + direcoes[i][0]; // Faz a soma da linha da celula analisada com a linha da direção (Norte, Sul, Oeste, Leste)
            int coluna_vizinha = atual->coluna + direcoes[i][1]; // Faz a soma da coluna da celula analisada com a coluna da direção (Norte, Sul, Oeste, Leste)

            // Verifica se o vizinho está dentro dos limites se é caminhável e se não está na lista fechada
            if (linha_vizinha >= 0 && linha_vizinha < LINHAS && coluna_vizinha >= 0 && coluna_vizinha < COLUNAS &&
                labirinto[linha_vizinha][coluna_vizinha].caminhavel && !listaFechada[linha_vizinha][coluna_vizinha]) {

                No* vizinho = &labirinto[linha_vizinha][coluna_vizinha]; // Pega a celula do vizinho no labirinto
                int novo_g = atual->g + 1; // Possivel novo g (g atual + '1 passo')

                // Verifica se o vizinho precisa ser atualizado, se ele for maior substitui (já que foi inciado como infinito)
                if (novo_g < vizinho->g) {
                    vizinho->g = novo_g; // Atualiza o g do vizinho
                    vizinho->h = calcularH(vizinho, destino); // Atualiza o h do vizinho
                    vizinho->f = vizinho->g + vizinho->h; // Atuazila o f do vinho

                    mapaCaminho[linha_vizinha][coluna_vizinha] = atual; // Manda o mapa da célula vizinha apontar para a celula anterior (refazer o caminho)

                    enfileirar(&listaAberta, vizinho); // Adiciona o vizinho à fila aberta
                }
            }
        }
    }
    *caminho_comprimento = 0; // Nenhum caminho encontrado
}

int main() {
    // Definindo o labirinto como uma matriz de nós
    // Ordem: linha, coluna, caminhável
    No labirinto[LINHAS][COLUNAS] = {
        {{0, 0, 1}, {0, 1, 1}, {0, 2, 0}, {0, 3, 0}, {0, 4, 1}, {0, 5, 0}, {0, 6, 0}},
        {{1, 0, 1}, {1, 1, 1}, {1, 2, 1}, {1, 3, 1}, {1, 4, 1}, {1, 5, 1}, {1, 6, 1}},
        {{2, 0, 0}, {2, 1, 1}, {2, 2, 1}, {2, 3, 0}, {2, 4, 1}, {2, 5, 0}, {2, 6, 1}},
        {{3, 0, 0}, {3, 1, 1}, {3, 2, 1}, {3, 3, 0}, {3, 4, 1}, {3, 5, 1}, {3, 6, 1}},
        {{4, 0, 0}, {4, 1, 1}, {4, 2, 1}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 1}},
        {{5, 0, 0}, {5, 1, 1}, {5, 2, 1}, {5, 3, 1}, {5, 4, 1}, {5, 5, 1}, {5, 6, 1}},
        {{6, 0, 0}, {6, 1, 1}, {6, 2, 0}, {6, 3, 1}, {6, 4, 0}, {6, 5, 0}, {6, 6, 1}}
    };

    No* inicio = &labirinto[1][0]; // Definindo a célula que começaremos no labirinto
    No* destino = &labirinto[6][6]; // Definindo a célula destino do labirito
    No* caminho[MAX_CELULAS]; // Inicianto o array de nós que nos leva do destino a partida
    int caminho_comprimento = 0; // Inicia o comprimento do mapa que nos leva do destino a partida

    aEstrela(labirinto, inicio, destino, caminho, &caminho_comprimento); // Chama a função A*

    if (caminho_comprimento > 0) // Se o comprimento do caminho for maior que 0 ele achou algum caminho, então reconstroi
    {
        printf("Caminho encontrado: "); // Avisa que um caminho foi encontrado
        for (int i = caminho_comprimento - 1; i >= 0; i--) // Imprime o caminho invertido já que a gente guardou do destino para a partida
        {
            if (i == 0)
            {
                printf("(%d, %d)", caminho[i]->linha, caminho[i]->coluna); // Quando ele tiver chegado no final do for imprime sem '->'
                break;
            }
            printf("(%d, %d) -> ", caminho[i]->linha, caminho[i]->coluna); // Imprime (linha, coluna) da celula
        }
    } 
    else 
    {
        printf("Nenhum caminho encontrado.\n"); // Avisa que nenhum caminho foi encontrado
    }

    return 0; // Programa finalizado com sucesso
}
