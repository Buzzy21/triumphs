/*
https://usaco.org/index.php?page=viewproblem2&cpid=1471
-hands down the most torturous silver problem ive done
-implementation HELL
-like 10 hours down the drain 💔
*/

#include <bits/stdc++.h>
using namespace std;
#define int long long

int till(int n, int target, int k) {
    if(n<target) return target-n;
    return k-n+target;
}

int solve() {
    int n,k;
    cin >> n >> k;
    vector<int> v(n);
    for(int i = 0; i < n; i++) cin >> v[i];
    for(int i = 0; i < n; i++) v[i] %= k;

    int mid1 = k/2+1, mid2 = k/2+1+k%2;

    int MX = k*1.1;
    map<int,int> totalPrefix,prefix;

    for(int val : v) {
        map<int,int> prefix;   
        if(val >= mid1 || (mid1==mid2 && val+1>=mid1)) {
            prefix[1]=-1;
        }
        else if(mid1!=mid2 && val+1==mid1) {
            prefix[1]=0;
        }
        else {
            prefix[1]=1;
        }

        int time = 0;
        while(time < MX) {
            if(val > mid1 || val == 0 || (val==mid1 && mid1==mid2)) {
                prefix[time] = -1;

                time += till(val,1,k);
                val = 1;
            }

            else if(val < mid1) {
                prefix[time] = 1;

                time += till(val,mid1,k);
                val = mid1;
            }

            else {
                prefix[time] = 0;

                time += till(val,mid2,k);                                                                          
                val = mid2;
            }
        }
        
        int lastAction = 0;
        for(auto it : prefix) {
            if(it.first==0) continue;

            if(it.second==0) {
                if(lastAction==1) totalPrefix[it.first]--;
                else if(lastAction==-1) totalPrefix[it.first]++;
            }
            if(it.second==1) {
                if(lastAction==-1) totalPrefix[it.first]+=2;
                else if(lastAction==0) totalPrefix[it.first]++;
            }
            if(it.second==-1) {
                if(lastAction==1) totalPrefix[it.first]-=2;
                else if(lastAction==0) totalPrefix[it.first]--;
            }
            lastAction = it.second;
        }
    }
    
    int ans = 0;
    for(int val : v) {
        ans += min(val,k-val);
    }
    int curr = ans, last = 0, sum = 0;

    for(auto it : totalPrefix) {
        if(it.second>=0) {
            totalPrefix[it.first-1];
        }
    }

    for(auto it : totalPrefix) {
        if(it.first==0) continue;
        curr += (it.first-last-1)*sum+(it.second+sum);
        sum += it.second;

        ans = min(ans,curr);
        last = it.first;
    }

    return ans;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while(t--) {
        cout << solve() << endl;
    }
}
