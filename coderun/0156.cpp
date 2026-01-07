#include <bits/stdc++.h>
using namespace std;

typedef string str;
typedef unordered_map<str,int> umsi;
typedef queue<str> qs;

#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i <= (b); ++i)
#define pb push_back
#define fi first
#define se second
#define rsr(v,n) (v).reserve(n)

struct rule { str a, b; };

typedef vector<rule> vr;

static umsi reach(const str& start, const vr& rules, int maxd)
{
    umsi dist;
    rsr(dist, 4096);

    qs q;
    dist[start] = 0;
    q.push(start);

    while (!q.empty())
    {
        str cur = q.front();
        q.pop();

        int d = dist[cur];
        if (d == maxd) continue;

        for (auto &r : rules)
        {
            const str &from = r.a;
            const str &to = r.b;

            size_t pos = 0;
            while (true)
            {
                pos = cur.find(from, pos);
                if (pos == str::npos) break;

                str nxt;
                rsr(nxt, sz(cur) - sz(from) + sz(to));
                nxt.append(cur, 0, pos);
                nxt += to;
                nxt.append(cur, pos + sz(from), str::npos);

                if (nxt != cur)
                {
                    auto it = dist.find(nxt);
                    if (it == dist.end() || it->se > d + 1)
                    { dist[nxt] = d + 1; q.push(nxt); }
                }

                pos = pos + 1;
            }
        }
    }

    return dist;
}

int main()
{

    ios::sync_with_stdio(0);
    cin.tie(0);

    str s, t;
    if (!(cin >> s)) return 0;
    cin >> t;

    int n;
    cin >> n;

    vr fwd, bwd;
    rsr(fwd, n);
    rsr(bwd, n);

    rep(i, 1, n)
    {

        str a, b;
        cin >> a >> b;
        fwd.pb({a, b});
        bwd.pb({b, a});
    }

    if (s == t) { cout << 0 << '\n'; return 0; }

    umsi d1 = reach(s, fwd, 2);
    umsi d2 = reach(t, bwd, 2);

    int ans = 1e9;

    auto it1 = d1.find(t);
    if (it1 != d1.end()) ans = min(ans, it1->se);

    auto it2 = d2.find(s);
    if (it2 != d2.end()) ans = min(ans, it2->se);

    if (sz(d1) <= sz(d2))
    {
        for (auto &kv : d1)
        {
            auto it = d2.find(kv.fi);
            if (it != d2.end()) ans = min(ans, kv.se + it->se);
        }
    }
    else
    {
        for (auto &kv : d2)
        {
            auto it = d1.find(kv.fi);
            if (it != d1.end()) ans = min(ans, kv.se + it->se);
        }
    }

    if (ans > 4) ans = -1;
    cout << ans << '\n';
    return 0;

}
