// havent looked at editorial yet, but its safe to say this IS NOT the optimal sol impl wise

#include <bits/stdc++.h>
using namespace std;
#define int long long

const int MOD = 1e9+7;

struct Node {
    int pos,cntZero,sum,curr,adding,prev;
    bool operator<(const Node& other) const {
        return pos<other.pos;
    }
};

int n,m,qn;
vector<Node> sums;
int twk;

int exp(int b, int e) {
    int res = 1;
    while(e>0) {
        if(e%2==1) res=res*b%MOD;
        e/=2;
        b=b*b%MOD;
    }
    return res;
}

// r is not perfectly defined
int getOneCount(int l, int r) {
    r++;
    Node leftBound = *lower_bound(sums.begin(),sums.end(),Node{-l,INT_MIN}), rightBound = *lower_bound(sums.begin(),sums.end(),Node{-r,INT_MIN});
    int leftX = -leftBound.pos, leftZero = leftBound.cntZero, leftParity = leftBound.curr;
    int rightX = -rightBound.pos, rightZero = rightBound.cntZero, rightParity = rightBound.curr;

    int exactRightZero = r>n?0:rightZero;
    if(r<=n && rightParity==0) {
        exactRightZero += (rightX-r);
    }

    int zeroInRange = leftZero-exactRightZero;
    return (r-l)-zeroInRange;
}

int getZeroSuffCount(int x) {
    Node v = *lower_bound(sums.begin(),sums.end(),Node{-x,INT_MIN,INT_MIN});
    return v.cntZero;
} 

// l is not perfectly defined
int getRangeSum(int l, int r) {
    r++;
    Node leftBound = *lower_bound(sums.begin(),sums.end(),Node{-l,INT_MIN}), rightBound = *lower_bound(sums.begin(),sums.end(),Node{-r,INT_MIN});
    int leftSum = leftBound.sum, leftX = -leftBound.pos, leftParity = leftBound.curr, specificSum = leftBound.adding, prevLeftX = leftBound.prev,
    rightSum = rightBound.sum;

    int rangeSum = (leftSum-(r>n?0:rightSum)+MOD)%MOD;
    if(leftParity==0) return rangeSum;

    int bitIndex = n-l, prevBitIndex = n-prevLeftX-1;
    return (leftSum-specificSum+exp(2,bitIndex+1)-1-(exp(2,prevBitIndex+1)-1)+MOD)%MOD;
}


int getSum(int l, int m, int r, int k, int invNR) {
    int oneCount = getOneCount(l,m), stillNeed = k-oneCount;
    if(stillNeed > r-m) return -1;

    int subStart = r-stillNeed+1;
    int subSum = getRangeSum(subStart,r);

    int firstSum = twk-1 - (exp(2,stillNeed)-1);
    firstSum=(firstSum+MOD)%MOD;
    int secondSum = subSum*invNR;

    return (firstSum+secondSum+MOD)%MOD;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> qn;
    set<pair<int,int>> flips;
    
    for(int i = 0; i < m; i++) {
        int l,r; cin >> l >> r;
        if(!flips.count({l-1,1})) flips.insert({l-1,1});
        else flips.erase({l-1,1});
        if(!flips.count({r,1})) flips.insert({r,1});
        else flips.erase({r,1});
    }
    flips.erase({0,1});
    if(!flips.count({0,1})) flips.insert({0,0});

    vector<vector<int>> queries;
    for(int i = 0; i < qn; i++) {
        int l,r,k; cin >> l >> r >> k;
        queries.push_back({l,r,k});
        if(!flips.count({l-1,1}) && !flips.count({l-1,0})) flips.insert({{l-1,0}});
        if(!flips.count({r,1}) && !flips.count({r,0})) flips.insert({{r,0}});
    }

    int curr = 0, cntZero = 0, sum = 0, prev = n;

    for(auto it = flips.rbegin(); it != flips.rend(); it++) {
        int x = it->first;
        int bitIndex = n-x-1, prevBitIndex = n-prev-1;
        int adding = 0;

        if(curr==0) { // everything before us were 0
            cntZero += prev-x;
        }
        else {
            adding = (exp(2,bitIndex+1)-1-(exp(2,prevBitIndex+1)-1)+MOD)%MOD;
            sum = (sum+adding)%MOD;
        }

        if(x!=n) sums.push_back({-(x+1),cntZero,sum,curr,adding,prev});
        curr=(curr+it->second)&1;
        prev = x;
    }

    for(vector<int>& q : queries) {
        int l=q[0],r=q[1],k=q[2];
        twk = exp(2,k);
        if(getOneCount(l,r) >= k) {
            cout << twk-1 << "\n";
            continue;
        }
        
        int binL = l-1, binR = r, ans = 0, invNR = exp(exp(2,n-r),MOD-2);
        while(binL <= binR) {
            int binM = (binL+binR)/2;
            int res = getSum(l,binM,r,k,invNR);

            if(res == -1) {
                binR = binM-1;
            }
            else {
                ans = res;
                binL = binM+1;
            }
        }

        cout << ans << "\n";
    }
}