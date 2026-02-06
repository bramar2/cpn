#include <bits/stdc++.h>
using namespace std;

struct UnionFind {
    vector<int> parent, size;
    UnionFind(int sz) : parent(sz), size(sz, 1) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        return parent[x] == x ? x : (parent[x] = find(parent[x]));
    }
    
    bool unite(int a, int b) {
        int p1 = find(a), p2 = find(b);
        if(p1 == p2) return false;
        if(size[p1] < size[p2]) swap(p1, p2);
        parent[p2] = p1;
        size[p1] += size[p2];
        return true;
    }
};

int main() {
    int n, q;
    cin >> n >> q;
    
    UnionFind dsu(n);
    
    while(q--) {
        int t, u, v;
        cin >> t >> u >> v;
        if(t == 0) dsu.unite(u, v);
        else cout << (dsu.find(u) == dsu.find(v)) << "\n";
    }
    return 0;
}
