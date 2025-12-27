// https://coderun.yandex.ru/problem/mugs

#include <stdio.h>
#include <stdlib.h>

typedef struct { int *data; int size; } MaxHeap;

void heap_push(MaxHeap *hp, int val)
{
    hp->data[++hp->size] = val;
    int cur = hp->size;
    while (cur > 1 && hp->data[cur] > hp->data[cur / 2])
    {
        int tmp = hp->data[cur];
        hp->data[cur] = hp->data[cur / 2];
        hp->data[cur / 2] = tmp;
        cur /= 2;
    }
}

int heap_pop(MaxHeap *hp)
{
    int res = hp->data[1];
    hp->data[1] = hp->data[hp->size--];
    int cur = 1;
    while (cur * 2 <= hp->size)
    {
        int child = cur * 2;
        if (child + 1 <= hp->size && hp->data[child + 1] > hp->data[child]) child++;

        if (hp->data[cur] < hp->data[child])
        {
            int tmp = hp->data[cur];
            hp->data[cur] = hp->data[child];
            hp->data[child] = tmp;
            cur = child;
        } else break;
    }
    return res;
}


int main()
{
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int *all_prereqs = (int *)malloc(200005 * sizeof(int));
    int *offsets = (int *)malloc((n + 1) * sizeof(int));
    int *k_counts = (int *)malloc((n + 1) * sizeof(int));
    int *out_degree = (int *)calloc(n + 1, sizeof(int));

    int current_offset = 0;
    for (int i = 1; i <= n; i++)
    {
        int k;
        scanf("%d", &k);
        k_counts[i] = k;
        offsets[i] = current_offset;
        for (int j = 0; j < k; j++)
        {
            int p;
            scanf("%d", &p);
            all_prereqs[current_offset++] = p;
            out_degree[p]++;
        }
    }

    MaxHeap hp;
    hp.data = (int *)malloc((n + 1) * sizeof(int));
    hp.size = 0;

    for (int i = 1; i <= n; i++) if (out_degree[i] == 0) heap_push(&hp, i);

    int *result = (int *)malloc(n * sizeof(int));
    int res_ptr = 0;

    while (hp.size > 0)
    {
        int u = heap_pop(&hp);
        result[res_ptr++] = u;

        int start = offsets[u];
        int count = k_counts[u];
        for (int j = 0; j < count; j++)
        {
            int p = all_prereqs[start + j];
            out_degree[p]--;
            if (out_degree[p] == 0) heap_push(&hp, p);
        }
    }

    for (int i = n - 1; i >= 0; i--) printf("%d%c", result[i], (i == 0 ? '\n' : ' '));

    free(all_prereqs);
    free(offsets);
    free(k_counts);
    free(out_degree);
    free(hp.data);
    free(result);

    return 0;
}
