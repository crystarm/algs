// https://coderun.yandex.ru/problem/cafe

#include <stdio.h>

#define MAXN 100
#define INF 1000000000

static int dp[MAXN + 1][MAXN + 1];
static int prev_j[MAXN + 1][MAXN + 1];
static char used[MAXN + 1][MAXN + 1];
static int cost[MAXN + 1];
static int days[MAXN + 1];

int main(void)
{
    int n;
    int i, j;

    if (scanf("%d", &n) != 1) return 0;


    for (i = 1; i <= n; i++) { scanf("%d", &cost[i]); }

    for (i = 0; i <= n; i++)
    {
        for (j = 0; j <= n; j++)
        {
            dp[i][j] = INF;
            prev_j[i][j] = -1;
            used[i][j] = 0;
        }
    }

    dp[0][0] = 0;

    for (i = 1; i <= n; i++)
    {
        for (j = 0; j <= n; j++)
        {
            if (dp[i - 1][j] < INF)
            {
                int j2;
                int val;

                j2 = j + (cost[i] > 100 ? 1 : 0);
                if (j2 <= n)
                {
                    val = dp[i - 1][j] + cost[i];
                    if (val < dp[i][j2])
                    {
                        dp[i][j2] = val;
                        prev_j[i][j2] = j;
                        used[i][j2] = 0;
                    }
                }

                if (j > 0)
                {
                    j2 = j - 1;
                    val = dp[i - 1][j];
                    if (val < dp[i][j2])
                    {
                        dp[i][j2] = val;
                        prev_j[i][j2] = j;
                        used[i][j2] = 1;
                    }
                }
            }
        }
    }

    int min_cost = INF;
    int best_coupons = 0;

    for (j = 0; j <= n; j++)
    {
        if (dp[n][j] < min_cost || (dp[n][j] == min_cost && j > best_coupons))
        {
            min_cost = dp[n][j];
            best_coupons = j;
        }
    }

    printf("%d\n", min_cost);

    int k2 = 0;
    i = n;
    j = best_coupons;

    while (i > 0)
    {
        if (used[i][j]) days[k2++] = i;
        j = prev_j[i][j];
        i--;
    }

    printf("%d %d\n", best_coupons, k2);

    for (i = k2 - 1; i >= 0; i--) { printf("%d\n", days[i]); }

    return 0;
}
