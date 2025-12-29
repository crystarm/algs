// https://acmp.ru/index.asp?main=task&id_task=861
// number theory

#include <bits/stdc++.h>
using namespace std;

using u64 = uint64_t;
using u128 = __uint128_t;

static inline u64 mul_mod(u64 a, u64 b, u64 mod)
{
    return (u128)a * b % mod;
}

static inline u64 pow_mod(u64 a, u64 d, u64 mod)
{
    u64 r = 1;
    while (d)
    {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static inline u64 gcd_u64(u64 a, u64 b)
{
    while (b)
    {
        u64 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool is_prime(u64 n)
{
    if (n < 2) return false;
    static u64 small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (u64 p : small_primes)
    {
        if (n == p) return true;
        if (n % p == 0) return (n == p);
    }

    u64 d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    auto witness = [&](u64 a) -> bool
    {
        if (a % n == 0) return false;
        u64 x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) return false;
        for (u64 i = 1; i < s; ++i)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) return false;
        }
        return true;
    };

    static u64 bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (u64 a : bases)
    {
        if (a >= n) continue;
        if (witness(a)) return false;
    }
    return true;
}

static u64 rng64()
{
    static u64 x = (u64)chrono::high_resolution_clock::now().time_since_epoch().count();
    x += 0x9e3779b97f4a7c15ULL;
    u64 z = x;
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

static u64 pollard_rho(u64 n)
{
    if ((n & 1ULL) == 0) return 2;
    if (n % 3ULL == 0) return 3;

    while (true)
    {
        u64 c = (rng64() % (n - 2)) + 1;
        u64 x = (rng64() % (n - 2)) + 2;
        u64 y = x;
        u64 d = 1;

        auto f = [&](u64 v) -> u64
        {
            return (mul_mod(v, v, n) + c) % n;
        };

        while (d == 1)
        {
            x = f(x);
            y = f(f(y));
            u64 diff = (x > y) ? (x - y) : (y - x);
            d = gcd_u64(diff, n);
        }

        if (d != n) return d;
    }
}

static void factor(u64 n, vector<u64> &out)
{
    if (n == 1) return;
    if (is_prime(n))
    {
        out.push_back(n);
        return;
    }
    u64 d = pollard_rho(n);
    factor(d, out);
    factor(n / d, out);
}

static string to_string_u128(u128 x)
{
    if (x == 0) return "0";
    string s;
    while (x)
    {
        int digit = (int)(x % 10);
        s.push_back(char('0' + digit));
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    u64 N;
    if (!(cin >> N)) return 0;

    if (N == 1)
    {
        cout << 1 << "\n";
        return 0;
    }

    vector<u64> f;
    factor(N, f);
    sort(f.begin(), f.end());

    u128 ans = 1;
    for (size_t i = 0; i < f.size(); )
    {
        u64 p = f[i];
        int a = 0;
        while (i < f.size() && f[i] == p)
        {
            ++a;
            ++i;
        }

        u128 pow_pa_1 = 1;
        for (int k = 1; k <= a - 1; ++k)
        {
            pow_pa_1 *= (u128)p;
        }

        u128 term = pow_pa_1 * (((u128)(a + 1) * (u128)p) - (u128)a);
        ans *= term;
    }

    cout << to_string_u128(ans) << "\n";
    return 0;
}
