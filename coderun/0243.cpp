// https://coderun.yandex.ru/problem/svd-recommender
// ML / recommender system (rating prediction) + matrix factorization + SGD training + regularization + RMSE metric

#include <bits/stdc++.h>
using namespace std;;

typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<double> vd;

#define rep(i,a,b) for(int i=(a); i<(b); i++)
#define u_r_d uniform_real_distribution

static const int ISZ = 1 << 20;
static const int OSZ = 1 << 20;

static int ip = 0, il = 0;
static char ib[ISZ];

static int op = 0;
static char ob[OSZ];

static inline char gc()
{
    if(ip >= il)
    {
        il = (int)fread(ib, 1, ISZ, stdin);
        ip = 0;
        if(il == 0) return 0;
    }

    return ib[ip++];
}

template <class t>
static inline bool ri(t &x)
{
    char c;
    do { c = gc(); if(!c) return false; } while(c <= ' ');

    t s = 1;
    if(c == '-')
    { s = -1; c = gc(); }

    t v = 0;
    while(c > ' ')
    { v = v * 10 + (c - '0'); c = gc(); }
    x = v * s;
    return true;
}

static inline void fo() { if(op) { fwrite(ob, 1, op, stdout); op = 0; } }

static inline void pc(char c) { if(op >= OSZ) fo(); ob[op++] = c;}

static inline void wd(double x)
{
    char tmp[64];
    int n = snprintf(tmp, sizeof(tmp), "%.6f\n", x);
    rep(i,0,n) pc(tmp[i]);
}

static inline float clampf(float x, float lo, float hi)
{
    if(x < lo) return lo;
    if(x > hi) return hi;
    return x;
}

int main()
{
    int k, u_n, m_n, d_n, t_n;
    if(!ri(k)) return 0;
    ri(u_n);
    ri(m_n);
    ri(d_n);
    ri(t_n);

    vi iu(d_n), im(d_n);
    vf ir(d_n);

    double mu = 0.0;

    vi cu(u_n, 0), cm(m_n, 0);

    rep(i,0,d_n)
    {
        int u, m, r;
        ri(u);
        ri(m);
        ri(r);
        iu[i] = u;
        im[i] = m;
        ir[i] = (float)r;
        mu += (double)r;
        cu[u]++;
        cm[m]++;

    }

    mu /= max(1, d_n);

    vf bu(u_n, 0.0f), bm(m_n, 0.0f);

    const float reg0 = 10.0f;

    rep(it,0,2)
    {
        vd su(u_n, 0.0), sm(m_n, 0.0);

        rep(i,0,d_n)
        {
            int u = iu[i];
            int m = im[i];
            double r = ir[i];
            su[u] += (r - mu - bm[m]);
        }

        rep(u,0,u_n) if(cu[u]) bu[u] = (float)(su[u] / (cu[u] + reg0));


        rep(i,0,d_n)
        {
            int u = iu[i];
            int m = im[i];
            double r = ir[i];
            sm[m] += (r - mu - bu[u]);
        }

        rep(m,0,m_n) if(cm[m]) bm[m] = (float)(sm[m] / (cm[m] + reg0));
    }

    const int d = 10;

    vf pu((size_t)u_n * d), qm((size_t)m_n * d);

    mt19937 rng(1234567);
    u_r_d<float> ur(-0.01f, 0.01f);

    rep(i,0,(int)pu.size()) pu[i] = ur(rng);
    rep(i,0,(int)qm.size()) qm[i] = ur(rng);

    int ep_n = 15;
    if(d_n < 200000) ep_n = 25;
    else if(d_n < 500000) ep_n = 20;

    float lr0 = 0.1f / max(1, k);
    float dec = 0.92f;

    float reg = 0.02f;
    float regb = 0.01f;

    rep(ep,0,ep_n)
    {
        float lr = lr0 * powf(dec, (float)ep);

        rep(i,0,d_n)
        {
            int u = iu[i];
            int m = im[i];
            float r = ir[i];

            float *p = &pu[(size_t)u * d];
            float *q = &qm[(size_t)m * d];

            float dot = 0.0f;
            rep(f,0,d) dot += p[f] * q[f];

            float pr = (float)mu + bu[u] + bm[m] + dot;
            float e = r - pr;

            bu[u] += lr * (e - regb * bu[u]);
            bm[m] += lr * (e - regb * bm[m]);

            rep(f,0,d)
            {
                float pv = p[f];
                float qv = q[f];
                p[f] = pv + lr * (e * qv - reg * pv);
                q[f] = qv + lr * (e * pv - reg * qv);
            }
        }
    }

    float lo = 1.0f, hi = (float)k;

    rep(i,0,t_n)
    {
        int u, m;
        ri(u);
        ri(m);

        float *p = &pu[(size_t)u * d];
        float *q = &qm[(size_t)m * d];

        float dot = 0.0f;
        rep(f,0,d) dot += p[f] * q[f];

        float pr = (float)mu + bu[u] + bm[m] + dot;
        pr = clampf(pr, lo, hi);

        wd((double)pr);
    }

    fo();
    return 0;
}
