// https://coderun.yandex.ru/problem/ambulance
// integer arithmetic + feasible range of values + parameter recovery from constraints

#include <stdio.h>

typedef long long ll;

static ll cdiv_ll(ll x, ll y) { return (x + y - 1) / y; }

static void locate(ll K, ll M, ll A, ll *P, ll *N)
{
    ll S = A * M;
    ll p = cdiv_ll(K, S);
    ll inside = K - (p - 1) * S;
    ll n = cdiv_ll(inside, A);
    *P = p;
    *N = n;
}

int main(void)
{
    ll K1, M, K2, P2, N2;
    if (scanf("%lld %lld %lld %lld %lld", &K1, &M, &K2, &P2, &N2) != 5) return 0;

    if (N2 > M) { printf("-1 -1\n"); return 0; }

    ll a = (P2 - 1) * M + (N2 - 1);
    ll b = a + 1;

    ll Amin = cdiv_ll(K2, b);
    ll Amax;

    if (a == 0) { Amax = Amin; if (Amax < K1) Amax = K1; }
    else
    {
        Amax = (K2 - 1) / a;
        if (Amin > Amax || Amax <= 0) { printf("-1 -1\n"); return 0; }
    }

    ll P1_min, N1_min, P1_max, N1_max;
    locate(K1, M, Amin, &P1_min, &N1_min);
    locate(K1, M, Amax, &P1_max, &N1_max);

    ll P1 = (P1_min == P1_max) ? P1_min : 0;
    ll N1 = (N1_min == N1_max) ? N1_min : 0;

    printf("%lld %lld\n", P1, N1);
    return 0;
}
