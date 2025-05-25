// THIS TOOK SO PAINFULLY LONG 
// PROBLEM LINK: https://usaco.org/index.php?page=viewproblem2&cpid=1470
#include <bits/stdc++.h>
using namespace std;
#define int long long

int n;
vector<int> A,B;
map<int,vector<int>> distListA;
map<int,vector<int>> distListB;
int ans = 0;

void calc_same(int i) {
    int total = 0;
    total += min(i+1,n-i)-1;
    total += (i*(i+1))/2;
    total += ((n-i-1)*(n-i))/2;
    ans += total;
    //  cout << "i: " << i << " same total: " << total << endl;
    return;
}

void calc_priority_A(int i) {
    int num = A[i];
    vector<int>& dists = distListB[num];
    int dist = min(i,n-i-1);
    
    // search for first element not prioritized against
    int first = lower_bound(dists.begin(),dists.end(),dist) - dists.begin(); 
    int total = (dists.size()-first) * (dist+1);
    //  cout << "i: " << i << " first: " << first << " dist: " << dist << endl; 
    //  cout << "total : " << total << endl;
    ans += total;   

    if(A[i] == B[i]) ans -= dist;
    return;
}

void calc_priority_B(int i) {
    int num = B[i];
    vector<int>& dists = distListA[num];
    int dist = min(i,n-i-1);
    
    // search for first element not prioritized against, except this time sames don't count
    int first = lower_bound(dists.begin(),dists.end(),dist+1) - dists.begin();
    int total = (dists.size()-first) * (dist+1);
    ans += total;
    return;
}   

void scan_A() {
    for(int i = 0; i < n; i++) {
        if(A[i] == B[i]) {
            calc_same(i);
        }
        calc_priority_A(i);
    }
    return;
}

void scan_B() {
    for(int i = 0; i < n; i++) {
        calc_priority_B(i);
    }
    return;
}

signed main() {
    cin >> n;
    A.resize(n); 
    B.resize(n);

    for(int i = 0; i < n; i++) {
        cin >> A[i];
        distListA[A[i]].push_back(min(i,n-i-1));
    }
    for(int i = 0; i < n; i++) {
        cin >> B[i];
        distListB[B[i]].push_back(min(i,n-i-1));
    }
    for(auto& it : distListA) {
        vector<int>& v = it.second;
        sort(v.begin(),v.end());
    }
    for(auto& it : distListB) {
        vector<int>& v = it.second;
        sort(v.begin(),v.end());
    }

    scan_A(); 
    scan_B();
    
    cout << ans << endl;
}
