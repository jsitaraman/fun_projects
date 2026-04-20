#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000  // max number of nodes

typedef struct {
    int n;               // number of nodes
    int *row_ptr;        // CSR row pointer
    int *col_idx;        // CSR column indices
} SparseGraph;

typedef struct {
    int queue[MAX_N];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

void enqueue(Queue *q, int x) {
    q->queue[q->rear++] = x;
}

int dequeue(Queue *q) {
    return q->queue[q->front++];
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

// Compare function to sort by degree (ascending)
int compare_degree(const void *a, const void *b, void *deg) {
    int *degree = (int *)deg;
    int u = *(int *)a, v = *(int *)b;
    return degree[u] - degree[v];
}

// Perform RCM ordering
void reverse_cuthill_mckee(const SparseGraph *G, int *rcm_order) {
    int visited[MAX_N] = {0};
    int degree[MAX_N];
    int order[MAX_N];
    int idx = 0;

    // Compute degree of each node
    for (int i = 0; i < G->n; i++) {
        degree[i] = G->row_ptr[i + 1] - G->row_ptr[i];
    }

    for (int start = 0; start < G->n; start++) {
        if (visited[start]) continue;

        Queue q;
        initQueue(&q);
        enqueue(&q, start);
        visited[start] = 1;

        while (!isEmpty(&q)) {
            int u = dequeue(&q);
            order[idx++] = u;

            // Gather unvisited neighbors
            int neighbors[MAX_N], cnt = 0;
            for (int j = G->row_ptr[u]; j < G->row_ptr[u + 1]; j++) {
                int v = G->col_idx[j];
                if (!visited[v]) {
                    neighbors[cnt++] = v;
                    visited[v] = 1;
                }
            }

            // Sort neighbors by increasing degree
            qsort(neighbors, cnt, sizeof(int), compare_degree, degree);

            for (int i = 0; i < cnt; i++) {
                enqueue(&q, neighbors[i]);
            }
        }
    }

    // Reverse the order
    for (int i = 0; i < G->n; i++) {
        rcm_order[i] = order[G->n - 1 - i];
    }
}

int main() {
    // Example symmetric graph (undirected), 5 nodes
    // Adjacency: 0: [1,2], 1: [0,2], 2: [0,1,3], 3: [2,4], 4: [3]
    SparseGraph G;
    G.n = 5;
    int row_ptr[6] = {0, 2, 4, 7, 9, 10};
    int col_idx[10] = {1,2, 0,2, 0,1,3, 2,4, 3};
    G.row_ptr = row_ptr;
    G.col_idx = col_idx;

    int rcm_order[5];
    reverse_cuthill_mckee(&G, rcm_order);

    printf("RCM Order: ");
    for (int i = 0; i < G.n; i++) {
        printf("%d ", rcm_order[i]);
    }
    printf("\n");

    return 0;
}

