#include <iostream>
#include <vector>
#include <algorithm>
#include <queue> // For BFS propagation
using namespace std;
// 全局变量，方便在函数间共享
vector<vector<int>> adj;      // 邻接表 (正向图)
vector<vector<int>> rev_adj;  // 反向图的邻接表 (用于路径计数)
vector<int> ans;                   // 存储最终答案 (-1, 0, 1, 2)

// 用于第一次DFS (检测环和无限路径)
vector<bool> visited_cycles_dfs; // 标记节点是否在dfs_cycles中被访问过 (即从1可达)
vector<bool> on_stack_dfs;       // 标记节点是否在当前dfs_cycles的递归栈中

// 用于第二次DFS (计数有限路径)
vector<int> dp_paths;             // dp_paths[u] 存储从节点1到节点u的路径数 (0, 1, 或 2表示>1)
vector<bool> memo_count_dfs;     // 标记节点u的dp_paths值是否已计算

/**
 * @brief 第一次DFS: 检测从节点1可达的、且位于环上的节点。
 * 如果节点u在环上，则将ans[u]标记为-1。
 * @param u 当前访问的节点
 */
void dfs_mark_on_cycle(int u) {
    visited_cycles_dfs[u] = true; // 标记u已被本次DFS访问 (即从1可达)
    on_stack_dfs[u] = true;       // 标记u进入当前递归栈

    for (int v : adj[u]) {
        // 如果v已经被标记为-1（例如，它是另一个环的一部分），则无需进一步处理通过这条边形成的环，
        // 因为v及其后续节点将通过下游传播被处理。
        // if (ans[v] == -1) continue; // Optimization: can sometimes be useful but review impact

        if (on_stack_dfs[v]) { // 如果v已在递归栈中，说明找到了一个环 (u -> ... -> v -> ... -> u)
            // v是环上的一个节点 (也是u的一个祖先)。
            ans[v] = -1;
            // u 也是这个环的一部分 (因为它直接连接到在栈上的v)
            // ans[u] = -1; // u 也会被标记，因为它连接到环上的v。
                // 实际上，如果 v 被标记为 -1，那么如果 u->v 是 u 到环的唯一路径，下游传播将会处理 u。
                // 目前，仅仅标记栈上的祖先 v 是直接的查找方式。
                // 问题描述的例子 (1->3, 1->4, 3->4, 4->5, 5->5) 暗示着
                // 只有节点 5 (在环 5->5 上) 初始时会被标记为 -1。
                // 因此，标记栈上*作为回边目标*的节点。
        } else if (!visited_cycles_dfs[v]) { // 如果v未被访问过
            dfs_mark_on_cycle(v); // 递归访问v
        }
        // 重要：移除了原先的向上层叠的-1标记逻辑: if (ans[v] == -1) { ans[u] = -1; }
        // 现在只标记环上被回指的节点。下游传播将处理其他情况。
    }
    on_stack_dfs[u] = false; // u退出当前递归栈
}


/**
 * @brief 第二次DFS: 计算从节点1到节点u的有限路径数量 (0, 1, 或 2表示多于1条)。
 * 使用记忆化搜索，并只考虑不通向无限路径的节点。
 * @param u 当前需要计算路径数的节点
 * @return 从节点1到节点u的路径数 (0, 1, 或 2)
 */
int count_paths(int u) {
    // 如果节点u已经被标记为有无限路径，则它对有限路径计数没有贡献
    if (ans[u] == -1) {
        return 0;
    }
    // 如果节点u的路径数已计算过，直接返回结果
    if (memo_count_dfs[u]) {
        return dp_paths[u];
    }

    memo_count_dfs[u] = true; // 标记为正在计算/已计算
    int current_path_sum = 0;

    if (u == 1) { // 基准情况：节点1到自身的路径（空路径）算作1条
        current_path_sum = 1;
    } else {
        // 遍历u的所有前驱节点p (在反向图中p是u的邻居)
        for (int p : rev_adj[u]) {
            if (ans[p] == -1) continue; // 跳过那些通向无限路径的前驱节点

            int paths_from_p = count_paths(p); // 递归计算从1到p的路径数

            // 将从p来的路径数累加到current_path_sum，并确保结果不超过2
            current_path_sum += paths_from_p;
            if (current_path_sum > 2) { // 题目要求区分0, 1, >1(用2表示)
                current_path_sum = 2;
            }
            // 优化：如果路径数已经达到2，则无需继续累加
            if (current_path_sum == 2) break;
        }
    }

    dp_paths[u] = current_path_sum; // 存储计算结果
    return dp_paths[u];
}

