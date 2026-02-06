#pragma GCC optimize ("O3")
template <class T, class Z>
struct capply_lazy {
    T operator() (T node, Z lazy, int len) const {
        return node + lazy;
    }
};

template <class T>
struct cmerge_node_min {
    T operator() (T a, T b) const {
        return min(a, b);
    }
};

template <class T>
struct cmerge_node_max {
    T operator() (T a, T b) const {
        return max(a, b);
    }
};

// https://codeforces.com/blog/entry/18051
template <class T, class Z = T,
    class merge_node = plus<T>,
    class combine_lazy = plus<Z>,
    class apply_lazy = capply_lazy<T, Z>>
struct LazySegmentTree {
    int n, h;
    vector<T> tree;
    vector<Z> lazy;

    T default_value;
    Z default_lazy_value;

    LazySegmentTree(int size, T def = T(), Z def_lazy = Z()) :
        default_value(def), default_lazy_value(def_lazy) {
        n = (size == 1) ? 1 : (1 << (8 * sizeof(int) - __builtin_clz(size - 1)));
        h = sizeof(int) * 8 - 1 - __builtin_clz(n);
        tree.resize(2*n, default_value);
        lazy.resize(n, default_lazy_value);
    }


    template <typename Container>
    void build(const Container& arr) {
        copy(begin(arr), end(arr), begin(tree) + n);
        build_from_leaves();
    }

    void build_from_leaves() {
        for(int i = n - 1; i > 0; i--) tree[i] = merge_node()(tree[i<<1], tree[i<<1|1]);
    }

    
    int segment(int p) {
        return n >> (sizeof(int) * 8 - 1 - __builtin_clz(p));
    }

    void apply(int p, Z value) {
        tree[p] = apply_lazy()(tree[p], value, segment(p));
        if(p < n) lazy[p] = combine_lazy()(lazy[p], value);
    }

    void build(int p) {
        while(p > 1) {
            p >>= 1;
            tree[p] = apply_lazy()(merge_node()(tree[p<<1], tree[p<<1|1]), lazy[p], segment(p));
        }
    }

    void push(int p) {
        for(int s = h; s > 0; s--) {
            int i = p >> s;
            if(lazy[i] != default_lazy_value) {
                apply(i<<1, lazy[i]);
                apply(i<<1|1, lazy[i]);
                lazy[i] = default_lazy_value;
            }
        }
    }

    void update(int l, int r, Z value) {
        l += n, r += n + 1;
        int l0 = l, r0 = r;
        for(; l < r; l >>= 1, r >>= 1) {
            if(l & 1) apply(l++, value);
            if(r & 1) apply(--r, value);
        }
        build(l0);
        build(r0 - 1);
    }

    T query(int l, int r) {
        l += n, r += n + 1;
        push(l);
        push(r - 1);
        T resLeft = default_value, resRight = default_value;
        for(; l < r; l >>= 1, r >>= 1) {
            if(l & 1) resLeft = merge_node()(resLeft, tree[l++]);
            if(r & 1) resRight = merge_node()(tree[--r], resRight);
        }
        return merge_node()(resLeft, resRight);
    }
};

LazySegmentTree<int, int, cmerge_node_min<int>> st_min(100'000, INT_MAX);
LazySegmentTree<int, int, cmerge_node_max<int>> st_max(100'000, INT_MIN);

auto init = [](){
    vector<int> initial(100'000);
    st_min.build(initial);
    st_max.build(initial);
    return 0;
}();
class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size(), ans = 0;

        vector<int> p(n);
        iota(p.begin(), p.end(), 1);
        if(nums == p) return n;

        map<int, int> last;
        for(int l = n - 1; l >= 0; l--) {
            int val = nums[l];
            int delta = (val % 2 == 0) ? +1 : -1;
            if(last.contains(val)) {
                int prev_idx = last[val];
                st_min.update(l, prev_idx - 1, delta);
                st_max.update(l, prev_idx - 1, delta);
            }else {
                st_min.update(l, n - 1, delta);
                st_max.update(l, n - 1, delta);
            }
            last[val] = l;

            int left = l + 1, right = n - 1;
            while(left <= right) {
                int mid = (left + right) / 2;
                int min_val = st_min.query(mid, n - 1);
                int max_val = st_max.query(mid, n - 1);

                if(min_val <= 0 && 0 <= max_val) {
                    ans = max(ans, mid - l + 1);
                    left = mid + 1;
                }else {
                    right = mid - 1;
                }
            }
        }

        for(int i = 0; i < n; i++) {
            st_min.update(i, i, -st_min.query(i,i));
            st_max.update(i, i, -st_max.query(i,i));
        }

        return ans;
    }
};
