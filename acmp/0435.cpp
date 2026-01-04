// https://acmp.ru/index.asp?main=task&id_task=435
// reachable-state enumeration + interning (dedup) + сhinese remainder theorem + subset sum + NFA -> DFA with subset construction + Miller–Rabin

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef unsigned __int128 u128;

#define sz(x) (int)(x).size()

const int INF = 1e9;
const ll LINF = (ll)4e18;

static inline ull mod_mul(ull a, ull b, ull mod) { return (ull)((u128)a * (u128)b % (u128)mod); }

static inline ull mod_pow(ull a, ull e, ull mod)
{
    ull r = 1;
    while (e)
    {
        if (e & 1ULL) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1ULL;
    }

    return r;
}

static inline bool is_prime_u64(ull n)
{
    if (n < 2) return false;
    static const ull small_primes[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL};
    for (ull p : small_primes)
    {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    ull d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1ULL; ++s; }

    static const ull bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (ull a : bases)
    {
        ull x = a % n;
        if (x == 0) continue;
        x = mod_pow(x, d, n);
        if (x == 1 || x == n - 1) continue;

        bool comp = true;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }

        if (comp) return false;
    }
    return true;
}

static inline ull prev_prime(ull start_odd)
{
    ull x = (start_odd | 1ULL);
    while (!is_prime_u64(x)) x -= 2ULL;
    return x;
}

struct big_dec
{
    static const uint32_t BASE = 1000000000u;
    vector<uint32_t> d;

    big_dec(ull x = 0)
    {
        if (x == 0) d.push_back(0);
        else
        {
            while (x)
            {
                d.push_back((uint32_t)(x % BASE));
                x /= BASE;
            }
        }
    }

    inline void norm() { while (d.size() > 1 && d.back() == 0) d.pop_back(); }

    ull mod_u64(ull m) const
    {
        u128 rem = 0;
        for (int i = (int)d.size() - 1; i >= 0; --i) { rem = (rem * (u128)BASE + (u128)d[i]) % (u128)m; }
        return (ull)rem;
    }

    void mul_u64(ull m)
    {
        if (m == 0) { d.assign(1, 0); return; }
        u128 carry = 0;
        for (size_t i = 0; i < d.size(); ++i)
        {
            u128 cur = carry + (u128)d[i] * (u128)m;
            d[i] = (uint32_t)(cur % (u128)BASE);
            carry = cur / (u128)BASE;
        }
        while (carry)
        {
            d.push_back((uint32_t)(carry % (u128)BASE));
            carry /= (u128)BASE;
        }
        norm();
    }

    void add_big(big_dec const &b)
    {
        size_t n = max(d.size(), b.d.size());
        d.resize(n, 0);
        uint32_t carry = 0;
        for (size_t i = 0; i < n; ++i)
        {
            uint64_t cur = (uint64_t)d[i] + (uint64_t)carry + (i < b.d.size() ? (uint64_t)b.d[i] : 0ULL);
            if (cur >= BASE)
            {
                d[i] = (uint32_t)(cur - BASE);
                carry = 1;
            }
            else
            {
                d[i] = (uint32_t)cur;
                carry = 0;
            }
        }
        if (carry) d.push_back(carry);
        norm();
    }

    void add_mul(big_dec const &m, ull k)
    {
        if (k == 0) return;
        big_dec tmp = m;
        tmp.mul_u64(k);
        add_big(tmp);
    }

    void print() const
    {
        printf("%u", d.back());
        for (int i = (int)d.size() - 2; i >= 0; --i) printf("%09u", d[i]);
        putchar('\n');
    }
};

static inline void store24(uint8_t *p, uint32_t v)
{
    p[0] = (uint8_t)(v & 255u);
    p[1] = (uint8_t)((v >> 8) & 255u);
    p[2] = (uint8_t)((v >> 16) & 255u);
}

static inline uint32_t load24(const uint8_t *p) { return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16); }