// 处理单个测试用例
void solve_test_case() {
    int n, m; // n: 顶点数, m: 边数
    cin >> n >> m;

    // 初始化数据结构
    adj.assign(n + 1, vector<int>());
    rev_adj.assign(n + 1, vector<int>());
    ans.assign(n + 1, 0); // 默认答案为0 (不可达或0条路径)

    visited_cycles_dfs.assign(n + 1, false); // Tracks reachability from 1 for the first DFS pass
    on_stack_dfs.assign(n + 1, false);

    dp_paths.assign(n + 1, 0);
    memo_count_dfs.assign(n + 1, false);

    // 读取边的信息，构建邻接表和反向邻接表
    for (int k = 0; k < m; ++k) {
        int u_node, v_node;
        cin >> u_node >> v_node;
        adj[u_node].push_back(v_node);
        rev_adj[v_node].push_back(u_node);
    }

    // 步骤1: 从节点1开始DFS，标记那些直接位于从1可达的环上的节点为 -1
    if (n > 0) {
        dfs_mark_on_cycle(1);
    }

    // 步骤2: 向下游传播 -1 标记
    // 如果节点x被标记为-1 (在环上)，那么所有从x可达的节点y也应标记为-1
    if (n > 0) {
        queue<int> q_prop;
        for(int i = 1; i <= n; ++i) {
            if(ans[i] == -1) {
                q_prop.push(i);
            }
        }
        while(!q_prop.empty()){
            int curr = q_prop.front();
            q_prop.pop();

            for(int neighbor : adj[curr]){
                if(ans[neighbor] != -1){ // 如果邻居尚未标记为-1
                    ans[neighbor] = -1;
                    q_prop.push(neighbor); // 将其加入队列以进一步传播
                    visited_cycles_dfs[neighbor] = true; // Mark as "handled" by -1 logic
                }
            }
        }
    }

    // 步骤3: 对于未被标记为-1的节点，计算从节点1到它们的有限路径数
    for (int i = 1; i <= n; ++i) {
        if (ans[i] != -1) { // 只为没有无限路径的节点计算有限路径
             // count_paths 会被递归调用，填充 dp_paths
        }
    }

    // 步骤4: 根据dfs_cycles(visited_cycles_dfs for reachability)和count_paths的结果，整理最终的ans数组
    for (int i = 1; i <= n; ++i) {
        if (ans[i] == -1) {
            // 如果已标记为-1，则保持-1
        } else if (!visited_cycles_dfs[i] && i != 1 && n > 0) {
            // 如果节点i (非节点1) 未被dfs_mark_on_cycle(1)访问到，则它从节点1不可达
            ans[i] = 0;
        } else {
            // 如果可达且未被标记为-1，则其路径数由count_paths(i)决定
            ans[i] = count_paths(i); // This will use memoized dp_paths values
        }
    }

    // 输出当前测试用例的结果
    for (int i = 1; i <= n; ++i) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << "\n";
}

int main() {
    // 优化C++标准IO流速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t; // 测试用例数量
    cin >> t;
    while (t--) {
        solve_test_case();
    }
    return 0;
}

/*
【问题描述】

       给定一个有向图 G ，可以包含环，也可以有自环（节点连接到自身）,一对顶点之间最多有一条边相连。顶点编号从1~n 。

       从顶点 u 到 v 的路径是一个边的序列，描述如下

顶点 u 是路径上第一条边的开始顶点

顶点 v 是路径上最后一条边的结束顶点

对于所有相邻边对，后一条边的开始顶点是前一条边的结束顶点

       假定边的空序列是从 u 到 u 的路径。

       对于每个顶点 v ，输出以下四个值之一：

0，表示从顶点 1 到 v 没有路径

1，表示从顶点 1 到 v 只有一条路径

2，表示从顶点 1 到 v 有超过一条路径，且路径数是有限的

-1，表示从顶点 1 到 v 有无穷多条路径

       示例如下

图片.png

       那么

顶点1的结果为1：从1到1只有一条路径（路径长度为0）

顶点2的结果为0：从1到2没有路径

顶点3的结果为1：从1到3仅有一条路径（为边（1,3））

顶点4的结果为2：从1到4有超过一条路径，但路径数量是有限的（两条路径：【（1,3），（3,4）】和【（1,4）】）

顶点5的结果为-1：从1到5的路径数是无穷的（环可以用于路径无穷多次）

顶点6的结果为-1：从1到6的路径数是无穷的（环可以用于路径无穷多次）



【输入形式】



       输入的第一行为一个整数 t （1≤ t ≤104），表示测试用例的组数，接下来是 t 个测试用例，每个测试用例前有一个空行，这个空行的作用仅为阅读方便，有否不影响最终结果。



      每个测试用例的第一行为两个整数 n 和 m (1≤ n ≤ 4×105，1≤ m ≤ 4×105)，分别表示图的顶点数和边数。接下来的 m 行包括了对边的描述，每行两个整数 ai、bi (1≤ ai、bi  ≤ n)，表示第 i 边的开始点和结束点，图的顶点编号从1至 n ，给定的图中可以包含环（可能有ai =bi）, 但不可以包含多边（即对于 i ≠ j，不可能有 ai = aj 以及 bi = bj）。



       所有测试用例中 n 之和不超过4×104，同样的，所有的 m 之和不超过4×104。



【输出形式】

         输出包括 t 行，第 i 行是第 i 个测试用例的答案，为一个 n 个整数序列，取值在-1到2之间。

【样例输入】

5

6 7
1 4
1 3
3 4
4 5
2 1
5 5
5 6

1 0

3 3
1 2
2 3
3 1

5 0

4 4
1 2
2 3
1 4
4 3
【样例输出】

1 0 1 2 -1 -1
1
-1 -1 -1
1 0 0 0 0
1 1 2 1
 */