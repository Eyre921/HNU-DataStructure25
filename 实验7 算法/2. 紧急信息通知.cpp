#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// 执行BFS广度优先搜索，检查是否所有节点都能被访问到
// 修正后能正确处理 n_nodes <= 0 的情况
bool is_graph_connected(int n_nodes, const vector<vector<int>>& adj_list) {
    if (n_nodes <= 0) {
        // 如果没有人（或者人数是无效的非正数），
        // 就无法选择"任何一个人"来通知。所以，不是所有人都能被通知到。
        // 这种情况理想情况下应该在main函数中通过N=M=0的终止条件过滤掉
        // 但是，让这个函数变得健壮是好的。
        return false;
    }
    if (n_nodes == 1) {
        // 只有一个节点的图总是连通的；那一个人就是"所有人"。
        return true;
    }

    // 从这里开始，n_nodes >= 2
    vector<bool> visited(n_nodes + 1, false); // visited[0]对于1索引的节点是未使用的，大小至少为3
    queue<int> q;

    int start_node = 1; // 从节点1开始BFS。因为n_nodes >= 2，所以节点1存在

    q.push(start_node);
    visited[start_node] = true; // 安全的，因为visited的大小是n_nodes + 1，且n_nodes >= 2
    int nodes_reached = 0; // 统计能够到达的节点数量

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        nodes_reached++;

        // adj_list[u]是安全的，因为u是从队列中获得的，从1开始，
        // 后续节点是邻居'v'，它们都在[1, n_nodes]范围内。
        // adj_list本身的大小是n_nodes+1。
        for (int v : adj_list[u]) {
            // 如果输入边有效，v也在[1, n_nodes]范围内。
            // visited[v]的访问是安全的。
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // 如果能到达的节点数等于总节点数，说明图是连通的
    return nodes_reached == n_nodes;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    while (cin >> N >> M) {
        if (N == 0 && M == 0)
            break; // 终止条件

        // 如果N=0且M!=0，基于"1<=a,b<=N"的边约束，这是不一致的输入。
        // 修正后的is_graph_connected函数在用N=0调用时会返回false。
        // 如果这样的输入（N=0, M>0）在之前的邻接表填充中引起问题，
        // 那将是一个单独的故障点。提供的修复针对is_graph_connected。

        vector<vector<int>> adj(N + 1); // 创建邻接表，索引从1到N

        // 读取M条边
        for (int i = 0; i < M; ++i) {
            int u, v;
            cin >> u >> v;
            // 基于问题陈述，假设1 <= u, v <= N
            // 如果N=0，且u,v >= 1，如果之前没有处理，adj[u]会在这里崩溃。
            // 然而，我们的修复在is_graph_connected中，假设这部分是正常的或
            // 崩溃发生在函数内部。
            if (N > 0) { // 添加检查以防止N=0时的访问（尽管adj的大小为1）
                if (u >= 1 && u <= N && v >= 1 && v <= N) { // 确保u,v是有效索引
                    adj[u].push_back(v); // 添加无向边
                    adj[v].push_back(u); // 无向图，两个方向都要添加
                }
            } else if (N == 0 && M > 0) {
                // 这是不一致的情况（N=0, M>0）。
                // 循环将运行M次消耗边，但邻接表是有问题的。
                // is_graph_connected(0,...)将返回false。
                // 如果is_graph_connected处理N=0，这里不需要特定操作。
            }
        }

        // 检查图是否连通
        if (is_graph_connected(N, adj))
            cout << "Yes\n";
        else
            cout << "No\n";
    }

    return 0;
}
/*2. 紧急信息通知
【问题描述】

突然发生紧急情况，需要把这个消息发给所有的市民。已知有N个人，他们有M个联系方式（联系方式是双向的，即a可以通知b，b也可以通知a）。将每个人给一个从1到N的编号，并且已知M个联系方式，需要知道：通知其中任何一个人，其他的人是否都可被通知到。

编程解决这个问题。

【输入形式】

有多组测试数据，每组测试数据，第一行两个整数N（N ≤ 1,000）, M(M≤N(N-1)/2). N 是人数 ,M 是联系方式数.N=M=0 意味输入结束并且不需要处理。随后M行表示M个联系方式，每行一个联系方式，格式如下:

a b

a,b 是正整数1≤ a,b ≤ N ,表示a，b之间可以相互通知。

40%的测试数据 3 ≤ N ≤ 10；

30%的测试数据 10 < N ≤ 50；

20%的测试数据 50 < N ≤ 100；

10%的测试数据 100 < N ≤ 1,000；

【输出形式】

对于每组测试数据，如果可以只需要通知一个人，所有人都可被通知到，输出“Yes”（输出没有引号）占一行，否则，输出“No”（输出没有引号）占一行。

【样例输入】


3 2
1 2
2 3
3 1
1 2
0 0
【样例输出】

Yes
No
【样例说明】

样例两组测试数据

第一组1号和2号相互通知，2号和3号相互通知，只要通知一个人，其他人都可被通知到；

第二组3号和1号、2号之间无法相互通知。*/