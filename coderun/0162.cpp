// https://coderun.yandex.ru/problem/random-swaps
// Markov chain + random swaps + mixing/convergence (eigenvalues) + probabilities

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for(int i=(a); i<(b); i++)
#define pb push_back
#define fi first
#define se second

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    int k;
    cin >> s >> k;

    int n = sz(s);

    int sm = 0, c5 = 0, ce = 0;
    for(char c : s)
    {
        int d = c - '0';
        sm += d;
        if(c == '5') c5++;
        if(c == '2' || c == '4' || c == '6' || c == '8') ce++;
    }

    char lst = s.back();
    bool l5 = (lst == '5');
    bool le = (lst == '2' || lst == '4' || lst == '6' || lst == '8');

    int c5o = c5 - (l5 ? 1 : 0);
    int ceo = ce - (le ? 1 : 0);

    auto pw = [&](ld x, int e)
    {
        ld r = 1.0L;
        while(e > 0)
        {
            if(e & 1) r *= x;
            x *= x;
            e >>= 1;
        }
        return r;
    };

    ld nn = (ld)n;
    ld cf = (ld)(n - 3) / (ld)(n - 1);
    ld a = 1.0L / nn + (1.0L - 1.0L / nn) * pw(cf, k);
    ld po = (1.0L - a) / (nn - 1.0L);

    ld p5 = (l5 ? a : 0.0L) + po * (ld)c5o;
    ld pe = (le ? a : 0.0L) + po * (ld)ceo;

    ld ans = p5;
    if(sm % 3 == 0) ans += pe;

    cout << fixed << setprecision(16) << (double)ans << '\n';
    return 0;
}
