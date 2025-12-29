// https://acmp.ru/index.asp?main=task&id_task=652
// range queries + sparse representation + interval splitting

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Node { ll len, val; int d; };

vector<Node> v;

int split(ll pos) {
    ll cur = 1;
    for (int i = 0; i < v.size(); ++i)
    {
        if (cur == pos) return i;
        if (cur + v[i].len > pos)
        {
            ll l_part = pos - cur;
            ll r_part = v[i].len - l_part;

            Node next = {r_part, v[i].val + l_part * v[i].d, v[i].d};
            v[i].len = l_part;

            v.insert(v.begin() + i + 1, next);
            return i + 1;
        }
        cur += v[i].len;
    }
    return v.size();
}

int main()
{
    ios_base::sync_with_stdio(0); cin.tie(0);

    ll n; int k;
    if (!(cin >> n >> k)) return 0;

    v.push_back({n, 1, 1});

    while (k--)
    {
        char op; ll l, r;
        cin >> op >> l >> r;

        int p1 = split(l);
        int p2 = split(r + 1);

        if (op == 'I')
        {
            reverse(v.begin() + p1, v.begin() + p2);
            for (int i = p1; i < p2; ++i)
            {
                v[i].val += (v[i].len - 1) * v[i].d;
                v[i].d = -v[i].d;
            }
        }
        else
        {
            ll sum = 0;
            for (int i = p1; i < p2; ++i)
                sum += v[i].len * (2 * v[i].val + (v[i].len - 1) * v[i].d) / 2;
            cout << sum << "\n";
        }
    }
    return 0;
}
