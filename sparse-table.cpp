struct SparseTable {
    int n, k;
    vector<vector<int>> st;
    
    template <typename Container>
    SparseTable(const Container& array) {
        n = size(array);
        k = 31 - __builtin_clz(n);
        st = vector(k + 1, vector<int>(n));
        copy(begin(array), end(array), begin(st[0]));
        for(int i = 1; i <= k; i++)
            for(int j = 0; j + (1 << i) <= n; j++)
                st[i][j] = merge(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);   
    }
 
    int query(int l, int r) {
        int i = 31 - __builtin_clz(r - l + 1);
        return merge(st[i][l], st[i][r - (1 << i) + 1]);
    }
 
    int merge(int a, int b) {
        return lcm(a, b);
    }
};
