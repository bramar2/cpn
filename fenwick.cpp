struct FenwickTree {
    int sz;
    vector<int> bit;
 
    explicit FenwickTree(int c_sz) : sz(c_sz + 1), bit(c_sz + 1, 0) {}
 
    int query(int r) {
        int ans = 0;
        for(r++; r > 0; r -= r & -r)
            ans += bit[r];
        return ans;
    }
    int query(int l, int r) {
        return query(r) - query(l - 1);
    }
    void update(int i, int delta) {
        for(i++; i < sz; i += i & -i)
            bit[i] += delta;
    }
    void clear() {
        fill(bit.begin(), bit.end(), 0);
    }
};
