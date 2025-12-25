// https://acmp.ru/index.asp?main=task&id_task=11

#include <stdio.h>
#include <stdlib.h>

#define BASE 1000000000

typedef struct list_node
{
    int val;
    struct list_node *prev;
    struct list_node *next;
} lnode_t;

typedef struct big_num
{
    lnode_t *head;
    lnode_t *tail;
} bn_t;

bn_t* bn_alloc(void)
{
    bn_t *res = (bn_t*)malloc(sizeof(bn_t));
    res->head = NULL;
    res->tail = NULL;
    return res;
}

void bn_push(bn_t *b, int v)
{
    lnode_t *n = (lnode_t*)malloc(sizeof(lnode_t));
    n->val = v;
    n->next = NULL;
    n->prev = b->tail;
    if (b->tail) b->tail->next = n;
    else b->head = n;
    b->tail = n;
}

bn_t* bn_init(int v)
{
    bn_t *res = bn_alloc();
    if (v == 0)
    {
        bn_push(res, 0);
        return res;
    }
    while (v > 0)
    {
        bn_push(res, v % BASE);
        v /= BASE;
    }
    return res;
}

void bn_free(bn_t *b)
{
    if (!b) return;
    lnode_t *cur = b->head;
    while (cur)
    {
        lnode_t *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(b);
}

bn_t* bn_add(bn_t *a, bn_t *b)
{
    bn_t *res = bn_alloc();
    lnode_t *pa = a ? a->head : NULL;
    lnode_t *pb = b ? b->head : NULL;
    int cr = 0;
    long long sum;

    while (pa || pb || cr)
    {
        sum = cr;
        if (pa) { sum += pa->val; pa = pa->next; }
        if (pb) { sum += pb->val; pb = pb->next; }
        cr = sum / BASE;
        bn_push(res, (int)(sum % BASE));
    }
    if (!res->head) bn_push(res, 0);
    return res;
}

void bn_print(bn_t *b)
{
    if (!b || !b->tail)
    {
        printf("0\n");
        return;
    }
    printf("%d", b->tail->val);
    lnode_t *cur = b->tail->prev;
    while (cur)
    {
        printf("%09d", cur->val);
        cur = cur->prev;
    }
    printf("\n");
}

int main(void)
{
    /* freopen("INPUT.TXT", "r", stdin); */
    /* freopen("OUTPUT.TXT", "w", stdout); */

    int k, n, i, j;
    if (scanf("%d %d", &k, &n) != 2) return 0;

    if (n == 0)
    {
        printf("0\n");
        return 0;
    }

    bn_t **dp = (bn_t**)malloc((n + 1) * sizeof(bn_t*));

    dp[0] = bn_init(1);

    for (i = 1; i <= n; i++)
    {
        dp[i] = bn_init(0);
        for (j = 1; j <= k; j++)
        {
            if (i - j >= 0) {
                bn_t *tmp = bn_add(dp[i], dp[i - j]);
                bn_free(dp[i]);
                dp[i] = tmp;
            }
        }
    }

    bn_print(dp[n]);

    for (i = 0; i <= n; i++) bn_free(dp[i]);
    free(dp);

    return 0;
}