struct key_pool
{
    int words = 0;
    static const int BLOCK_ST = 4096;

    vector<ull*> blk;
    uint32_t states = 0;

    void init(int _words)
    {
        words = _words;
        blk.clear();
        states = 0;
    }

    void destroy()
    {
        for (ull *p : blk) free(p);
        blk.clear();
        states = 0;
        words = 0;
    }

    inline ull* ptr(uint32_t id)
    {
        uint32_t b = id / (uint32_t)BLOCK_ST;
        uint32_t o = id % (uint32_t)BLOCK_ST;
        return blk[b] + (size_t)o * (size_t)words;
    }

    inline const ull* ptr(uint32_t id) const
    {
        uint32_t b = id / (uint32_t)BLOCK_ST;
        uint32_t o = id % (uint32_t)BLOCK_ST;
        return blk[b] + (size_t)o * (size_t)words;
    }

    uint32_t add(const ull *k)
    {
        if ((states % (uint32_t)BLOCK_ST) == 0)
        {
            ull *nb = (ull*)malloc(sizeof(ull) * (size_t)BLOCK_ST * (size_t)words);
            blk.push_back(nb);
        }
        ull *dst = ptr(states);
        for (int i = 0; i < words; ++i) dst[i] = k[i];
        return states++;
    }
};

static inline ull splitmix64(ull x) noexcept
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

struct Intern
{
    int words = 0;
    ull last_mask = 0;

    int cap = 0;
    int sz = 0;

    uint32_t *h1 = nullptr;
    uint32_t *sid = nullptr;

    key_pool pool;

    void init(int _words, ull _last_mask, int init_cap)
    {
        words = _words;
        last_mask = _last_mask;

        cap = max(8, init_cap);
        sz = 0;
        h1 = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)cap);
        sid = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)cap);
        memset(h1, 0, sizeof(uint32_t) * (size_t)cap);

        pool.init(words);
    }

    void destroy()
    {
        free(h1);
        free(sid);
        h1 = nullptr;
        sid = nullptr;
        cap = 0;
        sz = 0;
        words = 0;
        last_mask = 0;
        pool.destroy();
    }

    inline ull hash64(const ull *k) const
    {
        ull h = 0x243f6a8885a308d3ULL;
        for (int i = 0; i < words; ++i)
        {
            h = splitmix64(h ^ splitmix64(k[i] + 0x9e3779b97f4a7c15ULL * (ull)(i + 1)));
        }
        return h;
    }

    static inline uint32_t hash32_from64(ull h)
    {
        uint32_t x = (uint32_t)(h ^ (h >> 32));
        if (x == 0xFFFFFFFFu) x = 0xFFFFFFFEu;
        return x;
    }

    inline bool key_eq(const ull *a, const ull *b) const
    {
        for (int i = 0; i < words; ++i) if (a[i] != b[i]) return false;
        return true;
    }

    void rehash(int new_cap)
    {
        uint32_t *nh1 = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)new_cap);
        uint32_t *nsid = (uint32_t*)malloc(sizeof(uint32_t) * (size_t)new_cap);
        memset(nh1, 0, sizeof(uint32_t) * (size_t)new_cap);

        int old_cap = cap;
        uint32_t *oh1 = h1;
        uint32_t *osid = sid;

        h1 = nh1;
        sid = nsid;
        cap = new_cap;
        sz = 0;

        for (int i = 0; i < old_cap; ++i)
        {
            uint32_t hv = oh1[i];
            if (!hv) continue;
            uint32_t idv = osid[i];

            uint32_t h32 = hv - 1u;
            int pos = (int)(h32 % (uint32_t)cap);
            while (h1[pos]) { if (++pos == cap) pos = 0; }
            h1[pos] = hv;
            sid[pos] = idv;
            ++sz;
        }

        free(oh1);
        free(osid);
    }

    uint32_t get_or_add(const ull *k, ull h64, bool &is_new)
    {
        if ((sz + 1) * 10 >= cap * 7) { rehash(cap * 2 + 1); }

        uint32_t h32 = hash32_from64(h64);
        uint32_t hv = h32 + 1u;

        int pos = (int)(h32 % (uint32_t)cap);
        while (h1[pos])
        {
            if (h1[pos] == hv)
            {
                uint32_t idv = sid[pos];
                if (key_eq(pool.ptr(idv), k))
                {
                    is_new = false;
                    return idv;
                }
            }

            if (++pos == cap) pos = 0;
        }

        uint32_t idv = pool.add(k);
        h1[pos] = hv;
        sid[pos] = idv;
        ++sz;

        is_new = true;
        return idv;
    }

    uint32_t find_id(const ull *k, ull h64) const
    {
        uint32_t h32 = hash32_from64(h64);
        uint32_t hv = h32 + 1u;

        int pos = (int)(h32 % (uint32_t)cap);
        while (h1[pos])
        {
            if (h1[pos] == hv)
            {
                uint32_t idv = sid[pos];
                if (key_eq(pool.ptr(idv), k)) return idv;
            }

            if (++pos == cap) pos = 0;
        }
        return UINT32_MAX;
    }
};

