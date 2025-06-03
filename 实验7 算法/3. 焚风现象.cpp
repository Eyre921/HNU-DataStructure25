#include <iostream>
#include <vector>
using namespace std;

// 线段树存储叶子节点的实际值,lazy数组用于区间更新
vector<long long> tree;
vector<long long> lazy;
int array_size_N; // 表示海拔数组A的最大索引（0到N）

// 将懒惰标记下推到子节点或应用到叶子节点
void push(int node, int start, int end) {
    if (lazy[node] != 0) {
        // 将懒惰值应用到当前节点的存储值
        // 对于叶子节点,tree[node]是实际海拔A[start]
        // 对于内部节点,tree[node]在这个问题的查询逻辑中不严格使用,
        // 但更新它是无害的,如果是求和问题则是标准做法
        tree[node] += lazy[node];

        if (start != end) { // 如果不是叶子节点,将懒惰值传播给子节点
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        lazy[node] = 0; // 清除当前节点的懒惰值
    }
}

// 从初始海拔数组'a'构建线段树
void build(int node, int start, int end, const vector<long long>& a) {
    lazy[node] = 0; // 初始化懒惰值
    if (start == end) { // 叶子节点
        tree[node] = a[start];
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid, a);
    build(2 * node + 1, mid + 1, end, a);
    // 内部节点的tree[node]不用于点查询,所以这里不需要聚合
}

// 更新区间[l, r]内的海拔,每个都加上'val'
void update_range(int node, int start, int end, int l, int r, int val) {
    push(node, start, end); // 首先应用当前节点的待处理懒惰更新

    // 当前线段[start, end]在更新区间[l, r]之外
    if (start > end || start > r || end < l)
        return;

    if (l <= start && end <= r) { // 当前线段完全在更新区间内
        // 将'val'加到此节点的有效值,并标记子节点进行懒惰更新
        tree[node] += val; // 如果是叶子节点或内部节点存储组合值,直接应用
        if (start != end) { // 如果不是叶子节点,将'val'传递给子节点的懒惰标记
            lazy[2 * node] += val;
            lazy[2 * node + 1] += val;
        }
        return;
    }

    // 部分重叠：递归处理子节点
    int mid = (start + end) / 2;
    update_range(2 * node, start, mid, l, r, val);
    update_range(2 * node + 1, mid + 1, end, l, r, val);
    // 对于点查询结构,不需要从子节点更新tree[node]
}

// 查询海拔A[idx]
long long query_point(int node, int start, int end, int idx) {
    push(node, start, end); // 应用待处理的懒惰更新以确保当前节点的值是最新的

    if (start == end) // 叶子节点,找到了元素
        return tree[node];

    int mid = (start + end) / 2;
    if (idx <= mid) // 在左子树中查询
        return query_point(2 * node, start, mid, idx);
    else // 在右子树中查询
        return query_point(2 * node + 1, mid + 1, end, idx);
}

// 计算两个地点之间的温度变化
long long calculate_segment_temp_change(long long alt1, long long alt2, int S_uphill_factor, int T_downhill_factor) {
    if (alt2 > alt1) // 上坡：海拔升高,温度下降
        return -(alt2 - alt1) * S_uphill_factor;
    else if (alt2 < alt1) // 下坡：海拔降低,温度上升
        return (alt1 - alt2) * T_downhill_factor;
    else // 平地：海拔不变,温度不变
        return 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N_problem, Q_queries, S_uphill, T_downhill; // N_problem是地点的最大索引（0到N）
    cin >> N_problem >> Q_queries >> S_uphill >> T_downhill;

    array_size_N = N_problem; // 海拔数组的最大索引是N_problem

    // 读取初始海拔数组
    vector<long long> A_initial(array_size_N + 1);
    for (int i = 0; i <= array_size_N; ++i)
        cin >> A_initial[i];

    // 线段树大小通常是4 * (元素数量)
    tree.resize(4 * (array_size_N + 1));
    lazy.resize(4 * (array_size_N + 1));
    build(1, 0, array_size_N, A_initial); // 为索引0到array_size_N构建线段树

    // 使用初始海拔计算初始总温度
    long long total_temperature = 0;
    for (int i = 0; i < array_size_N; ++i) // 线段是(A[i], A[i+1]),从(A[0], A[1])到(A[N-1], A[N])
        total_temperature += calculate_segment_temp_change(A_initial[i], A_initial[i+1], S_uphill, T_downhill);

    // 处理Q次地壳运动查询
    for (int q_idx = 0; q_idx < Q_queries; ++q_idx) {
        int Lj_input, Rj_input, Xj_change;
        cin >> Lj_input >> Rj_input >> Xj_change;
        // Lj_input, Rj_input在问题描述中是1-based的地点1..N
        // 这里我们假设它们是直接的0-indexed数组索引,按照样例和典型竞赛编程的解释
        // 其中"地点k"意味着A[k],约束1<=Lj<=Rj<=N意味着A[0]不直接受Xj影响

        // 边界1：连接(A[Lj-1], A[Lj])
        // 当A[Lj]改变时,这个连接会受到影响
        if (Lj_input - 1 >= 0) { // 检查Lj-1是否是有效索引（A[0]或更大）
            int prev_loc_idx = Lj_input - 1;
            int curr_loc_idx_L = Lj_input;

            long long alt_prev = query_point(1, 0, array_size_N, prev_loc_idx);
            long long alt_curr_L_old = query_point(1, 0, array_size_N, curr_loc_idx_L);

            // 计算旧的温度贡献和新的温度贡献
            long long old_contrib = calculate_segment_temp_change(alt_prev, alt_curr_L_old, S_uphill, T_downhill);
            long long alt_curr_L_new = alt_curr_L_old + Xj_change;
            long long new_contrib = calculate_segment_temp_change(alt_prev, alt_curr_L_new, S_uphill, T_downhill);

            total_temperature += (new_contrib - old_contrib);
        }

        // 边界2：连接(A[Rj], A[Rj+1])
        // 当A[Rj]改变时,这个连接会受到影响。A[Rj+1]不会因为A[Rj]的Xj_change而改变
        if (Rj_input + 1 <= array_size_N) { // 检查Rj+1是否是有效索引（直到A[N]）
            int curr_loc_idx_R = Rj_input;
            int next_loc_idx = Rj_input + 1;

            long long alt_curr_R_old = query_point(1, 0, array_size_N, curr_loc_idx_R);
            long long alt_next = query_point(1, 0, array_size_N, next_loc_idx);

            // 计算旧的温度贡献和新的温度贡献
            long long old_contrib = calculate_segment_temp_change(alt_curr_R_old, alt_next, S_uphill, T_downhill);
            long long alt_curr_R_new = alt_curr_R_old + Xj_change;
            long long new_contrib = calculate_segment_temp_change(alt_curr_R_new, alt_next, S_uphill, T_downhill);

            total_temperature += (new_contrib - old_contrib);
        }

        // 在线段树上执行实际的海拔更新
        update_range(1, 0, array_size_N, Lj_input, Rj_input, Xj_change);

        cout << total_temperature << "\n";
    }

    return 0;
}
/*
3. 焚风现象
【问题描述】

焚风是是由于空气作绝热下沉运动时，因温度升高湿度降低而形成的一种干热风。焚风常出现在山脉背风坡，由山地引发的过山气流在背风坡下沉，使过山气流变得干热的一种风。在高压区，空气下沉也可产生焚风。
IOI 王国永远刮着海风。风从地点 0 依次吹到地点 1，地点 2 ……直到地点 N，共 N+1 个地点。JOI 君住在地点 N。地点 0 的海拔 A0=0，地点 i 的海拔为 Ai。
地表风的温度随海拔升降而变化。地点 0 在海边，温度为 0 度；对于任一地点 i，从地点 i 吹到地点 i+1 的风的温差仅取决于两地的海拔差。具体来说：
如果 A[i]=A[i+1]，风的温度不变；
如果 A[i]<A[i+1]，风每爬升 1 米，温度就会下降 S 度；
如果 A[i]> A[i+1]，风每下沉 1 米，温度就会升高 T 度。
IOI 国的地壳运动很强烈。你得到了 Q 天来地壳运动的数据。在第 j 日，地点 Lj , ... , Rj 的海拔升高了 Xj，注意 Xj 可能是负数。
你的任务是，计算每天地壳运动后 JOI 君住所的温度。
【输入形式】

第一行有四个整数 N, Q, S, T，用空格分隔。
在接下来的 N+1 行中，第 i 行 (1 ≤ i ≤ N+1) 有一个整数 Ai-1。
在接下来的 Q 行中，第 j 行 (1 ≤ j ≤ Q) 有三个整数 Lj, Rj, Xj，用空格分隔。
输入的所有数的含义见题目描述。
数据保证

    1≤N, Q≤2*10⁵, 1≤S, T≤10⁶;

    A0=0, |Ai|≤10⁶(1≤i≤N); 1≤Lj≤Rj≤N, |Xj|≤10⁶ (1≤j≤Q)。

【输出形式】

共 Q 行，第 j 行 (1 ≤ j ≤ Q) 有一个整数，表示第 j 日地壳运动后 JOI 君住所的温度。

【样例输入1】

3 5 1 2
0
4
1
8
1 2 2
1 1 -2
2 3 5
1 2 -1
1 3 5
【样例输出1】


-5
-7
-13
-13
-18
【样例输入2】



2 2 5 5
0
6
-1
1 1 4
1 2 8
【样例输出2】



5
-35
【样例输入3】



7 8 8 13
0
4
-9
4
-2
3
10
-9
1 4 8
3 5 -2
3 3 9
1 7 4
3 5 -1
5 6 3
4 4 9
6 7 -10
【样例输出3】


277
277
322
290
290
290
290
370
【样例1说明】

最初，地点 0,1,2,3 的海拔分别是 0,4,1,8。第一天地壳运动后，海拔分别为 0,6,3,8。 此时，风的温度分别为 0,-6,0,-5。
 */