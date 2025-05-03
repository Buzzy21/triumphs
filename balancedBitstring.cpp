// figured out the solution within the first 10 minutes, failed ot implement it for the next 3 days
#include <bits/stdc++.h>
using namespace std;
#define int long long

int n, k;
string s;

bool fill() {
    for(int i = 0; i+k < n; i++) {
        if(s[i] != '?' && s[i+k] != '?') {
            if(s[i] != s[i+k]) {
                // cout << i << " " << i+k << " " << s[i] << " " << s[i+k] << endl;
                return false;
            }
        }
        else if(s[i] == '?' && s[i+k] != '?') {
            s[i] = s[i+k];
        }
        else if(s[i] != '?' && s[i+k] == '?') {
            s[i+k] = s[i];
        }
    }
    return true;
}

bool solve() {
    cin >> n >> k >> s;
    if(!fill()) return false;
    reverse(s.begin(),s.end());
    if(!fill()) return false;
    reverse(s.begin(),s.end());
    // cout << s << endl;
    
    int zeroes = 0, ones = 0, unknowns = 0;
    for(int i = 0; i < k; i++) {
        if(s[i] == '0') zeroes++;
        else if(s[i] == '1') ones++;
        else unknowns++;
    }
    int standardZeroes = zeroes, standardOnes = ones, standardUnknowns = unknowns;
    
    // Check if the standards are valid
    int diff = abs(standardZeroes-standardOnes);
    int unknownsLeft = standardUnknowns-diff;
    if(unknownsLeft<0 || unknownsLeft%2!=0) return false;
    // cout << unknownsLeft << endl;

    int l = 0; 
    for(int i = k; i < n; i++) {
        if(s[i] == '0') zeroes++;
        else if(s[i] == '1') ones++;
        else unknowns++;
        
        if(s[l] == '0') zeroes--;
        else if(s[l] == '1') ones--;
        else unknowns--;
        l++;

        if(zeroes != standardZeroes || ones != standardOnes || unknowns != standardUnknowns) {
            // cout << zeroes << " " << standardZeroes << " " << ones << " " << standardOnes << " " << unknowns << " " << standardUnknowns << endl;
            return false;
        }
    }
    return true;
}

signed main() {
    int t;
    cin >> t;
    while(t--) {
        if(solve()) cout << "YES" << endl;
        else cout << "NO" << endl;
    }
}