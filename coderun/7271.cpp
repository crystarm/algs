// https://coderun.yandex.ru/problem/language-barier
// trie/prefix-tree + strings/prefix relation + bottom-up DFS greedy pairing + DSU-on-tree.

#include <bits/stdc++.h>
using namespace std;

typedef string str;
typedef long long ll;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpii;
typedef vector<string> vs;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for(int i=(a); i<(b); i++)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define eb emplace_back
#define ppb pop_back
#define fi first
#define se second
#define ins insert

struct slv
{
    struct nd
    { int nx[26]; vi id; nd() { memset(nx, -1, sizeof(nx)); } }; typedef vector<nd> vnd;

    vnd t;
    vpii ans;

    void ins(const str& s, int k)
    {
        int u = 0;
        for(char c : s)
        {
            int x = c - 'a';
            if(t[u].nx[x] == -1)
            {
                t[u].nx[x] = sz(t);
                t.eb();
            }
            u = t[u].nx[x];
        }
        t[u].id.pb(k);
    }

    vi dfs(int u)
    {
        vi buf;

        rep(i,0,26)
        {
            int v = t[u].nx[i];
            if(v == -1) continue;

            vi tmp = dfs(v);

            if(buf.empty())
            {
                buf = move(tmp);
            }
            else
            {
                if(sz(tmp) > sz(buf)) swap(buf, tmp);
                buf.ins(buf.end(), tmp.begin(), tmp.end());
            }
        }

        vi& loc = t[u].id;

        while(!buf.empty() && !loc.empty())
        {
            int a = loc.back();
            loc.ppb();
            int b = buf.back();
            buf.ppb();
            ans.pb({a, b});
        }

        while(sz(loc) >= 2)
        {
            int a = loc.back();
            loc.ppb();
            int b = loc.back();
            loc.ppb();
            ans.pb({a, b});
        }

        if(!loc.empty()) buf.pb(loc.back());

        return buf;
    }

    vpii run(int n, const vs& w)
    {
        (void)n;

        t.clear();
        ans.clear();

        rsr(t, 500005);
        t.eb();

        rep(i,0,sz(w)) ins(w[i], i + 1);

        dfs(0);
        return ans;
    }
};

vpii solve(int n, vs w)
{
    slv s;
    return s.run(n, w);
}
