// https://acm.timus.ru/problem.aspx?space=1&num=1624
// constraint propagation + hypothesis tracking + bitsets + grid graph + consistency/uniqueness checks

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)((x).size())
#define rep(i,a,b) for (int i=(a); i<(b); ++i)
#define res reset
#define pb push_back
#define ppb pop_back
#define fi first
#define se second

const int MAX_CELLS = 150;
const int MAX_EDGES = 300;

typedef bitset<MAX_CELLS> msk;

int w, h, k;
int n, eh;

struct sc_state
{
    int pos[MAX_CELLS];
    msk act;
    msk wall[MAX_EDGES];
    msk open[MAX_EDGES];

    void init(int _n)
    {
        act.res();
        rep(i,0,_n)
        {
            pos[i] = i;
            act.set(i);
        }
        rep(e,0,MAX_EDGES)
        {
            wall[e].res();
            open[e].res();
        }
    }
};

static sc_state s1, s2;
static msk mate[MAX_CELLS];

static inline int ge(int r, int c, char dir)
{
    if (dir == 'N') return (r - 1) * w + c;
    if (dir == 'S') return r * w + c;
    if (dir == 'W') return eh + r * (w - 1) + (c - 1);
    if (dir == 'E') return eh + r * (w - 1) + c;
    return -1;
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    if (!(cin >> w >> h >> k)) return 0;

    n = w * h;
    eh = (h - 1) * w;

    s1.init(n);
    s2.init(n);

    rep(i,0,n) mate[i].set();

    if (n < MAX_CELLS)
    {
        msk keep;
        rep(j,0,n) keep.set(j);
        rep(i,0,n) mate[i] &= keep;
    }

    int sol = -1;

    vi touch;
    touch.reserve(n);

    msk nw[MAX_EDGES];
    msk no[MAX_EDGES];

    rep(step,1,k + 1)
    {
        char dir, res;
        cin >> dir >> res;

        bool s1_turn = (step & 1);

        sc_state *cur = s1_turn ? &s1 : &s2;
        sc_state *opp = s1_turn ? &s2 : &s1;

        msk died;
        died.res();

        if (!s1_turn) touch.clear();

        rep(i,0,n)
        {
            if (!cur->act[i]) continue;

            int p = cur->pos[i];
            int r = p / w;
            int c = p % w;

            int nr = r, nc = c;
            if (dir == 'N') nr--;
            else if (dir == 'S') nr++;
            else if (dir == 'E') nc++;
            else if (dir == 'W') nc--;

            bool oob = (nr < 0 || nr >= h || nc < 0 || nc >= w);

            if (oob)
            {
                if (res == '+')
                { cur->act[i] = 0; died.set(i); } continue;
            }

            int e = ge(r, c, dir);
            int tgt = nr * w + nc;

            if (res == '+')
            {
                if (cur->wall[e][i])
                {
                    cur->act[i] = 0;
                    died.set(i);
                }
                else
                {
                    cur->open[e].set(i);
                    cur->pos[i] = tgt;

                    if (s1_turn) mate[i] &= ~opp->wall[e];
                    else
                    {
                        if (!no[e].any() && !nw[e].any()) touch.pb(e);
                        no[e].set(i);
                    }
                }
            }
            else
            {
                if (cur->open[e][i])
                {
                    cur->act[i] = 0;
                    died.set(i);
                }
                else
                {
                    cur->wall[e].set(i);

                    if (s1_turn) mate[i] &= ~opp->open[e];
                    else
                    {
                        if (!no[e].any() && !nw[e].any()) touch.pb(e);
                        nw[e].set(i);
                    }
                }
            }
        }

        if (!s1_turn)
        {
            if (died.any())
            {
                msk alive = ~died;
                rep(i,0,n) if (s1.act[i]) mate[i] &= alive;
            }

            for (int e : touch)
            {
                if (no[e].any())
                {
                    msk bad1 = s1.wall[e];
                    msk bad2 = no[e];
                    rep(i,0,n) if (s1.act[i] && bad1[i]) mate[i] &= ~bad2;
                    no[e].res();
                }

                if (nw[e].any())
                {
                    msk bad1 = s1.open[e];
                    msk bad2 = nw[e];
                    rep(i,0,n) if (s1.act[i] && bad1[i]) mate[i] &= ~bad2;
                    nw[e].res();
                }
            }
        }

        bool psb = 0;
        bool uniq = 1;

        if (sol == -1)
        {
            int p1 = -1;
            int p2 = -1;
            msk bad;

            rep(i,0,n)
            {
                if (!s1.act[i]) continue;
                if (!mate[i].any()) continue;

                int x = s1.pos[i];

                if (!psb)
                {
                    psb = 1;
                    p1 = x;

                    rep(j,0,n)
                    {
                        if (mate[i][j])
                        {
                            p2 = s2.pos[j];
                            break;
                        }
                    }

                    bad.res();
                    rep(j,0,n) if (s2.pos[j] != p2) bad.set(j);
                }

                if (x != p1) { uniq = 0; break; }
                if ((mate[i] & bad).any()) { uniq = 0; break; }
            }
        }
        else
            rep(i,0,n)
                if (s1.act[i] && mate[i].any())
                { psb = 1; break; }

        if (!psb)
        {
            cout << "A mistake has been made at step number " << step << '\n';
            return 0;
        }

        if (uniq && sol == -1) sol = step;
    }

    if (sol != -1) cout << "The scouts are safe at step number " << sol << '\n';
    else cout << "There is not enough data" << '\n';


    return 0;
}
