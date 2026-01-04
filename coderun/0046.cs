// https://coderun.yandex.ru/problem/robots
// parity + minimax

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;

class Program
{
    const int INF = 1000000000;

    static void Main()
    {
        FS fs = new FS();

        int n = fs.RI();
        int k = fs.RI();

        List<int>[] g = new List<int>[n + 1];
        for (int i = 0; i <= n; ++i) g[i] = new List<int>();

        int[] eu = new int[k];
        int[] ev = new int[k];

        for (int i = 0; i < k; ++i)
        {
            int u = fs.RI();
            int v = fs.RI();
            g[u].Add(v);
            g[v].Add(u);
            eu[i] = u;
            ev[i] = v;
        }

        int m = fs.RI();
        int[] a = new int[m];
        for (int i = 0; i < m; ++i) a[i] = fs.RI();

        int[,,] dist = new int[m, n + 1, 2];
        for (int r = 0; r < m; ++r)
            for (int v = 0; v <= n; ++v)
            {
                dist[r, v, 0] = INF;
                dist[r, v, 1] = INF;
            }

        int[] qv = new int[n * 2 + 5];
        int[] qd = new int[n * 2 + 5];

        for (int r = 0; r < m; ++r)
        {
            int s = a[r];
            int qh = 0, qt = 0;

            dist[r, s, 0] = 0;
            qv[qt] = s;
            qd[qt] = 0;
            ++qt;

            while (qh < qt)
            {
                int u = qv[qh];
                int d = qd[qh];
                ++qh;

                int nd = d + 1;
                int p = nd & 1;

                var adj = g[u];
                for (int i = 0; i < adj.Count; ++i)
                {
                    int v = adj[i];
                    if (dist[r, v, p] == INF)
                    {
                        dist[r, v, p] = nd;
                        qv[qt] = v;
                        qd[qt] = nd;
                        ++qt;
                    }
                }
            }
        }

        double ans = double.MaxValue;

        for (int v = 1; v <= n; ++v)
        {
            int mx = 0;
            bool ok = true;

            for (int r = 0; r < m; ++r)
            {
                int d = dist[r, v, 0];
                if (d == INF) { ok = false; break; }
                if (d > mx) mx = d;
            }
            if (ok && mx < ans) ans = mx;

            mx = 0;
            ok = true;

            for (int r = 0; r < m; ++r)
            {
                int d = dist[r, v, 1];
                if (d == INF) { ok = false; break; }
                if (d > mx) mx = d;
            }
            if (ok && mx < ans) ans = mx;
        }

        int[,] best = new int[m, n + 1];
        for (int r = 0; r < m; ++r)
            for (int v = 1; v <= n; ++v)
            {
                int d0 = dist[r, v, 0];
                int d1 = dist[r, v, 1];
                best[r, v] = d0 < d1 ? d0 : d1;
            }

        for (int i = 0; i < k; ++i)
        {
            int u = eu[i];
            int v = ev[i];

            int mx = 0;
            bool ok = true;

            for (int r = 0; r < m; ++r)
            {
                int du = best[r, u];
                int dv = best[r, v];
                int cur = du < dv ? du : dv;

                if (cur == INF) { ok = false; break; }
                if (cur > mx) mx = cur;

                if (mx + 0.5 >= ans) { ok = false; break; }
            }

            if (ok)
            {
                double t = mx + 0.5;
                if (t < ans) ans = t;
            }
        }

        if (ans == double.MaxValue) Console.WriteLine("-1");
        else Console.WriteLine(ans.ToString("0.#", CultureInfo.InvariantCulture));
    }
}

class FS
{
    readonly Stream s = Console.OpenStandardInput();
    readonly byte[] b = new byte[1 << 16];
    int i, n;

    int GC()
    {
        if (i >= n)
        {
            n = s.Read(b, 0, b.Length);
            i = 0;
            if (n <= 0) return -1;
        }
        return b[i++];
    }

    public int RI()
    {
        int c = GC();
        while (c <= 32 && c != -1) c = GC();
        int sign = 1;
        if (c == '-') { sign = -1; c = GC(); }
        int x = 0;
        while (c > 32 && c != -1)
        {
            x = x * 10 + (c - '0');
            c = GC();
        }
        return x * sign;
    }
}
