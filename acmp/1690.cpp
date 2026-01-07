// https://acmp.ru/asp/do/index.asp?main=task&id_course=2&id_section=20&id_topic=44&id_problem=1739
// 3D prefix sums + inclusion-exclusion

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define rep(i,a,b) for (int i = (a); i <= (b); ++i)

static int ps[105][105][105];

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int H, N, M, Q;
    cin >> H >> N >> M >> Q;

    rep(x, 1, H)
    {
        rep(y, 1, N)
        {
            string s;
            cin >> s;
            rep(z, 1, M)
            {
                int a = s[z - 1] - '0';
                ps[x][y][z] = a
                    + ps[x - 1][y][z] + ps[x][y - 1][z] + ps[x][y][z - 1]
                    - ps[x - 1][y - 1][z] - ps[x - 1][y][z - 1] - ps[x][y - 1][z - 1]
                    + ps[x - 1][y - 1][z - 1];
            }
        }
    }

    auto P = [&](int x, int y, int z) -> ll
    { return (x < 0 || y < 0 || z < 0) ? 0LL : (ll)ps[x][y][z]; };

    rep(_, 1, Q)
    {
        int x1, y1, z1, x2, y2, z2;
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (z1 > z2) swap(z1, z2);

        int a = x1 - 1, b = y1 - 1, c = z1 - 1;

        ll ans =
            P(x2, y2, z2)
            - P(a, y2, z2) - P(x2, b, z2) - P(x2, y2, c)
            + P(a, b, z2) + P(a, y2, c) + P(x2, b, c)
            - P(a, b, c);

        cout << ans << '\n';
    }

    return 0;
}
