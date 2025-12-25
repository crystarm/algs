// https://coderun.yandex.ru/problem/advertising

#include <stdio.h>
#include <stdlib.h>

typedef struct { int a, b; } Customer;

int compare(const void *a, const void *b) { return (*(int*)a - *(int*)b); }

int main()
{
    int n;
    if (scanf("%d", &n) != 1) return 0;

    Customer *c = malloc(sizeof(Customer) * n);
    int valid_n = 0;
    for (int i = 0; i < n; i++)
    {
        int u, v;
        scanf("%d %d", &u, &v);
        if (v - u >= 5) { c[valid_n].a = u; c[valid_n].b = v; valid_n++; }
    }

    if (valid_n == 0) { printf("0 1 6\n"); return 0; }

    int *pts = malloc(sizeof(int) * (valid_n * 2 + 1));
    int p_cnt = 0;
    for (int i = 0; i < valid_n; i++) { pts[p_cnt++] = c[i].a; pts[p_cnt++] = c[i].a + 5; }
    pts[p_cnt++] = 2000000000;

    qsort(pts, p_cnt, sizeof(int), compare);

    int u_cnt = 0;
    for (int i = 0; i < p_cnt; i++) if (i == 0 || pts[i] != pts[i - 1]) pts[u_cnt++] = pts[i];

    int m_len = (valid_n + 63) / 64;
    unsigned long long **m = malloc(sizeof(unsigned long long*) * u_cnt);
    for (int i = 0; i < u_cnt; i++)
    {
        m[i] = calloc(m_len, sizeof(unsigned long long));
        for (int j = 0; j < valid_n; j++) if (pts[i] >= c[j].a && pts[i] <= c[j].b - 5) m[i][j / 64] |= (1ULL << (j % 64));
    }

    int max_p = -1, ans1 = 1, ans2 = 6;
    for (int i = 0; i < u_cnt; i++)
    {
        for (int j = i; j < u_cnt; j++)
        {
            if (pts[j] < pts[i] + 5) continue;

            int cur = 0;
            for (int k = 0; k < m_len; k++) cur += __builtin_popcountll(m[i][k] | m[j][k]);

            if (cur > max_p) {
                max_p = cur;
                ans1 = pts[i];
                ans2 = pts[j];
            }
            if (max_p == valid_n) break;
        }
        if (max_p == valid_n) break;
    }

    printf("%d %d %d\n", max_p, ans1, ans2);

    for (int i = 0; i < u_cnt; i++) free(m[i]);
    free(m);
    free(pts);
    free(c);

    return 0;
}
