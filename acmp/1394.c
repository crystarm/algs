// https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=32&id_topic=57&id_problem=1050
// assignment problem + hungarian algorithm + augmenting path + primal-dual potentials

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long ll;

#ifndef getchar_unlocked
#define getchar_unlocked getchar
#endif

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

static inline int read_int()
{
    int c = getchar_unlocked();
    while (c != '-' && (c < '0' || c > '9'))
    {
        if (c == EOF) return 0;
        c = getchar_unlocked();
    }
    int s = 1;
    if (c == '-')
    {
        s = -1;
        c = getchar_unlocked();
    }
    int x = 0;
    while (c >= '0' && c <= '9')
    {
        x = x * 10 + (c - '0');
        c = getchar_unlocked();
    }
    return x * s;
}

int main()
{

    int n = read_int();
    if (n <= 0) return 0;

    int n1 = n + 1;
    int *a = (int*)malloc((size_t)n1 * (size_t)n1 * sizeof(int));
    if (!a) return 0;

    #define A(i,j) a[(size_t)(i) * (size_t)n1 + (size_t)(j)]

    rep(i,1,n) rep(j,1,n) A(i,j) = read_int();

    const ll LINF = 4000000000000000000LL;

    ll *u = (ll*)calloc((size_t)n1, sizeof(ll));
    ll *v = (ll*)calloc((size_t)n1, sizeof(ll));
    int *p = (int*)calloc((size_t)n1, sizeof(int));
    int *way = (int*)calloc((size_t)n1, sizeof(int));
    ll *minv = (ll*)malloc((size_t)n1 * sizeof(ll));
    unsigned char *used = (unsigned char*)malloc((size_t)n1);

    if (!u || !v || !p || !way || !minv || !used)
    {
        free(a);
        free(u); free(v); free(p); free(way); free(minv); free(used);
        return 0;
    }

    rep(i,1,n)
    {
        p[0] = i;
        int j0 = 0;

        rep(j,0,n)
        {
            minv[j] = LINF;
            used[j] = 0;
        }

        do
        {

            used[j0] = 1;
            int i0 = p[j0];
            ll delta = LINF;
            int j1 = 0;

            rep(j,1,n)
            {
                if (!used[j])
                {
                    ll cur = (ll)A(i0,j) - u[i0] - v[j];
                    if (cur < minv[j])
                    {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta)
                    {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }

            rep(j,0,n)
            {
                if (used[j])
                {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else minv[j] -= delta;
            }

            j0 = j1;
        } while (p[j0] != 0);

        do
        {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }

    printf("%lld\n", (ll)(-v[0]));

    free(a); free(u); free(v); free(p); free(way); free(minv); free(used);

    return 0;
}
