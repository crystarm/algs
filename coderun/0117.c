// https://coderun.yandex.ru/problem/security
// sweep line technique + prefix sums

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TIME 10000

typedef struct { int start; int end; } Guard;

Guard guards[10005];
int timeline[10005];
int ones_prefix[10005];

void solve()
{
    int N;
    if (scanf("%d", &N) != 1) return;

    memset(timeline, 0, sizeof(timeline));

    for (int i = 0; i < N; i++) {
        scanf("%d %d", &guards[i].start, &guards[i].end);
        if (guards[i].start < guards[i].end)
        {
            timeline[guards[i].start + 1]++;
            if (guards[i].end + 1 <= MAX_TIME) timeline[guards[i].end + 1]--;
        }
    }

    int current_guards = 0;
    int full_coverage = 1;

    for (int t = 1; t <= MAX_TIME; t++)
    {
        current_guards += timeline[t];
        timeline[t] = current_guards;

        if (current_guards == 0) full_coverage = 0;
    }

    if (!full_coverage) {
        printf("Wrong Answer\n");
        return;
    }

    ones_prefix[0] = 0;
    for (int t = 1; t <= MAX_TIME; t++)
    {
        int is_one = (timeline[t] == 1) ? 1 : 0;
        ones_prefix[t] = ones_prefix[t-1] + is_one;
    }

    for (int i = 0; i < N; i++)
    {
        int a = guards[i].start;
        int b = guards[i].end;

        if (a >= b) { printf("Wrong Answer\n"); return; }

        int essential_moments = ones_prefix[b] - ones_prefix[a];

        if (essential_moments == 0) { printf("Wrong Answer\n"); return; }
    }

    printf("Accepted\n");
}

int main() { int K; if (scanf("%d", &K) == 1) while (K--) solve(); return 0; }
