// problem: https://usaco.org/index.php?page=viewproblem2&cpid=1307
// welp, now i know how hard gold dp can be (sadly this is prob one of the easier ones too)

#include <bits/stdc++.h>
using namespace std;
#define int long long

const int MOD = 1e9+7,NSZ=301,DSZ=19;

int n,a,b;
vector<int> v;

int f(int x, const int (&dp)[DSZ][DSZ][3]) {
    string xStr = to_string(x);

    int res = 0;

    // get all integers with lesser digits
    for(int r = 0; r < xStr.size()-1; r++) {
        for(int k = 0; k < 3; k++) {
            res = (res+dp[0][r][k])%MOD;
        }
    }

    // get all integers with full digits <= x
    for(int k = 0; k <= 1; k++) {
        res = (res+dp[0][xStr.size()-1][k])%MOD;
    }
    return res;
}


void solveFor(int x, int (&precomp)[NSZ][NSZ]) {
    string xStr = to_string(x);

    for(int i = 0; i < n; i++) {
        int dp[DSZ][DSZ][3] = {};

        // precompute dp[i][i]... because no empty state
        int precompDig = v[i];
        for(int j = 0; j < xStr.size(); j++) {
            if(precompDig < xStr[j]-'0') dp[j][j][0] = 2;
            else if(precompDig > xStr[j]-'0') dp[j][j][2] = 2;

            else dp[j][j][1] = 2;
        }
        precomp[i][i] = f(x,dp);

        for(int j = i; j < n-1; j++) {
            int nextDp[DSZ][DSZ][3] = {};

            int currDig = v[j+1];

            for(int l = 0; l < xStr.size(); l++) {
                for(int r = l; r < xStr.size(); r++) {
                    // pushing to front
                    if(l>0) {
                        int firstDig = xStr[l-1]-'0';
                        if(currDig > firstDig) {
                            nextDp[l-1][r][2] += dp[l][r][0] + dp[l][r][1] + dp[l][r][2];
                        }
                        else if(currDig < firstDig) {
                            nextDp[l-1][r][0] += dp[l][r][0] + dp[l][r][1] + dp[l][r][2];
                        }
                        else {
                            nextDp[l-1][r][0] += dp[l][r][0];
                            nextDp[l-1][r][1] += dp[l][r][1];
                            nextDp[l-1][r][2] += dp[l][r][2];
                        }
                    }

                    // pushing to back
                    if(r+1<xStr.size()) {
                        int lastDig = xStr[r+1]-'0';
                        if(currDig > lastDig) {
                            nextDp[l][r+1][2] += dp[l][r][1] + dp[l][r][2];
                            nextDp[l][r+1][0] += dp[l][r][0];
                        }
                        else if(currDig < lastDig) {
                            nextDp[l][r+1][0] += dp[l][r][1] + dp[l][r][0];
                            nextDp[l][r+1][2] += dp[l][r][2];
                        }
                        else {
                            nextDp[l][r+1][0] += dp[l][r][0];
                            nextDp[l][r+1][1] += dp[l][r][1];
                            nextDp[l][r+1][2] += dp[l][r][2];
                        }
                    }

                    // ignoring
                    nextDp[l][r][0] += dp[l][r][0];
                    nextDp[l][r][1] += dp[l][r][1];
                    nextDp[l][r][2] += dp[l][r][2];

                    for(int k = 0; k < 3; k++) {
                        nextDp[l][r][k]%=MOD;
                        nextDp[l][r+1][k]%=MOD;
                        nextDp[l-1][r][k]%=MOD;
                    }
                }
            }

            swap(dp,nextDp);
            precomp[i][j+1] = f(x,dp);
        }
    }

    for(int i = n-2; i >= 0; i--) {
        for(int j = 0; j < n; j++) {
            precomp[i][j] = (precomp[i][j]+precomp[i+1][j])%MOD;
        }
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
   
    cin >> n >> a >> b;
    v.resize(n); for(int i = 0; i < n; i++) cin >> v[i];

    int qn; cin >> qn;

    static int precompA[NSZ][NSZ], precompB[NSZ][NSZ];

    solveFor(a-1,precompA);
    solveFor(b,precompB);

    while(qn--) {
        int i,j; cin >> i >> j;
        i--; j--;

        int ans = (precompB[i][j]-precompA[i][j] + MOD)%MOD;
        cout << ans << endl;
    }
}
