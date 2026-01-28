// https://coderun.yandex.ru/problem/mtf-encoding-decoding
// Move-to-Front (MTF) transform + dynamic permutation / self-organizing list + order statistics + Fenwick tree (BIT)

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

#define rep(i,a,b) for(int i=(a); i<(b); i++)

struct fw
{
    int n;
    vi f;

    void init(int n_)
    {
        n = n_;
        f.assign(n + 1, 0);
    }

    void add(int i, int v)
    {
        for(; i <= n; i += i & -i) f[i] += v;
    }

    int sum(int i)
    {
        int r = 0;
        for(; i > 0; i -= i & -i) r += f[i];
        return r;
    }

    int kth(int k)
    {
        int idx = 0;
        int cur = 0;

        int step = 1;
        while((step << 1) <= n) step <<= 1;

        for(; step; step >>= 1)
        {
            int nxt = idx + step;
            if(nxt <= n && cur + f[nxt] < k)
            {
                idx = nxt;
                cur += f[nxt];
            }
        }

        return idx + 1;
    }
};

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m, tp;
    if(!(cin >> n >> m >> tp)) return 0;

    if(n == 0)
    {
        cout << '\n';
        return 0;
    }

    int lim = n + m;

    fw ft;
    ft.init(lim);

    vi pos(m + 1);
    vi at(lim + 1);

    rep(i,1,m + 1)
    {
        int p = n + i;
        pos[i] = p;
        at[p] = i;
        ft.add(p, 1);
    }

    int head = n;

    rep(i,0,n)
    {
        int x;
        cin >> x;

        if(tp == 1)
        {
            int sym = x;
            int p = pos[sym];
            int r = ft.sum(p);

            cout << r << (i + 1 == n ? '\n' : ' ');

            ft.add(p, -1);
            pos[sym] = head;
            at[head] = sym;
            ft.add(head, 1);
            head--;
        }
        else
        {
            int r = x;
            int p = ft.kth(r);
            int sym = at[p];

            cout << sym << (i + 1 == n ? '\n' : ' ');

            ft.add(p, -1);
            pos[sym] = head;
            at[head] = sym;
            ft.add(head, 1);
            head--;
        }
    }

    return 0;
}
