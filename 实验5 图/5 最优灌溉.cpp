#include <iostream>
#include <vector>
#include <algorithm> // 用于 sort
#include <numeric>   // 用于 iota (可选, 用于初始化并查集)
using namespace std;

// 定义边的结构体
struct Edge {
    int u, v; // 边的两个端点 (麦田编号)
    int cost; // 边的费用

    // 重载小于运算符，方便对边按费用排序
    bool operator<(const Edge& other) const {
        return cost < other.cost;
    }
};

// 并查集 (Disjoint Set Union - DSU) 结构体
struct DSU {
    vector<int> parent; // 存储每个元素的父节点

    // 构造函数，初始化n个元素的并查集
    DSU(int n) {
        parent.resize(n + 1); // 麦田编号从1到n，所以大小为n+1
        // 初始化每个元素的父节点为它自身
        // iota(parent.begin() + 1, parent.end(), 1); // 如果用iota
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
    }

    // 查找元素i所在集合的代表元（根节点）
    // 同时进行路径压缩优化
    int find(int i) {
        if (parent[i] == i) {
            return i;
        }
        return parent[i] = find(parent[i]);
    }

    // 合并元素i和元素j所在的集合
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) { // 只有当它们不在同一集合时才合并
            parent[root_i] = root_j; // 将一个根指向另一个根
                                     // 更优化的合并可以使用按秩合并或按大小合并
        }
    }
};

int main() {
    // 优化C++标准库的输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_fields, m_canals; // n: 麦田数量, m: 可建水渠数量
    cin >> n_fields >> m_canals;

    vector<Edge> edges; // 存储所有可建的水渠
    for (int i = 0; i < m_canals; ++i) {
        int u, v, cost;
        cin >> u >> v >> cost;
        edges.push_back({u, v, cost});
    }

    // 1. 按费用对所有边进行升序排序
    sort(edges.begin(), edges.end());

    // 2. 初始化并查集
    DSU dsu(n_fields);

    long long total_min_cost = 0; // 存储最小总费用，使用long long以防费用过大
    int edges_in_mst = 0;       // 记录已加入最小生成树的边的数量

    // 3. 遍历排序后的边，构建最小生成树
    for (const Edge& edge : edges) {
        // 如果边的两个端点不在同一个连通分量中
        if (dsu.find(edge.u) != dsu.find(edge.v)) {
            dsu.unite(edge.u, edge.v);      // 合并这两个连通分量
            total_min_cost += edge.cost;    // 将当前边的费用加入总费用
            edges_in_mst++;                 // 已选边数加1

            // 优化：当已选边数达到 n-1 时，最小生成树已构建完成
            if (edges_in_mst == n_fields - 1) {
                break;
            }
        }
    }

    // 检查是否所有麦田都被连接 (对于此题，通常假定可以连接所有麦田)
    // 如果 edges_in_mst < n_fields - 1 且 n_fields > 0，则说明图不连通，
    // 无法灌溉所有麦田。但题目通常保证有解。

    // 输出结果
    cout << total_min_cost << endl;

    return 0;
}
/*
*
【问题描述】

    雷雷承包了很多片麦田，为了灌溉这些麦田，雷雷在第一个麦田挖了一口很深的水井，所有的麦田都从这口井来引水灌溉。
  　　为了灌溉，雷雷需要建立一些水渠，以连接水井和麦田，雷雷也可以利用部分麦田作为“中转站”，利用水渠连接不同的麦田，这样只要一片麦田能被灌溉，则与其连接的麦田也能被灌溉。
  　　现在雷雷知道哪些麦田之间可以建设水渠和建设每个水渠所需要的费用（注意不是所有麦田之间都可以建立水渠）。请问灌溉所有麦田最少需要多少费用来修建水渠。

【输入形式】

输入的第一行包含两个正整数n, m，分别表示麦田的片数和雷雷可以建立的水渠的数量。麦田使用1, 2, 3, ……依次标号。
  　　接下来m行，每行包含三个整数ai, bi, ci，表示第ai片麦田与第bi片麦田之间可以建立一条水渠，所需要的费用为ci。

【输出形式】

输出一行，包含一个整数，表示灌溉所有麦田所需要的最小费用。

【样例输入】

4 4
1 2 1
2 3 4
2 4 2
3 4 3
【样例输出】

6
【样例说明】

建立以下三条水渠：麦田1与麦田2、麦田2与麦田4、麦田4与麦田3。

【评分标准】

前20%的评测用例满足：n≤5。
  　　前40%的评测用例满足：n≤20。
  　　前60%的评测用例满足：n≤100。
  　　所有评测用例都满足：1≤n≤1000，1≤m≤100,000，1≤ci≤10,000。
* */