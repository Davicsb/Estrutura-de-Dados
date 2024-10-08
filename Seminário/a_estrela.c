#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_CELLS 100  // Defina um número máximo de células
#define LINHAS 7       // Número de linhas do labirinto
#define COLUNAS 7      // Número de colunas do labirinto

// Estrutura Node com todos os dados relevantes
typedef struct {
    int linha;
    int coluna;
    int walkable; // Indica se a célula é caminhável (1 para sim, 0 para não)
    int f;        // f = g + h
    int g;        // Custo do início até essa célula
    int h;        // Estimativa (heurística) do custo dessa célula até o destino
} Node;

typedef struct {
    Node* nodes[MAX_CELLS];
    int size;
} PriorityQueue;

// Funções da fila de prioridade
void initQueue(PriorityQueue* queue) {
    queue->size = 0;
}

int isEmpty(PriorityQueue* queue) {
    return queue->size == 0;
}

void enqueue(PriorityQueue* queue, Node* node) {
    if (queue->size < MAX_CELLS) {
        // Insere o elemento na fila
        queue->nodes[queue->size] = node;
        queue->size++;

        // Ordena a fila de acordo com o valor de f (menor f primeiro)
        for (int i = queue->size - 1; i > 0; i--) {
            if (queue->nodes[i]->f < queue->nodes[i - 1]->f) {
                Node* temp = queue->nodes[i];
                queue->nodes[i] = queue->nodes[i - 1];
                queue->nodes[i - 1] = temp;
            } else {
                break;
            }
        }
    }
}

Node* dequeue(PriorityQueue* queue) {
    if (!isEmpty(queue)) {
        // Retorna o nó com o menor valor de f (que é o primeiro)
        Node* minNode = queue->nodes[0];

        // Ajusta a fila removendo o primeiro elemento
        for (int i = 0; i < queue->size - 1; i++) {
            queue->nodes[i] = queue->nodes[i + 1];
        }

        queue->size--;
        return minNode;
    }
    return NULL; // Retorna NULL se a fila estiver vazia
}

// Funções auxiliares
int h_score(Node* node, Node* destination) {
    return abs(node->linha - destination->linha) + abs(node->coluna - destination->coluna);
}

// Função A* que calcula o caminho
void aestrela(Node maze[LINHAS][COLUNAS], Node* start, Node* destination, Node* caminho[MAX_CELLS], int* caminho_length) {
    PriorityQueue openList;
    initQueue(&openList);

    int closedList[LINHAS][COLUNAS] = {0}; // Lista fechada, marcando células visitadas
    Node* caminho_map[LINHAS][COLUNAS];    // Mapeia a célula atual para a célula anterior

    // Inicializa os nós do labirinto
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            maze[i][j].g = INT_MAX;
            maze[i][j].f = INT_MAX;
            caminho_map[i][j] = NULL;
        }
    }

    // Inicializa a célula inicial
    start->g = 0;
    start->h = h_score(start, destination);
    start->f = start->g + start->h;

    // Adiciona a célula inicial à fila de prioridade
    enqueue(&openList, start);

    while (!isEmpty(&openList)) {
        // Remove o nó com o menor valor de f na fila de prioridade
        Node* current = dequeue(&openList);

        // Marca o nó atual como visitado
        closedList[current->linha][current->coluna] = 1;

        // Se a célula atual é o destino, reconstrua o caminho
        if (current->linha == destination->linha && current->coluna == destination->coluna) {
            Node* c = destination;
            *caminho_length = 0;
            while (c != NULL) {
                caminho[(*caminho_length)++] = c;
                c = caminho_map[c->linha][c->coluna];
            }
            return;
        }

        // Verifica direções possíveis (Norte, Sul, Oeste, Leste)
        int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int i = 0; i < 4; i++) {
            int neighbor_linha = current->linha + direcoes[i][0];
            int neighbor_coluna = current->coluna + direcoes[i][1];

            // Verifica se o vizinho está dentro dos limites e é caminhável
            if (neighbor_linha >= 0 && neighbor_linha < LINHAS && neighbor_coluna >= 0 && neighbor_coluna < COLUNAS &&
                maze[neighbor_linha][neighbor_coluna].walkable && !closedList[neighbor_linha][neighbor_coluna]) {

                Node* neighbor = &maze[neighbor_linha][neighbor_coluna];
                int tentative_g_score = current->g + 1;

                // Verifica se o vizinho precisa ser atualizado
                if (tentative_g_score < neighbor->g) {
                    neighbor->g = tentative_g_score;
                    neighbor->h = h_score(neighbor, destination);
                    neighbor->f = neighbor->g + neighbor->h;
                    caminho_map[neighbor_linha][neighbor_coluna] = current;

                    // Adiciona o vizinho à lista aberta
                    enqueue(&openList, neighbor);
                }
            }
        }
    }

    *caminho_length = 0; // Nenhum caminho encontrado
}

int main() {
    // Definindo o labirinto como uma matriz de nós
    Node maze[LINHAS][COLUNAS] = {
        {{0, 0, 1}, {0, 1, 1}, {0, 2, 0}, {0, 3, 0}, {0, 4, 1}, {0, 5, 0}, {0, 6, 0}},
        {{1, 0, 1}, {1, 1, 1}, {1, 2, 1}, {1, 3, 1}, {1, 4, 1}, {1, 5, 1}, {1, 6, 1}},
        {{2, 0, 0}, {2, 1, 1}, {2, 2, 1}, {2, 3, 0}, {2, 4, 1}, {2, 5, 0}, {2, 6, 1}},
        {{3, 0, 0}, {3, 1, 1}, {3, 2, 1}, {3, 3, 0}, {3, 4, 1}, {3, 5, 1}, {3, 6, 1}},
        {{4, 0, 0}, {4, 1, 1}, {4, 2, 1}, {4, 3, 0}, {4, 4, 1}, {4, 5, 0}, {4, 6, 1}},
        {{5, 0, 0}, {5, 1, 1}, {5, 2, 1}, {5, 3, 1}, {5, 4, 1}, {5, 5, 1}, {5, 6, 1}},
        {{6, 0, 0}, {6, 1, 1}, {6, 2, 0}, {6, 3, 1}, {6, 4, 0}, {6, 5, 0}, {6, 6, 1}}
    };

    Node* start = &maze[1][0];
    Node* destination = &maze[6][6];
    Node* caminho[MAX_CELLS];
    int caminho_length = 0;

    aestrela(maze, start, destination, caminho, &caminho_length);

    if (caminho_length > 0) {
        printf("Caminho encontrado: ");
        for (int i = caminho_length - 1; i >= 0; i--) { // Imprime o caminho invertido
            printf("(%d, %d) ", caminho[i]->linha, caminho[i]->coluna);
        }
        printf("\n");
    } else {
        printf("Nenhum caminho encontrado.\n");
    }

    return 0;
}
