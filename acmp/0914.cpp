// https://acmp.ru/index.asp?main=task&id_task=914
// computational geometry

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

#define rep(i,a,b) for (int i = (a); i < (b); ++i)

struct vll3 { ll x, y, z; };

vll3 add(vll3 a, vll3 b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
vll3 sub(vll3 a, vll3 b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
ll dot(vll3 a, vll3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vll3 cross(vll3 a, vll3 b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

vll3 p1, u[3], p2, v[3];

bool check(vll3 axis, vll3 r)
{
    if (axis.x == 0 && axis.y == 0 && axis.z == 0) return false;

    ll sum = 0;
    rep(i, 0, 3) sum += abs(dot(u[i], axis));
    rep(i, 0, 3) sum += abs(dot(v[i], axis));

    return abs(dot(r, axis)) > sum;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> p1.x >> p1.y >> p1.z;
    rep(i, 0, 3) cin >> u[i].x >> u[i].y >> u[i].z;

    cin >> p2.x >> p2.y >> p2.z;
    rep(i, 0, 3) cin >> v[i].x >> v[i].y >> v[i].z;

    vll3 sum_u = add(u[0], add(u[1], u[2]));
    vll3 sum_v = add(v[0], add(v[1], v[2]));

    vll3 c1 = add({2 * p1.x, 2 * p1.y, 2 * p1.z}, sum_u);
    vll3 c2 = add({2 * p2.x, 2 * p2.y, 2 * p2.z}, sum_v);

    vll3 r = sub(c2, c1);

    rep(i, 0, 3) if (check(u[i], r)) { cout << "NO\n"; return 0; }
    rep(i, 0, 3) if (check(v[i], r)) { cout << "NO\n"; return 0; }

    rep(i, 0, 3)
        rep(j, 0, 3)
            if (check(cross(u[i], v[j]), r)) { cout << "NO\n"; return 0; }

    cout << "YES\n";
    return 0;
}
