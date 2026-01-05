// https://coderun.yandex.ru/problem/cat-search
// DSU + 4-connectivity + grid + path compression + kinda computer vision idk

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef long long ll;

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

static void *xrealloc(void *p, size_t n)
{
    void *q = realloc(p, n);
    if (!q) exit(0);
    return q;
}

static inline int dsu_find(int *par, int x)
{
    int r = x;
    while (par[r] != r) r = par[r];
    while (par[x] != x)
    {
        int p = par[x];
        par[x] = r;
        x = p;
    }
    return r;
}

static inline int dsu_union(int *par, int *sz, int a, int b)
{
    a = dsu_find(par, a);
    b = dsu_find(par, b);
    if (a == b) return 0;
    if (sz[a] < sz[b])
    {
        int t = a;
        a = b;
        b = t;
    }
    par[b] = a;
    sz[a] += sz[b];
    return 1;
}

int main()
{
    size_t cap = 1 << 20, len = 0;
    char *buf = (char*)malloc(cap + 1);
    if (!buf) return 0;

    while (1)
    {
        if (len == cap)
        {
            cap <<= 1;
            buf = (char*)xrealloc(buf, cap + 1);
        }
        size_t got = fread(buf + len, 1, cap - len, stdin);
        len += got;
        if (got == 0) break;
    }
    buf[len] = 0;

    int *vals = NULL;
    size_t vals_sz = 0, vals_cap = 0;

    size_t *row_st = NULL;
    int *row_len = NULL;
    int n = 0, ncap = 0;

    int cur_len = 0;
    int has_row = 0;

    for (size_t i = 0; i <= len; )
    {
        char c = buf[i];

        if (c == '\r' || c == '\n' || c == 0)
        {
            if (has_row)
            {
                if (n == ncap)
                {
                    ncap = ncap ? ncap * 2 : 16;
                    row_st = (size_t*)xrealloc(row_st, (size_t)ncap * sizeof(size_t));
                    row_len = (int*)xrealloc(row_len, (size_t)ncap * sizeof(int));
                }
                row_st[n] = vals_sz - (size_t)cur_len;
                row_len[n] = cur_len;
                n++;
            }
            cur_len = 0;
            has_row = 0;

            if (c == '\r' && i + 1 <= len && buf[i + 1] == '\n') i++;
            i++;
            continue;
        }

        if (c == ' ' || c == '\t' || c == '\v' || c == '\f')
        {
            i++;
            continue;
        }

        int sgn = 1;
        if (c == '-')
        {
            sgn = -1;
            i++;
        }

        if (i > len) break;
        c = buf[i];
        if (c < '0' || c > '9')
        {
            i++;
            continue;
        }

        ll x = 0;
        while (i <= len)
        {
            c = buf[i];
            if (c < '0' || c > '9') break;
            x = x * 10 + (c - '0');
            i++;
        }
        x *= sgn;

        if (vals_sz == vals_cap)
        {
            vals_cap = vals_cap ? vals_cap * 2 : 256;
            vals = (int*)xrealloc(vals, vals_cap * sizeof(int));
        }
        vals[vals_sz++] = (int)x;

        cur_len++;
        has_row = 1;
    }

    if (n == 0)
    {
        printf("0\n");
        free(buf);
        free(vals);
        free(row_st);
        free(row_len);
        return 0;
    }

    int m = row_len[0];
    if (m <= 0)
    {
        printf("0\n");
        free(buf);
        free(vals);
        free(row_st);
        free(row_len);
        return 0;
    }

    int *a = (int*)malloc((size_t)n * (size_t)m * sizeof(int));
    int *res = (int*)calloc((size_t)n * (size_t)m, sizeof(int));
    if (!a || !res)
    {
        free(buf);
        free(vals);
        free(row_st);
        free(row_len);
        free(a);
        free(res);
        return 0;
    }

    rep(r,0,n-1)
    {
        size_t st = row_st[r];
        int k = row_len[r];
        int take = k < m ? k : m;

        rep(c,0,take-1) a[(size_t)r * (size_t)m + (size_t)c] = vals[st + (size_t)c];
        rep(c,take,m-1) a[(size_t)r * (size_t)m + (size_t)c] = 0;
    }

    int tot = n * m;
    int *par = (int*)malloc((size_t)tot * sizeof(int));
    int *sz = (int*)malloc((size_t)tot * sizeof(int));
    int *rid = (int*)calloc((size_t)tot, sizeof(int));
    if (!par || !sz || !rid)
    {
        free(buf);
        free(vals);
        free(row_st);
        free(row_len);
        free(a);
        free(res);
        free(par);
        free(sz);
        free(rid);
        return 0;
    }

    rep(i,0,tot-1)
    {
        par[i] = i;
        sz[i] = 1;
    }

    rep(r,0,n-1)
    {
        rep(c,0,m-1)
        {
            int id = r * m + c;
            if (a[id] != 1) continue;

            if (r + 1 < n)
            {
                int id2 = (r + 1) * m + c;
                if (a[id2] == 1) dsu_union(par, sz, id, id2);
            }

            if (c + 1 < m)
            {
                int id2 = r * m + (c + 1);
                if (a[id2] == 1) dsu_union(par, sz, id, id2);
            }
        }
    }

    int cat = 0;
    rep(r,0,n-1)
    {
        rep(c,0,m-1)
        {
            int id = r * m + c;
            if (a[id] != 1) continue;

            int root = dsu_find(par, id);
            if (rid[root] == 0) rid[root] = ++cat;
            res[id] = rid[root];
        }
    }

    printf("%d\n", cat);
    rep(r,0,n-1)
    {
        rep(c,0,m-1)
        {
            if (c) putchar(' ');
            printf("%d", res[(size_t)r * (size_t)m + (size_t)c]);
        }
        putchar('\n');
    }

    free(buf);
    free(vals);
    free(row_st);
    free(row_len);
    free(a);
    free(res);
    free(par);
    free(sz);
    free(rid);

    return 0;
}
