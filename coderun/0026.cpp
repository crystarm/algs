// https://coderun.yandex.ru/problem/avto
// caching + belady (farthest next use) + heap

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define fi first
#define se second

const int INF = 1e9;

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, k, p;
    if (!(cin >> n >> k >> p)) return 0;

    vi req(p), nxt(p);
    rep(i, 0, p) cin >> req[i];

    if (k <= 0)
    {
        cout << p << '\n';
        return 0;
    }

    vi last(n + 1, INF);
    for (int i = p - 1; i >= 0; --i)
    {
        int x = req[i];
        nxt[i] = last[x];
        last[x] = i;
    }

    vi best(n + 1, INF);
    vector<unsigned char> in(n + 1, 0);

    priority_queue<pair<int, int>> pq;

    int cur = 0;
    int ans = 0;

    rep(i, 0, p)
    {
        int id = req[i];
        int nu = nxt[i];

        best[id] = nu;

        if (in[id])
        {
            pq.push({nu, id});
            continue;
        }

        ++ans;

        if (cur < k)
        {
            in[id] = 1;
            ++cur;
            pq.push({nu, id});
        }
        else
        {
            while (!pq.empty())
            {
                auto t = pq.top();
                pq.pop();

                int vid = t.se;
                int vnu = t.fi;

                if (in[vid] && best[vid] == vnu)
                {
                    in[vid] = 0;
                    --cur;
                    break;
                }
            }

            in[id] = 1;
            ++cur;
            pq.push({nu, id});
        }
    }

    cout << ans << '\n';
    return 0;
}
