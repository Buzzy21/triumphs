// problem: https://usaco.org/index.php?page=viewproblem2&cpid=231
// diabolical implementation

#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Segtree {
    int n;
    vector<int> pref,mx,suff;
    vector<int> pending,lens;

    void debug() {
        cout << "Tree: "; 
        for(int i = 1; i < pref.size(); i++) {
            cout << "(";
            for(int j = 0; j < 3; j++) {
                cout << vector<vector<int>>{pref,mx,suff}[i][j];
                if(j!=2) cout << ",";
            }
            cout << ") ";
        }
        cout << endl;
        cout << "Pending: ";
        for(int i = 1; i < pending.size(); i++) {
            cout << pending[i] << " ";
        }
        cout << endl << endl;
    }

    void setup(int u, int tl, int tr) {
        if(tl==tr) lens[u] = 1;
        else {
            int tm = tl+(tr-tl)/2;
            setup(u*2,tl,tm); setup(u*2+1,tm+1,tr);
            lens[u] = lens[u*2]+lens[u*2+1];
        }
        pref[u] = mx[u] = suff[u] = lens[u];
    }

    Segtree(int n) {
        this->n = n;
        pref.resize(n*4,0); mx = pref; suff = pref;
        pending.resize(n*4,-1); lens.resize(n*4,0); 
        setup(1,0,n-1);
    }

    void push(int u, int tl, int tr) {
        if(pending[u]==-1) return;

        if(pending[u] == 0) {
            pref[u] = suff[u] = mx[u] = lens[u];
        }
        else pref[u] = suff[u] = mx[u] = 0;

        if(tl!=tr) {
            pending[u*2] = pending[u];
            pending[u*2+1] = pending[u];
        }
        pending[u] = -1;
    }

    void update(int u, int tl, int tr, int l, int r, int val) {
        if(tl==l&&tr==r) pending[u] = val;
        push(u,tl,tr);

        if(l>r || (tl==l&&tr==r)) return;

        int tm = tl+(tr-tl)/2;
        update(u*2,tl,tm,l,min(r,tm),val);
        update(u*2+1,tm+1,tr,max(l,tm+1),r,val);

        pref[u] = pref[u*2];
        suff[u] = suff[u*2+1];
        if(mx[u*2] == lens[u*2]) pref[u] += pref[u*2+1];
        if(mx[u*2+1] == lens[u*2+1]) suff[u] += suff[u*2];

        mx[u] = max({mx[u*2],mx[u*2+1],pref[u],suff[u],suff[u*2]+pref[u*2+1]});
    }

    void update(int l, int r, int val) {
        update(1,0,n-1,l,r,val);
    }

    int queryEmpty(int u, int tl, int tr, int val) {
        push(u,tl,tr);
        if(tl==tr) return tl;

        int tm = tl+(tr-tl)/2;

        if(mx[u*2] >= val) {
            return queryEmpty(u*2,tl,tm,val);
        }
        else if(suff[u*2]+pref[u*2+1] >= val) {
            return tm-suff[u*2]+1;
        }
        else if(mx[u*2+1] >= val) {
            return queryEmpty(u*2+1,tm+1,tr,val);
        }
        else {
            return tl;
        }
    }

    int queryEmpty(int val) {
        if(mx[1] < val) return -1;
        return queryEmpty(1,0,n-1,val);
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // freopen("seating.in","r",stdin);
    // freopen("seating.out","w",stdout);

    int n,qn,ans=0; cin >> n >> qn;
    Segtree segtree(n);

    while(qn--) {
        char type; cin >> type;
        if(type=='A') {
            int q; cin >> q;
            int found = segtree.queryEmpty(q);
            if(found==-1) ans++;
            else segtree.update(found,found+q-1,1);
        }
        else {
            int l,r; cin >> l >> r;
            l--; r--;
            segtree.update(l,r,0);
        }
        // segtree.debug();
    }
    cout << ans << endl;
}