static inline void apply_step(ull *out, const ull *in, int words, ull last_mask, int d, const uint16_t tbl[10][512])
{
    ull carry = 0;
    for (int i = 0; i < words; ++i)
    {
        ull nw = (in[i] << d) | carry;
        carry = in[i] >> (64 - d);
        out[i] = nw;
    }

    carry = 0;
    for (int i = words - 1; i >= 0; --i)
    {
        ull nw = (in[i] >> d) | carry;
        carry = in[i] << (64 - d);
        out[i] |= nw;
    }

    ull low = in[0] & ((1ULL << d) - 1ULL);
    out[0] |= (ull)tbl[d][(unsigned)low];

    out[words - 1] &= last_mask;
}

static ull run_dp_mod(uint32_t states, int n, int k, const uint8_t *tr, const vector<uint8_t> &has0, ull mod)
{
    vector<ull> dp(states, 0), ndp(states, 0);
    vector<uint32_t> active, nxt;
    vector<uint8_t> seen(states, 0);

    active.reserve(states);
    nxt.reserve(states);

    dp[0] = 1 % mod;
    active.push_back(0);

    for (int step = 0; step < n; ++step)
    {
        memset(ndp.data(), 0, (size_t)states * sizeof(ull));
        nxt.clear();

        for (uint32_t id : active)
        {
            ull v = dp[id];
            if (!v) continue;

            {
                ull s = ndp[id] + v;
                if (s >= mod) s -= mod;
                if (!ndp[id] && s) { if (!seen[id]) { seen[id] = 1; nxt.push_back(id); } }
                ndp[id] = s;
            }

            const uint8_t *base = tr + (size_t)id * (size_t)k * 3u;
            for (int d = 1; d <= k; ++d)
            {
                uint32_t nid = load24(base + (size_t)(d - 1) * 3u);
                ull s = ndp[nid] + v;
                if (s >= mod) s -= mod;
                if (!ndp[nid] && s) { if (!seen[nid]) { seen[nid] = 1; nxt.push_back(nid); } }
                ndp[nid] = s;
            }
        }

        for (uint32_t id : nxt) seen[id] = 0;

        dp.swap(ndp);
        active.swap(nxt);
    }

    ull happy = 0;
    for (uint32_t id : active)
    {
        if (!has0[id]) continue;
        happy += dp[id];
        if (happy >= mod) happy -= mod;
    }

    ull total = mod_pow((ull)(k + 1), (ull)n, mod);
    return (total + mod - happy) % mod;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    cin >> n >> k;

    static uint16_t tbl[10][512];
    for (int d = 0; d <= 9; ++d)
    {
        int lim = 1 << d;
        for (int mask = 0; mask < lim; ++mask)
        {
            uint16_t out = 0;
            for (int a = 0; a < d; ++a) if (mask & (1 << a)) out |= (uint16_t)(1u << (d - a));


            tbl[d][mask] = out;
        }
    }

    int s = n * k;
    int bits = s + 1;
    int words = (bits + 63) / 64;
    int last_bits = bits - 64 * (words - 1);
    ull last_mask = (last_bits == 64) ? ~0ULL : ((1ULL << last_bits) - 1ULL);

    int init_cap = max(8192, (s * s + 64) * 2);
    Intern inter;
    inter.init(words, last_mask, init_cap);

    vector<uint8_t> has0;
    vector<uint8_t> depth;
    vector<uint32_t> q;

    has0.reserve(100000);
    depth.reserve(100000);
    q.reserve(100000);

    ull start_key[8];
    for (int i = 0; i < words; ++i) start_key[i] = 0;
    start_key[0] = 1ULL;
    start_key[words - 1] &= last_mask;

    bool added = false;
    uint32_t id0 = inter.get_or_add(start_key, inter.hash64(start_key), added);
    (void)id0;

    has0.push_back(1);
    depth.push_back(0);
    q.push_back(0);

    ull ns_key[8];

    for (size_t qi = 0; qi < q.size(); ++qi)
    {
        uint32_t id = q[qi];
        uint8_t dep = depth[id];
        if (dep >= (uint8_t)n) continue;

        const ull *st = inter.pool.ptr(id);
        for (int d = 1; d <= k; ++d)
        {
            apply_step(ns_key, st, words, last_mask, d, tbl);
            ull h64 = inter.hash64(ns_key);

            bool is_new = false;
            uint32_t nid = inter.get_or_add(ns_key, h64, is_new);

            if (is_new)
            {
                has0.push_back((ns_key[0] & 1ULL) ? 1 : 0);
                depth.push_back((uint8_t)(dep + 1));
                q.push_back(nid);
            }
        }
    }

    uint32_t states = inter.pool.states;

    size_t tr_bytes = (size_t)states * (size_t)k * 3u;
    uint8_t *tr = (uint8_t*)malloc(tr_bytes);
    memset(tr, 0, tr_bytes);

    for (uint32_t id = 0; id < states; ++id)
    {
        if (depth[id] >= (uint8_t)n) continue;

        const ull *st = inter.pool.ptr(id);
        uint8_t *base = tr + (size_t)id * (size_t)k * 3u;

        for (int d = 1; d <= k; ++d)
        {
            apply_step(ns_key, st, words, last_mask, d, tbl);
            ull h64 = inter.hash64(ns_key);
            uint32_t nid = inter.find_id(ns_key, h64);
            store24(base + (size_t)(d - 1) * 3u, nid);
        }
    }

    inter.destroy();
    vector<uint32_t>().swap(q);
    vector<uint8_t>().swap(depth);

    long double need_bits = (long double)n * log2l((long double)(k + 1)) + 2.0L;
    long double have_bits = 0.0L;

    vector<ull> p;
    ull cur = (1ULL << 63) - 25ULL;
    while (have_bits < need_bits)
    {
        ull pr = prev_prime(cur);
        p.push_back(pr);
        have_bits += log2l((long double)pr);
        cur = pr - 2ULL;
    }

    vector<ull> a;
    a.reserve(p.size());
    for (ull pr : p)
    {
        a.push_back(run_dp_mod(states, n, k, tr, has0, pr));
    }

    big_dec X(0);
    big_dec M(1);

    for (size_t i = 0; i < p.size(); ++i)
    {
        ull pr = p[i];
        ull r = a[i];

        ull x_mod = X.mod_u64(pr);
        ull m_mod = M.mod_u64(pr);

        ull t = (r + pr - x_mod) % pr;
        ull inv = mod_pow(m_mod, pr - 2, pr);
        ull kk = mod_mul(t, inv, pr);

        X.add_mul(M, kk);
        M.mul_u64(pr);
    }

    X.print();
    free(tr);
    return 0;
}
