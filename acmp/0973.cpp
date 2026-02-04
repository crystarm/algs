// https://acmp.ru/index.asp?main=task&id_task=973
// shortest-path subgraph construction + bridge-finding (DFS / tin-low)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef vector<char> vc;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); i++)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define fi first
#define se second


const ll LINF = (ll)4e18;

struct edge { int to, w, id; };

struct st { int v; ll d; bool operator > (const st& o) const { return d > o.d; } };

struct raw_edge { int u, v, w, id; };

static void dijkstra(int s, int n, const vector<vector<edge>>& g, vector<ll>& dist)
{
    dist.assign(n + 1, LINF);
    dist[s] = 0;

    priority_queue<st, vector<st>, greater<st>> pq;
    pq.push({s, 0});

    while (!pq.empty())
    {
        st cur = pq.top();
        pq.pop();

        int v = cur.v;
        ll d = cur.d;

        if (d != dist[v]) continue;

        for (auto e : g[v])
        {
            ll nd = d + (ll)e.w;
            if (nd < dist[e.to])
            {
                dist[e.to] = nd;
                pq.push({e.to, nd});
            }
        }
    }
}

static vector<vector<pair<int,int>>> sp;
static vi tin, low;
static vc vis;
static int tim = 0;
static vi br;

static void dfs(int v, int pe)
{
    vis[v] = 1;
    tin[v] = low[v] = tim++;

    for (auto it : sp[v])
    {
        int to = it.fi;
        int id = it.se;

        if (id == pe) continue;

        if (vis[to]) low[v] = min(low[v], tin[to]);
        else
        {
            dfs(to, id);
            low[v] = min(low[v], low[to]);
            if (low[to] > tin[v]) br.pb(id);
        }
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<edge>> g(n + 1);
    vector<raw_edge> es;
    rsr(es, m);

    rep(i, 1, m + 1)
    {
        int u, v, w;
        cin >> u >> v >> w;
        g[u].pb({v, w, i});
        g[v].pb({u, w, i});
        es.pb({u, v, w, i});
    }

    vector<ll> d1, d2;
    dijkstra(1, n, g, d1);
    dijkstra(n, n, g, d2);

    if (d1[n] >= LINF / 2) { cout << 0 << '\n' << '\n'; return 0; }

    ll best = d1[n];

    sp.assign(n + 1, {});
    for (auto e : es)
    {
        bool ok = false;

        if (d1[e.u] < LINF / 2 && d2[e.v] < LINF / 2 && d1[e.u] + (ll)e.w + d2[e.v] == best) ok = true;
        if (!ok && d1[e.v] < LINF / 2 && d2[e.u] < LINF / 2 && d1[e.v] + (ll)e.w + d2[e.u] == best) ok = true;
        if (ok) { sp[e.u].pb({e.v, e.id}); sp[e.v].pb({e.u, e.id}); }
    }

    tin.assign(n + 1, -1);
    low.assign(n + 1, -1);
    vis.assign(n + 1, 0);
    tim = 0;
    br.clear();

    dfs(1, -1);

    sort(all(br));

    cout << sz(br) << '\n';
    rep(i, 0, sz(br)) { if (i) cout << ' '; cout << br[i]; }
    cout << '\n';

    return 0;
}
