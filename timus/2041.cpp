// https://acm.timus.ru/problem.aspx?space=1&num=2041
// greedy + sorting + constructive + balanced BST + nesting
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define rep(i,a,b) for (int i = (a); i < (b); i++)
#define rsr(v,n) (v).reserve(n)
#define pb push_back
#define ins insert

const ll LINF = (ll)4e18;

struct node { ll a, b; int id; };

struct cmp_node
{
    bool operator()(const node &x, const node &y) const
    {
        if (x.a != y.a) return x.a < y.a;
        if (x.b != y.b) return x.b < y.b;
        return x.id < y.id;
    }
};

static bool build_ord(int s0, const vll &a, const vll &b, vi &ord)
{
    int n = sz(a);
    set<node, cmp_node> s;
    rep(i, 0, n)
    {
        if (i == s0) continue;
        s.ins(node{a[i], b[i], i});
    }

    ord.clear();
    rsr(ord, n);
    ord.pb(s0);

    int cur = s0;
    while (!s.empty())
    {
        ll cb = b[cur];
        node key{cb, -LINF, -1};
        auto it = s.lower_bound(key);
        if (it == s.begin())
        {
            return false;
        }
        --it;
        ord.pb(it->id);
        cur = it->id;
        s.erase(it);
    }
    return true;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;

    vll a(n), b(n);
    rep(i, 0, n) cin >> a[i] >> b[i];

    int s1 = 0;
    rep(i, 1, n)
    {
        if (a[i] > a[s1] || (a[i] == a[s1] && b[i] > b[s1]))
        {
            s1 = i;
        }
    }

    int s2 = 0;
    rep(i, 1, n)
    {
        if (b[i] > b[s2] || (b[i] == b[s2] && a[i] > a[s2]))
        {
            s2 = i;
        }
    }

    vi ord;
    if (!build_ord(s1, a, b, ord))
    {
        if (s2 == s1 || !build_ord(s2, a, b, ord))
        {
            cout << "No\n";
            return 0;
        }
    }

    cout << "Yes\n";
    rep(i, 0, n)
    {
        if (i) cout << ' ';
        cout << ord[i] + 1;
    }
    cout << '\n';
    return 0;
}
