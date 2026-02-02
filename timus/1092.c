// https://acm.timus.ru/problem.aspx?space=1&num=1092
// GF(2) matrix ops + transversals/permutations + parity invariant + pivot-style elimination

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef getchar_unlocked
#define getchar_unlocked getchar
#endif

#define MAXM 105
#define MAXOPS (MAXM * MAXM)

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

typedef unsigned char u8;

struct op { int r1, c1, r2, c2; };

static int n, m;

static u8 g0[MAXM][MAXM];
static u8 g1[MAXM][MAXM];
static u8 w[MAXM][MAXM];

static struct op ops[MAXOPS];
static int opsn;

static inline int rd(int *out)
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

    *out = x * s;
    return 1;
}

static inline int rds(char *s)
{
    int c = getchar_unlocked();
    while (c <= ' ')
    {
        if (c == EOF) return 0;
        c = getchar_unlocked();
    }

    int len = 0;
    while (c > ' ')
    {
        s[len++] = (char)c;
        c = getchar_unlocked();
    }

    s[len] = 0;
    return len;
}

static inline void flp(int r1, int c1, int r2, int c2)
{
    w[r1][c1] ^= 1;
    w[r1][c2] ^= 1;
    w[r2][c1] ^= 1;
    w[r2][c2] ^= 1;
}

static void print(int r1, int c1, int r2, int c2, int swp)
{
    int p[MAXM];
    u8 used[MAXM];

    rep(i, 0, m - 1) p[i] = -1;
    rep(i, 0, m - 1) used[i] = 0;

    if (!swp)
    {
        p[r1] = c1; used[c1] = 1;
        p[r2] = c2; used[c2] = 1;
    }

    else
    {
        p[r1] = c2; used[c2] = 1;
        p[r2] = c1; used[c1] = 1;
    }

    int col = 0;
    rep(r, 0, m - 1)
    {
        if (p[r] == -1)
        {
            while (col < m && used[col]) ++col;
            p[r] = col;
            used[col] = 1;
        }
    }

    rep(i, 0, m - 1) printf("%d%c", p[i] + 1, (i == m - 1) ? '\n' : ' ');
}

static int slv(u8 g[MAXM][MAXM])
{
    rep(pr, 0, m - 1)
    {
        rep(pc, 0, m - 1)
        {
            rep(i, 0, m - 1) rep(j, 0, m - 1) w[i][j] = g[i][j];
            opsn = 0;

            rep(r, 0, m - 1)
            {
                if (r == pr) continue;
                rep(c, 0, m - 1)
                {
                    if (c == pc) continue;
                    if (w[r][c])
                    {
                        ops[opsn].r1 = pr;
                        ops[opsn].c1 = pc;
                        ops[opsn].r2 = r;
                        ops[opsn].c2 = c;
                        ++opsn;

                        flp(pr, pc, r, c);
                    }
                }
            }

            int rr[MAXM], nr = 0;
            int cc[MAXM], nc = 0;

            rep(r, 0, m - 1) if (r != pr && w[r][pc]) rr[nr++] = r;
            rep(c, 0, m - 1) if (c != pc && w[pr][c]) cc[nc++] = c;


            int kmax = (nr < nc) ? nr : nc;
            for (int k = 0; k < kmax; ++k)
            {
                int r = rr[k];
                int c = cc[k];

                ops[opsn].r1 = pr;
                ops[opsn].c1 = pc;
                ops[opsn].r2 = r;
                ops[opsn].c2 = c;
                ++opsn;

                flp(pr, pc, r, c);
            }

            int cnt = 0;
            rep(i, 0, m - 1) rep(j, 0, m - 1) cnt += (int)w[i][j];

            if (cnt <= 2 * n) return 1;
        }
    }

    return 0;
}

int main()
{
    if (!rd(&n)) return 0;
    m = 2 * n + 1;

    char buf[MAXM + 5];

    rep(i, 0, m - 1)
    {
        if (!rds(buf)) return 0;
        rep(j, 0, m - 1) g0[i][j] = (buf[j] == '+');
    }

    if (slv(g0))
    {
        printf("There is solution:\n");
        for (int k = 0; k < opsn; ++k)
        {
            print(ops[k].r1, ops[k].c1, ops[k].r2, ops[k].c2, 0);
            print(ops[k].r1, ops[k].c1, ops[k].r2, ops[k].c2, 1);
        }
        return 0;
    }

    rep(i, 0, m - 1) rep(j, 0, m - 1) g1[i][j] = g0[i][j];
    rep(i, 0, m - 1) g1[i][i] ^= 1;

    if (slv(g1))
    {
        printf("There is solution:\n");

        rep(i, 0, m - 1)
        {
            printf("%d%c", i + 1, (i == m - 1) ? '\n' : ' ');
        }

        for (int k = 0; k < opsn; ++k)
        {
            print(ops[k].r1, ops[k].c1, ops[k].r2, ops[k].c2, 0);
            print(ops[k].r1, ops[k].c1, ops[k].r2, ops[k].c2, 1);
        }
        return 0;
    }

    printf("No solution\n");
    return 0;
}
