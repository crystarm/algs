// https://acmp.ru/?main=task&id_task=223
// stack permutations + process simulation + counting DP + combinatorics

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define B 1000000000u

typedef struct
{
    int n;
    uint32_t *d;
} num;

static void v(num *x)
{
    x->n = 0;
    x->d = NULL;
}

static void g(num *x)
{
    free(x->d);
    x->d = NULL;
    x->n = 0;
}

static void o(num *x)
{
    x->n = 1;
    x->d = (uint32_t *)malloc(sizeof(uint32_t));
    x->d[0] = 1;
}

static int z(const num *x)
{
    return x->n == 0;
}

static void t(num *x)
{
    while (x->n > 0 && x->d[x->n - 1] == 0) x->n--;
    if (x->n == 0)
    {
        free(x->d);
        x->d = NULL;
    }
}

static void a(num *x, const num *y)
{
    int i, n = (x->n > y->n ? x->n : y->n);
    uint64_t c = 0;

    if (x->n < n)
    {
        x->d = (uint32_t *)realloc(x->d, (size_t)n * sizeof(uint32_t));
        for (i = x->n; i < n; i++) x->d[i] = 0;
        x->n = n;
    }

    for (i = 0; i < n; i++)
    {
        uint64_t cur = c + x->d[i] + (i < y->n ? y->d[i] : 0);
        x->d[i] = (uint32_t)(cur % B);
        c = cur / B;
    }

    if (c)
    {
        x->d = (uint32_t *)realloc(x->d, (size_t)(x->n + 1) * sizeof(uint32_t));
        x->d[x->n++] = (uint32_t)c;
    }
}

static num m(const num *x, const num *y)
{
    num r;
    v(&r);

    if (x->n == 0 || y->n == 0) return r;

    r.n = x->n + y->n;
    r.d = (uint32_t *)calloc((size_t)r.n, sizeof(uint32_t));

    for (int i = 0; i < x->n; i++)
    {
        uint64_t c = 0;
        for (int j = 0; j < y->n || c; j++)
        {
            uint64_t cur = r.d[i + j] + c;
            if (j < y->n) cur += (uint64_t)x->d[i] * (uint64_t)y->d[j];
            r.d[i + j] = (uint32_t)(cur % B);
            c = cur / B;
        }
    }

    t(&r);
    return r;
}

static void p(const num *x)
{
    if (x->n == 0)
    {
        printf("0");
        return;
    }

    printf("%u", x->d[x->n - 1]);
    for (int i = x->n - 2; i >= 0; i--) printf("%09u", x->d[i]);
}

static size_t id(int i, int j, int w)
{
    return (size_t)i * (size_t)w + (size_t)j;
}

int main(void)
{
    char s[64], r[64];
    if (scanf("%63s %63s", s, r) != 2) return 0;

    int n = (int)strlen(s);

    num **dp = (num **)calloc((size_t)(n + 1), sizeof(num *));
    int *w = (int *)calloc((size_t)(n + 1), sizeof(int));

    w[0] = n + 1;
    dp[0] = (num *)malloc((size_t)w[0] * (size_t)w[0] * sizeof(num));
    for (int i = 0; i < w[0] * w[0]; i++) o(&dp[0][i]);

    for (int l = 1; l <= n; l++)
    {
        w[l] = n - l + 1;
        dp[l] = (num *)malloc((size_t)w[l] * (size_t)w[l] * sizeof(num));
        for (int i = 0; i < w[l] * w[l]; i++) v(&dp[l][i]);

        for (int i = 0; i + l <= n; i++)
        {
            for (int j = 0; j + l <= n; j++)
            {
                num ans;
                v(&ans);

                for (int k = 0; k < l; k++)
                {
                    if (r[j + k] != s[i]) continue;

                    num *x = &dp[k][id(i + 1, j, w[k])];
                    num *y = &dp[l - k - 1][id(i + k + 1, j + k + 1, w[l - k - 1])];

                    if (z(x) || z(y)) continue;

                    num q = m(x, y);
                    a(&ans, &q);
                    g(&q);
                }

                dp[l][id(i, j, w[l])] = ans;
            }
        }
    }

    p(&dp[n][id(0, 0, w[n])]);
    printf("\n");

    for (int l = 0; l <= n; l++)
    {
        int ww = w[l];
        for (int i = 0; i < ww * ww; i++) g(&dp[l][i]);
        free(dp[l]);
    }
    free(dp);
    free(w);

    return 0;
}
