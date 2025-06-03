#include <iostream>
#include <vector>

using namespace std;

struct SegmentTree {
    int n;
    long long mod;
    vector<long long> tree;    // 存储区间和
    vector<long long> lazy_mul; // 乘法懒惰标记
    vector<long long> lazy_add; // 加法懒惰标记
    
    SegmentTree(vector<long long>& arr, long long p) {
        n = arr.size(); 
        mod = p;
        tree.resize(4 * n);
        lazy_mul.resize(4 * n, 1);  // 乘法标记初始化为1
        lazy_add.resize(4 * n, 0);  // 加法标记初始化为0
        build(1, 0, n - 1, arr);
    }
    
    // 构建线段树
    void build(int node, int start, int end, vector<long long>& arr) {
        if (start == end) {
            tree[node] = arr[start] % mod;
        } else {
            int mid = (start + end) / 2;
            build(2 * node, start, mid, arr);
            build(2 * node + 1, mid + 1, end, arr);
            tree[node] = (tree[2 * node] + tree[2 * node + 1]) % mod;
        }
    }
    
    // 下推懒惰标记
    void push(int node, int start, int end) {
        if (lazy_mul[node] != 1 || lazy_add[node] != 0) {
            // 先处理乘法,再处理加法
            tree[node] = (tree[node] * lazy_mul[node] % mod + 
                         lazy_add[node] * (end - start + 1) % mod) % mod;
            
            if (start != end) { // 不是叶子节点,传播给子节点
                // 对于子节点,新的乘法标记 = 原乘法标记 * 当前乘法标记
                lazy_mul[2 * node] = (lazy_mul[2 * node] * lazy_mul[node]) % mod;
                lazy_mul[2 * node + 1] = (lazy_mul[2 * node + 1] * lazy_mul[node]) % mod;
                
                // 对于子节点,新的加法标记 = 原加法标记 * 当前乘法标记 + 当前加法标记
                lazy_add[2 * node] = (lazy_add[2 * node] * lazy_mul[node] % mod + lazy_add[node]) % mod;
                lazy_add[2 * node + 1] = (lazy_add[2 * node + 1] * lazy_mul[node] % mod + lazy_add[node]) % mod;
            }
            
            // 清除当前节点的懒惰标记
            lazy_mul[node] = 1;
            lazy_add[node] = 0;
        }
    }
    
    // 区间乘法更新
    void update_multiply(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end); // 先下推已有的懒惰标记
        
        if (start > end || start > r || end < l) // 区间不相交
            return;
        
        if (start >= l && end <= r) { // 当前区间完全包含在更新区间内
            lazy_mul[node] = (lazy_mul[node] * val) % mod;
            lazy_add[node] = (lazy_add[node] * val) % mod;
            push(node, start, end);
            return;
        }
        
        // 部分重叠,递归更新子节点
        int mid = (start + end) / 2;
        update_multiply(2 * node, start, mid, l, r, val);
        update_multiply(2 * node + 1, mid + 1, end, l, r, val);
        
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = (tree[2 * node] + tree[2 * node + 1]) % mod;
    }
    
    // 区间加法更新
    void update_add(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end); // 先下推已有的懒惰标记
        
        if (start > end || start > r || end < l) // 区间不相交
            return;
        
        if (start >= l && end <= r) { // 当前区间完全包含在更新区间内
            lazy_add[node] = (lazy_add[node] + val) % mod;
            push(node, start, end);
            return;
        }
        
        // 部分重叠,递归更新子节点
        int mid = (start + end) / 2;
        update_add(2 * node, start, mid, l, r, val);
        update_add(2 * node + 1, mid + 1, end, l, r, val);
        
        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = (tree[2 * node] + tree[2 * node + 1]) % mod;
    }
    
    // 区间查询求和
    long long query(int node, int start, int end, int l, int r) {
        if (start > end || start > r || end < l) // 区间不相交
            return 0;
        
        push(node, start, end); // 先下推懒惰标记
        
        if (start >= l && end <= r) // 当前区间完全包含在查询区间内
            return tree[node];
        
        // 部分重叠,递归查询子节点
        int mid = (start + end) / 2;
        long long left_sum = query(2 * node, start, mid, l, r);
        long long right_sum = query(2 * node + 1, mid + 1, end, l, r);
        return (left_sum + right_sum) % mod;
    }
    
    // 对外接口：区间乘法
    void multiply_range(int l, int r, long long c) {
        update_multiply(1, 0, n - 1, l, r, c);
    }
    
    // 对外接口：区间加法
    void add_range(int l, int r, long long c) {
        update_add(1, 0, n - 1, l, r, c);
    }
    
    // 对外接口：区间查询
    long long query_range(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    long long P;
    cin >> n >> P;
    
    vector<long long> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    
    SegmentTree st(a, P);
    
    int M;
    cin >> M;
    
    for (int i = 0; i < M; i++) {
        int op;
        cin >> op;
        
        if (op == 1) { // 区间乘法
            int t, g;
            long long c;
            cin >> t >> g >> c;
            st.multiply_range(t - 1, g - 1, c % P); // 转换为0索引
        } else if (op == 2) { // 区间加法
            int t, g;
            long long c;
            cin >> t >> g >> c;
            st.add_range(t - 1, g - 1, c % P); // 转换为0索引
        } else { // 区间查询
            int t, g;
            cin >> t >> g;
            cout << st.query_range(t - 1, g - 1) << "\n"; // 转换为0索引
        }
    }
    
    return 0;
}
/*1. 维护序列
【问题描述】

老师交给小可可一个维护数列的任务，现在小可可希望你来帮他完成。

有长为 n 的数列，不妨设为 a_1,a_2,... ,a_n。有如下三种操作形式：

把数列中的一段数全部乘一个值；

把数列中的一段数全部加一个值；

询问数列中的一段数的和，由于答案可能很大，你只需输出这个数模 P 的值。

【输入形式】
第一行两个整数 n 和 P；

第二行含有 n 个非负整数，从左到右依次为 a_1,a_2,... ,a_n；

第三行有一个整数 M，表示操作总数；

从第四行开始每行描述一个操作，输入的操作有以下三种形式：

操作 1：1 t g c，表示把所有满足 t ≤ i ≤ g 的 a_i 改为 a_i * c；

操作 2：2 t g c，表示把所有满足 t ≤ i ≤ g 的 a_i 改为 a_i + c；

操作 3：3 t g，询问所有满足 t ≤ i ≤ g 的 a_i 的和模 P 的值。

同一行相邻两数之间用一个空格隔开，每行开头和末尾没有多余空格。

数据保证 1 ≤ t ≤ g ≤ n; 1≤ n,M ≤ 100,000; 0 ≤ c,a_i ≤ 10⁹; 1 ≤ P ≤ 10⁹+7.

【输出形式】

对每个操作 3，按照它在输入中出现的顺序，依次输出一行一个整数表示询问结果。

【样例输入】

7 43
1 2 3 4 5 6 7
5
1 2 5 5
3 2 4
2 3 7 9
3 1 3
3 4 7
【样例输出】


2
35
8
*/