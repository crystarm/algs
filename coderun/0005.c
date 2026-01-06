// https://coderun.yandex.ru/problem/cafe
// dynamic

#include <stdio.h>

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

const int MAXN = 100;
const int INF = 1000000000;

static int dp[MAXN + 1][MAXN + 1];
static int prv[MAXN + 1][MAXN + 1];
static unsigned char use[MAXN + 1][MAXN + 1];

static int a[MAXN + 1];
static int days[MAXN + 1];

int main()
{
    int n = read_int();
    if (n <= 0) return 0;

    rep(i,1,n) a[i] = read_int();

    rep(i,0,n) rep(j,0,n)
    {
        dp[i][j] = INF;
        prv[i][j] = -1;
        use[i][j] = 0;
    }

    dp[0][0] = 0;

    rep(i,1,n)
    {
        rep(j,0,n)
        {
            if (dp[i - 1][j] >= INF) continue;

            int j2 = j + (a[i] > 100);
            if (j2 <= n)
            {
                int val = dp[i - 1][j] + a[i];
                if (val < dp[i][j2])
                {
                    dp[i][j2] = val;
                    prv[i][j2] = j;
                    use[i][j2] = 0;
                }
            }

            if (j > 0)
            {
                j2 = j - 1;
                int val = dp[i - 1][j];
                if (val < dp[i][j2])
                {
                    dp[i][j2] = val;
                    prv[i][j2] = j;
                    use[i][j2] = 1;
                }
            }
        }
    }

    int best_cost = INF;
    int best_c = 0;

    rep(j,0,n)
    {
        if (dp[n][j] < best_cost || (dp[n][j] == best_cost && j > best_c))
        {
            best_cost = dp[n][j];
            best_c = j;
        }
    }

    printf("%d\n", best_cost);

    int k = 0;
    int i = n;
    int j = best_c;

    while (i > 0)
    {
        if (use[i][j]) days[k++] = i;
        j = prv[i][j];
        --i;
    }

    printf("%d %d\n", best_c, k);
    for (int t = k - 1; t >= 0; --t) printf("%d\n", days[t]);

    return 0;
}
