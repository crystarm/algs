// https://acmp.ru/index.asp?main=task&id_task=11
// arbitrary precision arithmetic

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back

static const int BASE = 1000000000;

struct lnd { int val; lnd* pr; lnd* nx; };

struct bn { lnd* h; lnd* t; };

static bn* bn_alloc()
{
    bn* res = new bn;
    res->h = NULL;
    res->t = NULL;
    return res;
}

static void bn_push(bn* b, int v)
{
    lnd* n = new lnd;
    n->val = v;
    n->nx = NULL;
    n->pr = b->t;
    if (b->t) b->t->nx = n;
    else b->h = n;
    b->t = n;
}

static bn* bn_init(int v)
{
    bn* res = bn_alloc();
    if (v == 0)
    {
        bn_push(res, 0);
        return res;
    }
    while (v > 0)
    {
        bn_push(res, v % BASE);
        v /= BASE;
    }
    return res;
}

static void bn_free(bn* b)
{
    if (!b) return;
    lnd* cur = b->h;
    while (cur)
    {
        lnd* tmp = cur;
        cur = cur->nx;
        delete tmp;
    }
    delete b;
}

static bn* bn_add(bn* a, bn* b)
{
    bn* res = bn_alloc();
    lnd* pa = a ? a->h : NULL;
    lnd* pb = b ? b->h : NULL;
    ll cr = 0;

    while (pa || pb || cr)
    {
        ll sum = cr;
        if (pa) { sum += pa->val; pa = pa->nx; }
        if (pb) { sum += pb->val; pb = pb->nx; }
        cr = sum / BASE;
        bn_push(res, (int)(sum % BASE));
    }
    if (!res->h) bn_push(res, 0);
    return res;
}

static void bn_print(bn* b)
{
    if (!b || !b->t) { cout << "0\n"; return; }

    cout << b->t->val;
    lnd* cur = b->t->pr;
    while (cur)
    {
        cout << setw(9) << setfill('0') << cur->val;
        cur = cur->pr;
    }
    cout << '\n';
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int k, n;
    if (!(cin >> k >> n)) return 0;

    if (n == 0)
    {
        cout << "0\n";
        return 0;
    }

    vector<bn*> dp(n + 1, NULL);
    dp[0] = bn_init(1);

    rep(i, 1, n + 1)
    {
        dp[i] = bn_init(0);
        rep(j, 1, k + 1)
            if (i - j >= 0)
            {
                bn* tmp = bn_add(dp[i], dp[i - j]);
                bn_free(dp[i]);
                dp[i] = tmp;
            }
    }

    bn_print(dp[n]);

    rep(i, 0, n + 1) bn_free(dp[i]);
    return 0;
}
