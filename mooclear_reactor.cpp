/*
problem: https://usaco.org/index.php?page=viewproblem2&cpid=1543&lang=en
diabolical implementation
I got so lost implementing, i prob dont need half of this code (some of the methods still have pointless return values), but I'm too scared to remove any soooooo

*/





#include <bits/stdc++.h>
using namespace std;
#define int long long

int n,m;
vector<bool> visited;
vector<set<pair<int,int>>> edges;
vector<pair<int,int>> range;

vector<int> coloring;
vector<pair<int,int>> parent;
vector<int> values;

bool imposs = false;

int fillAndCheck(int start, int sum) {
    values[start] = sum;
    queue<pair<int,int>> aq;
    aq.push({start,sum});


    int res = 0;
    while(!aq.empty()) {
        auto [u,x] = aq.front(); aq.pop();
        visited[u] = true;
        res += (values[u] >= range[u].first && values[u] <= range[u].second);
        for(pair<int,int> v : edges[u]) {
            if(values[v.first] == INT_MAX) {
                values[v.first] = v.second-x;
                aq.push({v.first,v.second-x});
            }
        }
    }
    return res;
}

void colorDfs(int u, int p, int c, int &cycleNode) {
    for(auto v : edges[u]) {
        if(coloring[v.first] != -1 && coloring[v.first] == c) {
            parent[v.first] = {u,v.second};
            cycleNode = v.first;
            return;
        }

        if(coloring[v.first]==-1) {
            coloring[v.first] = c^1;
            parent[v.first] = {u,v.second};
            colorDfs(v.first,u,c^1,cycleNode);
        }
        if(cycleNode!=-1) break;
    }
}

int doOddCycle(int start) {
    stack<tuple<int,int,int>> q; // named a q but a stack!
    int cycleNode = -1;
    coloring[start] = 0;
    colorDfs(start,-1,0,cycleNode);

    if(cycleNode==-1) return -1;

    int curr = cycleNode;
    int sum = 0;
    
    while(true) {
        auto [p,w] = parent[curr];
        sum = -sum+w;

        curr = p;
        if(curr==cycleNode) break;
    }
    if(sum%2==1) {
        imposs = true;
        return 0;
    }
    int res = fillAndCheck(cycleNode,sum/2);
    return res;
}

int doComp(int start) {
    // check if this is a special cycle case
    int oddRes = doOddCycle(start);
    if(imposs) return 0;
    if(oddRes!=-1) return oddRes;

    vector<pair<int,int>> intervals;
    queue<tuple<int,int,bool>> q;

    q.push({start,0,true});
    visited[start] = true;
    while(!q.empty()) {
        auto [u,sum,pos] = q.front(); q.pop();
        int L = range[u].first-sum, R = range[u].second-sum;
        if(!pos) {
            L = -L;
            R = -R;
        }
        if(L>R) swap(L,R);
        intervals.push_back({L,R});
        
        for(pair<int,int> v : edges[u]) {
            if(!visited[v.first]) {
                visited[v.first] = true;
                q.push({v.first,-sum+v.second,!pos});
            }
        }
    }

    vector<pair<int,int>> events; // index, close = -1/open = 1S
    for(pair<int,int> p : intervals) {
        events.push_back({p.first,1});
        events.push_back({p.second+1,-1});
    }
    sort(events.begin(),events.end());

    int cnt = 0, res = 0, resVal = INT_MAX;
    for(pair<int,int> e : events) {
        cnt += e.second;
        if(cnt > res) {
            resVal = e.first;
            res = cnt;
        }
    }
    res = fillAndCheck(start,resVal);
    return res;
}   

int solve() {
    cin >> n >> m;
    vector<tuple<int,int,int>> rawEdges;

    range.assign(n,{}); visited.assign(n,false); edges.assign(n,{}); 
    coloring.assign(n,-1); parent.assign(n,{-1,-1}); values.assign(n,INT_MAX);
    
    for(int i = 0; i < n; i++) cin >> range[i].first;
    for(int i = 0; i < n; i++) cin >> range[i].second;
    
    for(int i = 0; i < m; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        u--; v--;
       
        edges[u].insert({v,w});
        edges[v].insert({u,w});
        rawEdges.push_back({u,v,w});
    }

    // checking if this graph is valid
    
    for(int u = 0; u < n; u++) {
        map<int,int> mp;
        for(pair<int,int> v : edges[u]) {
            if(u==v.first&&v.second&1) return -1;
            if(mp.count(v.first) && v.second != mp[v.first]) return -1;
            mp[v.first] = v.second;
        }
    }

    int ans = 0;
    for(int u = 0; u < n; u++) {
        if(!visited[u]) ans += doComp(u);
        if(imposs) return -1;
    }

    ans = 0;
    for(int i = 0; i < n; i++) {
        if(values[i] >= range[i].first && values[i] <= range[i].second) ans++;
        // cout << values[i] << " ";
    }
    // cout << endl;

    for(auto [u,v,w] : rawEdges) {
        if(values[u] + values[v] != w) {
            return -1;
        }
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
