#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_CELULAS 100  // Defina um número máximo de células
#define LINHAS 7         // Número de linhas do labirinto
#define COLUNAS 7        // Número de colunas do labirinto

// Estrutura Nó com todos os dados relevantes
typedef struct {
    int linha;
    int coluna;
    int caminhavel;  // Indica se a célula é caminhável (1 para sim, 0 para não)
    int f;           // f = g + h
    int g;           // Custo do início até essa célula
    int h;           // Estimativa (heurística) do custo dessa célula até o destino
} No;

typedef struct {
    No* nos[MAX_CELULAS];
    int tamanho;
} FilaPrioridade;

// Funções da fila de prioridade
void inicializarFila(FilaPrioridade* fila) {
    fila->tamanho = 0;
}

int estaVazia(FilaPrioridade* fila) {
    return fila->tamanho == 0;
}

void enfileirar(FilaPrioridade* fila, No* no) {
    if (fila->tamanho < MAX_CELULAS) {
        // Insere o elemento na fila
        fila->nos[fila->tamanho] = no;
        fila->tamanho++;

        // Ordena a fila de acordo com o valor de f (menor f primeiro)
        for (int i = fila->tamanho - 1; i > 0; i--) {
            if (fila->nos[i]->f < fila->nos[i - 1]->f) {
                No* temp = fila->nos[i];
                fila->nos[i] = fila->nos[i - 1];
                fila->nos[i - 1] = temp;
            } else {
                break;
            }
        }
    }
}

No* desenfileirar(FilaPrioridade* fila) {
    if (!estaVazia(fila)) {
        // Retorna o nó com o menor valor de f (que é o primeiro)
        No* menorNo = fila->nos[0];

        // Ajusta a fila removendo o primeiro elemento
        for (int i = 0; i < fila->tamanho - 1; i++) {
            fila->nos[i] = fila->nos[i + 1];
        }

        fila->tamanho--;
        return menorNo;
    }
    return NULL; // Retorna NULL se a fila estiver vazia
}

// Funções auxiliares
int calcularH(No* no, No* destino) {
    return abs(no->linha - destino->linha) + abs(no->coluna - destino->coluna);
}

// Função A* que calcula o caminho
void aEstrela(No labirinto[LINHAS][COLUNAS], No* inicio, No* destino, No* caminho[MAX_CELULAS], int* caminho_comprimento) {
    FilaPrioridade filaAberta;
    inicializarFila(&filaAberta);

    int listaFechada[LINHAS][COLUNAS] = {0}; // Lista fechada, marcando células visitadas
    No* mapaCaminho[LINHAS][COLUNAS];        // Mapeia a célula atual para a célula anterior

    // Inicializa os nós do labirinto
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            labirinto[i][j].g = INT_MAX;
            labirinto[i][j].f = INT_MAX;
            mapaCaminho[i][j] = NULL;
        }
    }

    // Inicializa a célula inicial
    inicio->g = 0;
    inicio->h = calcularH(inicio, destino);
    inicio->f = inicio->g + inicio->h;

    // Adiciona a célula inicial à fila de prioridade
    enfileirar(&filaAberta, inicio);

    while (!estaVazia(&filaAberta)) {
        // Remove o nó com o menor valor de f na fila de prioridade
        No* atual = desenfileirar(&filaAberta);
        
        // Marca o nó atual como visitado
        listaFechada[atual->linha][atual->coluna] = 1;

        // Se a célula atual é o destino, reconstrua o caminho
        if (atual->linha == destino->linha && atual->coluna == destino->coluna) {
            No* c = destino;
            *caminho_comprimento = 0;
            while (c != NULL) {
                caminho[(*caminho_comprimento)++] = c;
                c = mapaCaminho[c->linha][c->coluna];
            }
            return;
        }

        // Verifica direções possíveis (Norte, Sul, Oeste, Leste)
        int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int i = 0; i < 4; i++) {
            int linha_vizinha = atual->linha + direcoes[i][0];
            int coluna_vizinha = atual->coluna + direcoes[i][1];

            // Verifica se o vizinho está dentro dos limites e é caminhável
            if (linha_vizinha >= 0 && linha_vizinha < LINHAS && coluna_vizinha >= 0 && coluna_vizinha < COLUNAS &&
                labirinto[linha_vizinha][coluna_vizinha].caminhavel && !listaFechada[linha_vizinha][coluna_vizinha]) {

                No* vizinho = &labirinto[linha_vizinha][coluna_vizinha];
                int novo_g = atual->g + 1;

                // Verifica se o vizinho precisa ser atualizado
                if (novo_g < vizinho->g) {
                    vizinho->g = novo_g;
                    vizinho->h = calcularH(vizinho, destino);
                    vizinho->f = vizinho->g + vizinho->h;
                    mapaCaminho[linha_vizinha][coluna_vizinha] = atual;

                    // Adiciona o vizinho à fila aberta
                    enfileirar(&filaAberta, vizinho);
                }
            }
        }
    }

    *caminho_comprimento = 0; // Nenhum caminho encontrado
}

int main() {
    // Definindo o labirinto como uma matriz de nós
    No labirinto[LINHAS][COLUNAS] = {
        {{0, 0, 1}, {0, 1, 1}, {0, 2, 0}, {0, 3, 0}, {0, 4, 1}, {0, 5, 0}, {0, 6, 0}},
        {{1, 0, 1}, {1, 1, 1}, {1, 2, 1}, {1, 3, 1}, {1, 4, 1}, {1, 5, 1}, {1, 6, 1}},
        {{2, 0, 0}, {2, 1, 1}, {2, 2, 1}, {2, 3, 0}, {2, 4, 1}, {2, 5, 0}, {2, 6, 1}},
        {{3, 0, 0}, {3, 1, 1}, {3, 2, 1}, {3, 3, 0}, {3, 4, 1}, {3, 5, 1}, {3, 6, 1}},
        {{4, 0, 0}, {4, 1, 1}, {4, 2, 1}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 1}},
        {{5, 0, 0}, {5, 1, 1}, {5, 2, 1}, {5, 3, 1}, {5, 4, 1}, {5, 5, 1}, {5, 6, 1}},
        {{6, 0, 0}, {6, 1, 1}, {6, 2, 0}, {6, 3, 1}, {6, 4, 0}, {6, 5, 0}, {6, 6, 1}}
    };

    No* inicio = &labirinto[1][0];
    No* destino = &labirinto[6][6];
    No* caminho[MAX_CELULAS];
    int caminho_comprimento = 0;

    aEstrela(labirinto, inicio, destino, caminho, &caminho_comprimento);

    if (caminho_comprimento > 0) {
        printf("Caminho encontrado: ");
        for (int i = caminho_comprimento - 1; i >= 0; i--) { // Imprime o caminho invertido
            printf("(%d, %d) ", caminho[i]->linha, caminho[i]->coluna);
        }
        printf("\n");
    } else {
        printf("Nenhum caminho encontrado.\n");
    }

    return 0;
}
