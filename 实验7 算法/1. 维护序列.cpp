#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

const int MAXN = 100005;
long long arr[MAXN]; // 输入数组,存储原始数据

// 线段树节点结构体
struct Node {
    long long sum;      // 当前区间的和
    long long mul_tag;  // 乘法懒惰标记,表示该节点及其子树需要乘以的值
    long long add_tag;  // 加法懒惰标记,表示该节点及其子树需要加上的值

    // 构造函数：初始化sum为0,mul_tag为1（乘法单位元）,add_tag为0（加法单位元）
    Node() : sum(0), mul_tag(1), add_tag(0) {}
};

Node tree[4 * MAXN];  // 线段树数组,大小为4倍原数组长度
int N_global;         // 全局变量：数组长度
long long P_global;   // 全局变量：模数

// 向上更新：将子节点的信息合并到父节点
void push_up(int node_idx) {
    // 父节点的和 = 左子节点的和 + 右子节点的和（取模）
    tree[node_idx].sum = (tree[node_idx * 2].sum + tree[node_idx * 2 + 1].sum) % P_global;
}

// 向下传播懒惰标记：将当前节点的懒惰标记传递给子节点
void push_down(int node_idx, int current_l, int current_r) {
    // 如果没有懒惰标记需要传播,直接返回
    if (tree[node_idx].mul_tag == 1 && tree[node_idx].add_tag == 0) {
        return;
    }

    long long pm = tree[node_idx].mul_tag;  // 当前节点的乘法标记
    long long pa = tree[node_idx].add_tag;  // 当前节点的加法标记

    int mid = (current_l + current_r) / 2;
    int len_l_child = mid - current_l + 1;      // 左子树区间长度
    int len_r_child = current_r - mid;          // 右子树区间长度

    // 更新左子节点
    Node& lc = tree[node_idx * 2];
    // 新的和 = 原和 * 乘法标记 + 区间长度 * 加法标记
    lc.sum = (lc.sum * pm + (long long)len_l_child * pa) % P_global;
    // 新的乘法标记 = 原乘法标记 * 父节点的乘法标记
    lc.mul_tag = (lc.mul_tag * pm) % P_global;
    // 新的加法标记 = 原加法标记 * 父节点的乘法标记 + 父节点的加法标记
    lc.add_tag = (lc.add_tag * pm + pa) % P_global;

    // 更新右子节点（逻辑同左子节点）
    Node& rc = tree[node_idx * 2 + 1];
    rc.sum = (rc.sum * pm + (long long)len_r_child * pa) % P_global;
    rc.mul_tag = (rc.mul_tag * pm) % P_global;
    rc.add_tag = (rc.add_tag * pm + pa) % P_global;

    // 清除父节点的懒惰标记
    tree[node_idx].mul_tag = 1;
    tree[node_idx].add_tag = 0;
}

// 构建线段树
void build(int node_idx, int current_l, int current_r) {
    // 初始化懒惰标记
    tree[node_idx].mul_tag = 1;
    tree[node_idx].add_tag = 0;
    
    if (current_l == current_r) { // 叶子节点
        tree[node_idx].sum = arr[current_l] % P_global;
    } else { // 内部节点
        int mid = (current_l + current_r) / 2;
        // 递归构建左右子树
        build(node_idx * 2, current_l, mid);
        build(node_idx * 2 + 1, mid + 1, current_r);
        // 向上更新当前节点
        push_up(node_idx);
    }
}

// 区间乘法更新：将区间[target_l, target_r]内的所有元素乘以val_c
void update_multiply(int node_idx, int current_l, int current_r, int target_l, int target_r, long long val_c) {
    // 当前区间与目标区间无交集
    if (current_l > target_r || current_r < target_l) {
        return;
    }
    
    // 当前区间完全包含在目标区间内
    if (target_l <= current_l && current_r <= target_r) {
        // 更新当前节点的和
        tree[node_idx].sum = (tree[node_idx].sum * val_c) % P_global;
        // 更新乘法标记
        tree[node_idx].mul_tag = (tree[node_idx].mul_tag * val_c) % P_global;
        // 更新加法标记（加法标记也要乘以val_c）
        tree[node_idx].add_tag = (tree[node_idx].add_tag * val_c) % P_global;
        return;
    }

    // 部分重叠：先下推懒惰标记,然后递归更新子节点
    push_down(node_idx, current_l, current_r);
    int mid = (current_l + current_r) / 2;
    update_multiply(node_idx * 2, current_l, mid, target_l, target_r, val_c);
    update_multiply(node_idx * 2 + 1, mid + 1, current_r, target_l, target_r, val_c);
    push_up(node_idx); // 向上更新
}

// 区间加法更新：将区间[target_l, target_r]内的所有元素加上val_c
void update_add(int node_idx, int current_l, int current_r, int target_l, int target_r, long long val_c) {
    // 当前区间与目标区间无交集
    if (current_l > target_r || current_r < target_l) {
        return;
    }
    
    // 当前区间完全包含在目标区间内
    if (target_l <= current_l && current_r <= target_r) {
        long long range_len = current_r - current_l + 1; // 当前区间长度
        // 更新当前节点的和：原和 + 区间长度 * 加法值
        tree[node_idx].sum = (tree[node_idx].sum + range_len * val_c) % P_global;
        // 更新加法标记
        tree[node_idx].add_tag = (tree[node_idx].add_tag + val_c) % P_global;
        return;
    }

    // 部分重叠：先下推懒惰标记,然后递归更新子节点
    push_down(node_idx, current_l, current_r);
    int mid = (current_l + current_r) / 2;
    update_add(node_idx * 2, current_l, mid, target_l, target_r, val_c);
    update_add(node_idx * 2 + 1, mid + 1, current_r, target_l, target_r, val_c);
    push_up(node_idx); // 向上更新
}

// 区间求和查询：查询区间[target_l, target_r]内所有元素的和
long long query_sum(int node_idx, int current_l, int current_r, int target_l, int target_r) {
    // 当前区间与目标区间无交集
    if (current_l > target_r || current_r < target_l) {
        return 0;
    }
    
    // 当前区间完全包含在目标区间内
    if (target_l <= current_l && current_r <= target_r) {
        return tree[node_idx].sum;
    }

    // 部分重叠：先下推懒惰标记,然后递归查询子节点
    push_down(node_idx, current_l, current_r);
    int mid = (current_l + current_r) / 2;
    long long res_l = query_sum(node_idx * 2, current_l, mid, target_l, target_r);
    long long res_r = query_sum(node_idx * 2 + 1, mid + 1, current_r, target_l, target_r);
    return (res_l + res_r) % P_global; // 返回左右子树查询结果的和
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读取数组长度和模数
    cin >> N_global >> P_global;
    
    // 读取原始数组（1-indexed）
    for (int i = 1; i <= N_global; ++i) {
        cin >> arr[i];
    }

    // 构建线段树
    build(1, 1, N_global);

    // 读取操作数量
    int M;
    cin >> M;
    
    // 处理每个操作
    for (int k = 0; k < M; ++k) {
        int type;
        cin >> type;
        
        if (type == 1) { // 操作1：区间乘法
            int t, g;
            long long c;
            cin >> t >> g >> c;
            update_multiply(1, 1, N_global, t, g, c);
        } else if (type == 2) { // 操作2：区间加法
            int t, g;
            long long c;
            cin >> t >> g >> c;
            update_add(1, 1, N_global, t, g, c);
        } else { // 操作3：区间查询
            int t, g;
            cin >> t >> g;
            cout << query_sum(1, 1, N_global, t, g) << "\n";
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