#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // 用于 sort
#include <set>       // 虽然最终没用set，但注释中提到了思路演变

// 使用标准命名空间
using namespace std;

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; // 树中节点的数量
    cin >> n;

    // 邻接表，用于存储树的结构。节点编号从 1 到 n。
    vector<vector<int>> adj(n + 1);
    // 读取 n-1 条边
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // 因为是无向边
    }

    // 待检验的顶点序列 a (1-indexed)
    vector<int> sequence_a(n + 1);
    // pos_in_sequence[v] 存储顶点 v 在序列 a 中的位置 (1-indexed)
    vector<int> pos_in_sequence(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> sequence_a[i];
        pos_in_sequence[sequence_a[i]] = i;
    }

    // 检查 BFS 序列的第一个顶点是否为 1
    if (sequence_a[1] != 1) {
        cout << "No" << endl;
        return 0;
    }

    // BFS 队列
    queue<int> q;
    q.push(1); // 从顶点 1 开始 BFS

    // visited[v] 标记顶点 v 是否已被访问
    vector<bool> visited(n + 1, false);
    visited[1] = true;

    // child_candidate_start_idx 指向序列 sequence_a 中当前父节点 u 的第一个潜在子节点的位置
    // 例如，如果 sequence_a[1] 是父节点，那么它的子节点应该从 sequence_a[2] 开始
    int child_candidate_start_idx = 2;

    // BFS 主循环
    while (!q.empty()) {
        int u = q.front(); // 当前处理的父节点
        q.pop();

        // 收集当前父节点 u 在邻接表中的所有未访问过的邻居 (即实际的子节点)
        vector<int> actual_children;
        for (int v : adj[u]) {
            if (!visited[v]) {
                actual_children.push_back(v);
            }
        }

        // 如果 u 没有未访问的子节点，则 actual_children 为空，下面的循环不会执行，是正确的

        // 根据这些实际子节点在输入序列 sequence_a 中的出现顺序对它们进行排序
        // 这是关键步骤：BFS 要求父节点的所有子节点在序列中形成一个连续块，
        // 并且这个块中子节点的相对顺序由 sequence_a 决定。
        sort(actual_children.begin(), actual_children.end(), [&](int node1, int node2) {
            // pos_in_sequence[nodeX] 给出 nodeX 在 sequence_a 中的索引
            // 索引小的排在前面
            return pos_in_sequence[node1] < pos_in_sequence[node2];
        });

        // 现在，将排序后的 actual_children 与 sequence_a 中对应的段进行比较
        for (int i = 0; i < int(actual_children.size()); ++i) {
            // 检查 sequence_a 中是否有足够的元素来匹配 actual_children
            if (child_candidate_start_idx + i > n) {
                // sequence_a 中的元素不够了，但 u 还有未匹配的子节点
                cout << "No" << endl;
                return 0;
            }

            int child_from_seq = sequence_a[child_candidate_start_idx + i]; // 从序列中取出的期望子节点
            int actual_child_node = actual_children[i]; // 实际的、按序列顺序排序的子节点

            // 如果序列中的子节点与实际的子节点不匹配
            if (child_from_seq != actual_child_node) {
                cout << "No" << endl;
                return 0;
            }

            // 如果这个子节点在序列中出现，但已经被访问过（例如通过其他路径，这在树的正确BFS中不应发生）
            // 或者它根本不是 u 的邻居（这个检查其实已经通过 actual_children 的构建隐式完成了）
            // 主要检查是 visited 状态，确保节点只被正确地加入一次队列
            if (visited[child_from_seq]) {
                // 这个检查理论上不应该触发，因为我们只从 adj[u] 中取 !visited 的节点
                // 但如果 sequence_a 本身有问题，比如重复节点或非邻接节点，这里可以作为额外的保障
                // 不过，主要靠 actual_children 和 sequence_a 的直接比较
                cout << "No" << endl;
                return 0;
            }

            visited[child_from_seq] = true; // 标记为已访问
            q.push(child_from_seq);         // 加入 BFS 队列，供后续处理
        }

        // 将 child_candidate_start_idx 向前移动，跳过刚刚处理过的子节点块
        child_candidate_start_idx += actual_children.size();
    }

    // 如果成功处理完所有节点 (child_candidate_start_idx 应该恰好指向序列末尾的下一个位置)
    // 并且队列为空，说明序列是一个有效的 BFS 序列
    // 对于树来说，如果 child_candidate_start_idx == n + 1，意味着所有 n 个节点都按 BFS 规则被序列消耗了
    if (child_candidate_start_idx == n + 1) {
        cout << "Yes" << endl;
    } else {
        // 可能情况：图不连通（题目保证是树，所以不会），或者序列提前结束/过长，
        // 或者序列中节点的父子关系不符合BFS规则（已在循环中处理）
        cout << "No" << endl;
    }

    return 0;
}
/*
*
* 【问题描述】

       在图的BFS（广度优先搜索）中，通常采用队列来保存当前顶点的邻接点，但对对应邻接点的存入顺序没有要求，因此对于一个图的BFS结果可以有多个，在本问题中，从顶点1开始，请验证一个给定的顶点序列是否为一个有效的BFS序列？

【输入形式】

       输入的第一行为一个整数 n (1≤ n ≤ 2 ×105)，表示树中节点的数量。

       接下来 n-1 行描述了树的边，每行包含两个整数 x 和 y (1 ≤ x ,  y ≤ n)，表示对应边的两个端点，输入保证给定的图构成一颗树。

       最后一行为 n 个互不相同的整数 a1、a2、...、an( 1 ≤ ai ≤ n)，代表待检验的顶点序列。

【输出形式】

       如果待检验的序列是一个正确的BFS序列，输出"Yes"，否则输出"No"。

【样例输入1】

4
1 2
1 3
2 4
1 2 3 4
【样例输出1】

Yes
【样例输入2】

4
1 2
1 3
2 4
1 2 4 3
【样例输出2】

No
*/