#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF_VAL INT_MAX

typedef struct {
    int next_use_idx;
    int car_id;
} heap_node_t;

typedef struct {
    heap_node_t *buffer;
    size_t size;
    size_t capacity;
} pqueue_t;

pqueue_t* pq_create(size_t capacity) {
    pqueue_t *pq = (pqueue_t*)malloc(sizeof(pqueue_t));
    if (!pq) exit(EXIT_FAILURE);

    pq->buffer = (heap_node_t*)malloc(capacity * sizeof(heap_node_t));
    if (!pq->buffer) exit(EXIT_FAILURE);

    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void pq_destroy(pqueue_t *pq) {
    if (pq) {
        free(pq->buffer);
        free(pq);
    }
}

void pq_swap(heap_node_t *a, heap_node_t *b) {
    heap_node_t temp = *a;
    *a = *b;
    *b = temp;
}

void pq_push(pqueue_t *pq, int next_use_idx, int car_id) {
    if (pq->size >= pq->capacity) return;

    size_t current = pq->size++;
    pq->buffer[current].next_use_idx = next_use_idx;
    pq->buffer[current].car_id = car_id;

    while (current > 0) {
        size_t parent = (current - 1) / 2;
        if (pq->buffer[current].next_use_idx > pq->buffer[parent].next_use_idx) {
            pq_swap(&pq->buffer[current], &pq->buffer[parent]);
            current = parent;
        } else {
            break;
        }
    }
}

heap_node_t pq_pop(pqueue_t *pq) {
    heap_node_t root = pq->buffer[0];
    size_t last_idx = --pq->size;
    pq->buffer[0] = pq->buffer[last_idx];

    size_t current = 0;
    while (1) {
        size_t left = 2 * current + 1;
        size_t right = 2 * current + 2;
        size_t largest = current;

        if (left < pq->size && pq->buffer[left].next_use_idx > pq->buffer[largest].next_use_idx) {
            largest = left;
        }
        if (right < pq->size && pq->buffer[right].next_use_idx > pq->buffer[largest].next_use_idx) {
            largest = right;
        }

        if (largest != current) {
            pq_swap(&pq->buffer[current], &pq->buffer[largest]);
            current = largest;
        } else {
            break;
        }
    }
    return root;
}

int pq_is_empty(pqueue_t *pq) {
    return pq->size == 0;
}

void solve() {
    int N, K, P;

    if (scanf("%d %d %d", &N, &K, &P) != 3) return;

    int *requests = (int*)malloc(P * sizeof(int));
    if (!requests) exit(EXIT_FAILURE);

    for (int i = 0; i < P; i++) {
        scanf("%d", &requests[i]);
    }

    int *next_usage_at = (int*)malloc(P * sizeof(int));
    int *last_seen_pos = (int*)malloc((N + 1) * sizeof(int));

    for (int i = 0; i <= N; i++) last_seen_pos[i] = INF_VAL;

    for (int i = P - 1; i >= 0; i--) {
        int car = requests[i];
        next_usage_at[i] = last_seen_pos[car];
        last_seen_pos[car] = i;
    }

    char *in_cache = (char*)calloc(N + 1, sizeof(char));
    int current_cache_size = 0;

    int *car_latest_next_use = (int*)malloc((N + 1) * sizeof(int));
    for(int i = 0; i <= N; i++) car_latest_next_use[i] = INF_VAL;

    pqueue_t *heap = pq_create(P + 1);

    int operations = 0;

    for (int i = 0; i < P; i++) {
        int current_car = requests[i];
        int next_use = next_usage_at[i];

        car_latest_next_use[current_car] = next_use;

        if (in_cache[current_car]) {
            pq_push(heap, next_use, current_car);
        } else {
            operations++;

            if (current_cache_size < K) {
                in_cache[current_car] = 1;
                current_cache_size++;
                pq_push(heap, next_use, current_car);
            } else {
                while (!pq_is_empty(heap)) {
                    heap_node_t candidate = pq_pop(heap);

                    if (candidate.next_use_idx == car_latest_next_use[candidate.car_id]) {
                        in_cache[candidate.car_id] = 0;
                        break;
                    }
                }

                in_cache[current_car] = 1;
                pq_push(heap, next_use, current_car);
            }
        }
    }

    printf("%d\n", operations);

    free(requests);
    free(next_usage_at);
    free(last_seen_pos);
    free(in_cache);
    free(car_latest_next_use);
    pq_destroy(heap);
}

int main() {
    solve();
    return 0;
}
