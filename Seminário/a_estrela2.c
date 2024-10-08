#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_CELLS 100  // Defina um número máximo de células
#define LINHAS 7         // Número de linhas do labirinto
#define COLUNAS 7         // Número de colunas do labirinto

typedef struct {
    int linha;
    int coluna;
} Cell;

typedef struct {
    Cell cell;
    int f;
    int g;
    int h;
} Node;

typedef struct {
    Node nodes[MAX_CELLS];
    int size;
} PriorityQueue;

// Funções da fila de prioridade
void initQueue(PriorityQueue* queue) {
    queue->size = 0;
}

int isEmpty(PriorityQueue* queue) {
    return queue->size == 0;
}

void enqueue(PriorityQueue* queue, Node node) {
    if (queue->size < MAX_CELLS) {
        queue->nodes[queue->size] = node;
        queue->size++;

        // Reorganiza a fila para manter a ordem
        for (int i = queue->size - 1; i > 0; i--) {
            if (queue->nodes[i].f < queue->nodes[i - 1].f) {
                Node temp = queue->nodes[i];
                queue->nodes[i] = queue->nodes[i - 1];
                queue->nodes[i - 1] = temp;
            } else {
                break;
            }
        }
    }
}

Node dequeue(PriorityQueue* queue) {
    if (!isEmpty(queue)) {
        Node minNode = queue->nodes[0];
        for (int i = 0; i < queue->size - 1; i++) {
            queue->nodes[i] = queue->nodes[i + 1];
        }
        queue->size--;
        return minNode;
    }
    return (Node){{-1, -1}, INT_MAX, INT_MAX, INT_MAX}; // Retorna um nó inválido
}

// Funções auxiliares
int h_score(Cell cell, Cell destination) {
    return abs(cell.linha - destination.linha) + abs(cell.coluna - destination.coluna);
}

int isValid(int maze[LINHAS][COLUNAS], Cell cell) {
    return (cell.linha >= 0 && cell.linha < LINHAS && cell.coluna >= 0 && cell.coluna < COLUNAS && maze[cell.linha][cell.coluna] == 1);
}

// Função A* que calcula o caminho
void aestrela(int maze[LINHAS][COLUNAS], Cell start, Cell destination, Cell caminho[MAX_CELLS], int* caminho_length) {
    int f_score[LINHAS][COLUNAS];
    int g_score[LINHAS][COLUNAS];
    PriorityQueue openList; // Usando a fila de prioridade
    initQueue(&openList);

    // Lista fechada
    int closedList[LINHAS][COLUNAS] = {0}; // Inicializa como não visitado

    // Inicializa as pontuações com infinito
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            f_score[i][j] = INT_MAX;
            g_score[i][j] = INT_MAX;
        }
    }

    // Inicializa a célula inicial
    g_score[start.linha][start.coluna] = 0;
    f_score[start.linha][start.coluna] = g_score[start.linha][start.coluna] + h_score(start, destination);

    // Adiciona a célula inicial à fila de prioridade
    enqueue(&openList, (Node){start, f_score[start.linha][start.coluna], g_score[start.linha][start.coluna], h_score(start, destination)});

    Cell caminho_map[LINHAS][COLUNAS];  // Mapeia a célula atual para a célula anterior
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            caminho_map[i][j] = (Cell){-1, -1};  // Inicializa com -1 indicando que não foi visitado
        }
    }

    while (!isEmpty(&openList)) {
        // Remove o nó com o menor valor de f na fila de prioridade
        Node current = dequeue(&openList);

        // Marca o nó atual como visitado na lista fechada
        closedList[current.cell.linha][current.cell.coluna] = 1;

        // Se a célula atual é o destino, pare
        if (current.cell.linha == destination.linha && current.cell.coluna == destination.coluna) {
            Cell c = destination;
            *caminho_length = 0;
            while (c.linha != start.linha || c.coluna != start.coluna) {
                caminho[(*caminho_length)++] = c;
                c = caminho_map[c.linha][c.coluna];
            }
            caminho[(*caminho_length)++] = start; // Adiciona o início
            return;
        }

        // Verifica direções possíveis (N, S, W, E)
        Cell directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            Cell neighbor = {current.cell.linha + directions[i].linha, current.cell.coluna + directions[i].coluna};

            if (isValid(maze, neighbor) && closedList[neighbor.linha][neighbor.coluna] == 0) {
                int novo_g_score = g_score[current.cell.linha][current.cell.coluna] + 1;
                int novo_f_score = novo_g_score + h_score(neighbor, destination);

                // Verifica se é necessário atualizar os scores
                if (novo_f_score < f_score[neighbor.linha][neighbor.coluna]) {
                    f_score[neighbor.linha][neighbor.coluna] = novo_f_score;
                    g_score[neighbor.linha][neighbor.coluna] = novo_g_score;
                    caminho_map[neighbor.linha][neighbor.coluna] = current.cell;

                    // Adiciona o vizinho à fila de prioridade
                    enqueue(&openList, (Node){neighbor, novo_f_score, novo_g_score, h_score(neighbor, destination)});
                }
            }
        }
    }

    *caminho_length = 0; // Nenhum caminho encontrado
}

int main() {
    int maze[LINHAS][COLUNAS] = {
        {1, 1, 0, 0, 1, 0, 0},
        {1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 0, 1, 0, 1},
        {0, 1, 1, 0, 1, 1, 1},
        {0, 1, 1, 0, 1, 0, 1},
        {0, 1, 1, 1, 1, 1, 1},
        {0, 1, 0, 1, 0, 0, 1}
    };

    Cell start = {1, 0};
    Cell destination = {6, 6};
    Cell caminho[MAX_CELLS];
    int caminho_length = 0;

    aestrela(maze, start, destination, caminho, &caminho_length);

    if (caminho_length > 0) {
        printf("Caminho encontrado: ");
        for (int i = caminho_length - 1; i >= 0; i--) { // Imprime o caminho invertido
            printf("(%d, %d) ", caminho[i].linha, caminho[i].coluna);
        }
        printf("\n");
    } else {
        printf("Nenhum caminho encontrado.\n");
    }

    return 0;
}
