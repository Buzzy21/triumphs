/*
https://usaco.org/index.php?page=viewproblem2&cpid=1450

10 hours down the drain
*/


#include <bits/stdc++.h>
using namespace std;
#define int long long

const int MOD = 1e9+7;

struct SegmentTree {
    int n;
    vector<array<int,2>> tree;

    SegmentTree(int n) {
        this->n = n;
        tree.resize(4*n,{0,0});
    }

    void update(int u, int tl, int tr, int index, int val) {
        if(tl==tr) {
            tree[u][index%2] = val;
            return;
        }
        int tm=(tl+tr)/2;
        if(index<=tm) update(u*2,tl,tm,index,val);
        else update(u*2+1,tm+1,tr,index,val);

        tree[u][0] = (tree[u*2][0]  + tree[u*2+1][0]) % MOD;
        tree[u][1] = (tree[u*2][1]  + tree[u*2+1][1]) % MOD;
    }
    void update(int index, int val) {update(1,0,n-1,index,val);}

    int query(int u, int tl, int tr, int l, int r, int m) {
        if(l>r) return 0;
        if(l==tl&&r==tr) return tree[u][m];

        int tm=(tl+tr)/2;
        return (query(u*2,tl,tm,l,min(r,tm),m)+query(u*2+1,tm+1,tr,max(l,tm+1),r,m))%MOD;
    }
    int query(int l, int r, int m) {return query(1,0,n-1,l,r,m);}
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin >> n;
    vector<int> a(n+1); // 0 = X, 1 = R, 2 = B
    a[0] = -1;
    for(int i = 1; i <= n; i++) {
        char c; cin >> c;
        if(c=='X') a[i] = 0;
        else if(c=='R') a[i] = 1;
        else a[i] = 2;
    }

    SegmentTree segtree(n+1);
    vector<int> nextB(n+2,2e9);
    for(int i = n; i >= 0; i--) {
        if(a[i]==2) nextB[i] = i;
        else if(i+1<n) nextB[i] = nextB[i+1];
    }

    int lastR = -1;
    set<pair<int,int>> stops;
    if(a[0]==1) assert(1==2);
    segtree.update(0,1);
    stops.insert({2*nextB[1],0});

    for(int i = 1; i <= n; i++) {
        while(!stops.empty() && stops.begin()->first<=i) {
            segtree.update(stops.begin()->second,0);
            stops.erase(stops.begin());
        }

        if(a[i]==0) {
            int from = max(0ll,(2*lastR)-i);
            segtree.update(i,segtree.query(from,i,i%2)+segtree.query(i-1,i-1,(i-1)%2));

            int stopAt = 2*nextB[i+1]-i;
            stops.insert({stopAt,i});
        }
        if(a[i]==2) {
            int from = max(0ll,(2*lastR)-i);
            segtree.update(i,segtree.query(from,i,i%2));

            int stopAt = 2*nextB[i+1]-i;
            stops.insert({stopAt,i});
        }
        if(a[i]==1) {
            lastR = i;    
        }
    }

    cout << segtree.query(n,n,n%2) << endl;
}
