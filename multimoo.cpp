// took a damn load of nasty codewriting and what-i-call memoizations to just bareeeeeeeely slide through test case 9
// i hope test case 9 burns in a fire 

#include <bits/stdc++.h>
using namespace std;
#define int long long

vector<pair<int,int>> dir = {{0,1},{1,0},{-1,0},{0,-1}};
vector<int> idNum(62501,-1);
vector<int> idSize(62501,0);
vector<unordered_set<int>> edges(62501);
set<vector<int>> done;

void fill(int i, int j, vector<vector<int>>& idMatrix, vector<vector<int>>& grid, int& id, int& num) {
    if(idMatrix[i][j] != -1 || grid[i][j] != num) {
        if(idMatrix[i][j] != id && idMatrix[i][j] != -1) {
            edges[id].insert(idMatrix[i][j]);
            edges[idMatrix[i][j]].insert(id); 
        }
        return; 
    }
    idSize[id]++;
    idMatrix[i][j] = id;

    for(auto [a,b] : dir) {
        int ni = i + a, nj = j + b;
        if(ni >= 0 && nj >= 0 && ni < grid.size() && nj < grid[0].size()) {
            fill(ni,nj,idMatrix,grid,id,num);
        }
    }
    return;
}

int find(int a, int b, vector<unordered_set<int>>& edges, vector<int>& idSize, vector<int>& idNum) {
    int aNum = idNum[a], bNum = idNum[b];
    vector<bool> visited(62501,false);
    int count = 0;
    queue<int> q;
    q.push(a);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        if(done.find({aNum,bNum,u}) != done.end()) continue;
        done.insert({aNum,bNum,u});
        if(visited[u]) continue;
        visited[u] = true;
        count += idSize[u];

        for(auto v : edges[u]) {
            if(idNum[v] == aNum || idNum[v] == bNum) {
                q.push(v);
            }
        }
    }
    return count;
}       

signed main() {
    //freopen("multimoo.in","r",stdin);
    //freopen("multimoo.out","w",stdout);
    int n;
    cin >> n;
    vector<vector<int>> grid(n,vector<int>(n));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    vector<vector<int>> idMatrix(n,vector<int>(n,-1));
    int nextId = -1;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(idMatrix[i][j] == -1) {
                nextId++;
                idNum[nextId] = grid[i][j];
                fill(i,j,idMatrix,grid,nextId,grid[i][j]);
            }
        }
    }

    cout << *max_element(idSize.begin(),idSize.end()) << endl;
    
    int ans = 0;
    for(int i = 0; i < nextId; i++) {
        for(int j : edges[i]) {
            ans = max(ans,find(i,j,edges,idSize,idNum));
        }
    }
    cout << ans << endl;
}
