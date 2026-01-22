// https://coderun.yandex.ru/problem/linear-postman
// median + greedy + fast-sorting + big-input-optimization

#include <bits/stdc++.h>
using namespace std;


typedef long long ll;
typedef unsigned long long ull;

typedef unsigned int u32_t;
typedef vector<u32_t> vu32_t;


int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    u32_t a, b;
    cin >> n;
    cin >> a >> b;

    u32_t cur = 0;

    auto nr24 = [&]() -> u32_t
    {
        cur = cur * a + b;
        return cur >> 8;
    };

    auto nr32 = [&]() -> u32_t
    {
        u32_t x = nr24();
        u32_t y = nr24();
        return (x << 8) ^ y;
    };

    vu32_t v(n), tmp(n);
    for (int i = 0; i < n; i++)
    {
        v[i] = nr32();
    }

    const int K = 1 << 16;
    static u32_t cnt[K];

    for (int pass = 0; pass < 2; pass++)
    {
        memset(cnt, 0, sizeof(cnt));
        int shift = pass * 16;

        for (u32_t x : v)
        {
            cnt[(x >> shift) & 0xFFFF]++;
        }

        u32_t sum = 0;
        for (int i = 0; i < K; i++)
        {
            u32_t c = cnt[i];
            cnt[i] = sum;
            sum += c;
        }

        for (u32_t x : v)
        {
            tmp[cnt[(x >> shift) & 0xFFFF]++] = x;
        }

        v.swap(tmp);
    }

    u32_t med = v[n / 2];

    ull ans = 0;
    for (u32_t x : v)
    {
        if (x >= med) ans += (ull)(x - med);
        else ans += (ull)(med - x);
    }

    cout << ans << "\n";
    return 0;
}
