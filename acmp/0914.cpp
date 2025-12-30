#include <iostream>
#include <cmath>

using namespace std;

typedef long long ll;

struct vec { ll x, y, z; };


vec add(vec a, vec b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }

vec sub(vec a, vec b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }

ll dot(vec a, vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec cross(vec a, vec b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }


vec p1, u[3], p2, v[3];

bool check(vec axis, vec r)
{
    if (axis.x == 0 && axis.y == 0 && axis.z == 0) return false;
    ll sum = 0;
    for (int i = 0; i < 3; ++i) sum += abs(dot(u[i], axis));
    for (int i = 0; i < 3; ++i) sum += abs(dot(v[i], axis));
    return abs(dot(r, axis)) > sum;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    cin >> p1.x >> p1.y >> p1.z;
    for (int i = 0; i < 3; ++i) cin >> u[i].x >> u[i].y >> u[i].z;

    cin >> p2.x >> p2.y >> p2.z;
    for (int i = 0; i < 3; ++i) cin >> v[i].x >> v[i].y >> v[i].z;

    vec sum_u = add(u[0], add(u[1], u[2]));
    vec sum_v = add(v[0], add(v[1], v[2]));

    vec c1 = add({2 * p1.x, 2 * p1.y, 2 * p1.z}, sum_u);
    vec c2 = add({2 * p2.x, 2 * p2.y, 2 * p2.z}, sum_v);

    vec r = sub(c2, c1);

    for (int i = 0; i < 3; ++i) if (check(u[i], r)) { cout << "NO"; return 0; }

    for (int i = 0; i < 3; ++i) if (check(v[i], r)) { cout << "NO"; return 0; }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (check(cross(u[i], v[j]), r)) { cout << "NO"; return 0; }

    cout << "YES";
    return 0;
}
