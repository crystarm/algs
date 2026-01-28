// https://coderun.yandex.ru/problem/airline-tickets
// DFS (iterative with stack) + tree traversal (acyclic graph) + path reconstruction

#include <bits/stdc++.h>
using namespace std;

typedef string str;
typedef vector<str> vs;
typedef unordered_set<str> uss;

#define sz(x) (int)((x).size())
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define ppb pop_back
#define ins insert

vs findPath(const str& from, const str& to,
            const function<vs(const str&)>& fetchFlights)
{
    if(from == to) return {from};

    struct frm { str u; int i; vs kids; bool ldd; }; typedef vector<frm> vfrm;

    vfrm st;
    vs path;
    uss vis;

    st.pb({from, 0, {}, 0});
    path.pb(from);
    vis.ins(from);

    while(!st.empty())
    {
        frm &top = st.back();

        if(top.u == to) return path;

        if(!top.ldd)
        {
            top.kids = fetchFlights(top.u);
            top.i = 0;
            top.ldd = 1;
        }

        if(top.i >= sz(top.kids))
        {
            st.ppb();
            path.ppb();
            continue;
        }

        str v = top.kids[top.i++];
        if(vis.count(v)) continue;

        vis.insert(v);
        st.pb({v, 0, {}, 0});
        path.pb(v);
    }

    return {};
}


int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    vs t;
    str s;
    while(cin >> s) t.pb(s);
    if(t.empty()) return 0;

    auto is_int = [&](const str& x)
    {
        if(x.empty()) return 0;
        int i = 0;
        if(x[0] == '-')
        {
            if(sz(x) == 1) return 0;
            i = 1;
        }
        for(; i < sz(x); i++) if(!isdigit((unsigned char)x[i])) return 0;
        return 1;
    };

    str from, to;
    vector<pair<str,str>> e;

    if(is_int(t[0]))
    {
        int m = stoi(t[0]);
        if(sz(t) == 1 + 2 * m + 2)
        {
            for(int i = 0; i < m; i++) e.pb({t[1 + 2 * i], t[1 + 2 * i + 1]});
            from = t[1 + 2 * m];
            to = t[1 + 2 * m + 1];
        }
    }

    if(from.empty() && sz(t) >= 3 && is_int(t[2]))
    {
        int m = stoi(t[2]);
        if(sz(t) == 3 + 2 * m)
        {
            from = t[0];
            to = t[1];
            for(int i = 0; i < m; i++) e.pb({t[3 + 2 * i], t[3 + 2 * i + 1]});
        }
    }

    if(from.empty() && sz(t) >= 2 && (sz(t) - 2) % 2 == 0)
    {
        from = t[0];
        to = t[1];
        for(int i = 2; i < sz(t); i += 2) e.pb({t[i], t[i + 1]});
    }

    if(from.empty() && sz(t) >= 2 && (sz(t) - 2) % 2 == 0)
    {
        from = t[sz(t) - 2];
        to = t[sz(t) - 1];
        for(int i = 0; i + 1 < sz(t) - 2; i += 2) e.pb({t[i], t[i + 1]});
    }

    unordered_map<str, vs> g;
    for(auto &p : e) g[p.fi].pb(p.se);

    function<vs(const str&)> fetchFlights = [&](const str& u) -> vs
    {
        auto it = g.find(u);
        if(it == g.end()) return {};
        return it->second;
    };

    vs ans = findPath(from, to, fetchFlights);

    for(int i = 0; i < sz(ans); i++)
    {
        if(i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
