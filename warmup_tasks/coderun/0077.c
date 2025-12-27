// https://coderun.yandex.ru/problem/improving-academic-performance

#include <stdio.h>

int main()
{
    long long a, b, c;
    if (scanf("%lld %lld %lld", &a, &b, &c) != 3) return 0;
    long long val = 3 * a + b - c;
    if (val <= 0) printf("0\n"); else printf("%lld\n", (val + 2) / 3);
    return 0;
}
