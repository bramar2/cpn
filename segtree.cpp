template <class T, T default_value = T(), auto merge_node = plus<>()>
struct SegmentTree {
    int n;
    vector<T> tree;
 
    SegmentTree(int size) {
        n = 1;
        while(n < size) n *= 2;
        tree.resize(2*n, default_value);
    }
 
    template <typename Container>
    void build(const Container& arr) {
        assert((int) arr.size() <= n);
        copy(begin(arr), end(arr), begin(tree) + n);
        build_from_leaves();
    }
 
    void build_from_leaves() {
        for(int i = n - 1; i > 0; i--) tree[i] = merge_node(tree[i<<1], tree[i<<1|1]);
    }
 
    void update(int pos, T val) {
        tree[n + pos] = val;
        for(int idx = (n + pos) >> 1; idx > 0; idx >>= 1) {
            tree[idx] = merge_node(tree[idx<<1], tree[idx<<1|1]);
        }
    }
 
    T query(int x) {
        return tree[n + x];
    }
 
    T query(int l, int r) {
        T resLeft = default_value, resRight = default_value;
        for(l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if(l & 1) resLeft = merge_node(resLeft, tree[l++]);
            if(r & 1) resRight = merge_node(tree[--r], resRight);
        }
        return merge_node(resLeft, resRight);
    }
};
