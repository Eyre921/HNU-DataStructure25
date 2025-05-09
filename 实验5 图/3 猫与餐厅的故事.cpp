#include <iostream>
#include <vector>
#include <numeric> // 可能会用到 iota 等，但此题不需要

// 使用标准命名空间
using namespace std;

// 全局变量存储树的邻接表、每个节点是否有猫、猫的数量上限以及最终答案
vector<vector<int>> adj;    // 树的邻接表
vector<int> has_cat_arr;    // has_cat_arr[i] = 1 表示节点 i 有猫, 0 表示没有
int m_limit;                // 阿迪能忍受的最大连续猫节点数
int count_restaurants = 0;  // 可到达的餐厅数量

/**
 * @brief 深度优先搜索函数
 * @param u 当前访问的节点
 * @param parent 当前节点 u 的父节点 (用于避免在树中往回走)
 * @param consecutive_cats 到达父节点 parent 时，路径末尾的连续猫节点数
 */
void dfs(int u, int parent, int consecutive_cats_on_path_to_parent) {
    // 1. 计算到达当前节点 u 后，路径末尾的连续猫节点数
    int current_node_consecutive_cats;
    if (has_cat_arr[u] == 1) { // 如果当前节点 u 有猫
        current_node_consecutive_cats = consecutive_cats_on_path_to_parent + 1;
    } else { // 当前节点 u 没有猫
        current_node_consecutive_cats = 0; // 连续猫序列中断
    }

    // 2. 检查是否超出猫的忍耐上限
    if (current_node_consecutive_cats > m_limit) {
        return; // 这条路径不安全，从此分支返回
    }

    // 3. 判断当前节点 u 是否为叶子节点，并递归访问子节点
    bool is_leaf = true; // 假设当前节点是叶子节点
    for (int v : adj[u]) { // 遍历 u 的所有邻居 v
        if (v == parent) { // 如果邻居 v 是父节点，则跳过
            continue;
        }
        // 如果 v 不是父节点，说明 v 是 u 的一个子节点
        is_leaf = false; // 那么 u 就不是叶子节点
        dfs(v, u, current_node_consecutive_cats); // 递归访问子节点 v
                                                  // 传递给子节点的连续猫数是到达当前节点 u 后的值
    }

    // 4. 如果是叶子节点并且路径安全，则增加餐厅计数
    if (is_leaf) {
        // 此时 u 是一个叶子节点（餐厅），并且从家到这里的路径是安全的
        count_restaurants++;
    }
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; // 树的顶点数
    cin >> n >> m_limit;

    // 初始化/调整容器大小 (节点编号从 1 到 n)
    has_cat_arr.resize(n + 1);
    adj.resize(n + 1);

    // 读取每个顶点是否有猫的信息
    for (int i = 1; i <= n; ++i) {
        cin >> has_cat_arr[i];
    }

    // 读取 n-1 条边，构建邻接表
    for (int i = 0; i < n - 1; ++i) {
        int u_node, v_node;
        cin >> u_node >> v_node;
        adj[u_node].push_back(v_node);
        adj[v_node].push_back(u_node); // 因为是无向边
    }

    // 从顶点 1 (阿迪的家) 开始 DFS
    // 初始父节点设为 0 (一个不存在的节点编号)
    // 初始路径上的连续猫数为 0 (因为还没开始走)
    dfs(1, 0, 0);

    // 输出结果
    cout << count_restaurants << endl;

    return 0;
}
/*
* 【问题描述】

      公司今天发薪，阿迪想与朋友们去餐厅庆祝一下。

       他住在一个非常神奇的公园里，这个公园是一个根在顶点1，且由 n 个顶点组成的有根树，顶点1也就是他的住所。然而不幸的是，公园也有许多的猫，阿迪已经找出了所有包含猫的顶点。

       公园的叶子顶点都有餐厅，阿迪想选择一家他可以去的餐厅，但很不幸，他非常害怕猫，因而如果从餐厅去往他家的路径上有连续包含猫的数量超过 m 时，他将不能去往这家餐厅。

       你的任务是帮助他确认他能去的餐厅的数量。

【输入形式】

       输入的第一行包含两个整数 n 和 m （2≤ n ≤ 105,  1≤ m ≤ n），分别表示树的顶点数以及对于阿迪来说可以忍受的最大的包含猫的连续顶点数。

        第二行包含 n 个整数 a1、a2、...、an，这里的每个 ai 或者为0（顶点 i 无猫），或者为1（顶点 i 有猫）。

       接下来的 n - 1 行包含用形式“xi  yi”（1≤ xi , yi ≤ n，xi ≠ yi）表示的树的边，表示顶点xi和顶点yi之间有边相连。

【输出形式】

       输出为一个整数，表示从阿迪家去往叶子顶点的路径上至多包含 m 只猫的叶子顶点的数量。

【样例输入1】

4 1
1 1 0 0
1 2
1 3
1 4
【样例输出1】

2
【样例输入2】

7 1
1 0 1 1 0 0 0
1 2
1 3
2 4
2 5
3 6
3 7
【样例输出2】

2
【样例说明】

很显然，树是具有个 n 顶点 n - 1 条边的连通图，有根树是有一个称为根的特殊顶点的树。

在样例一中

QQ图片20210630152715.png

包含猫的顶点变为红色，餐厅在顶点2、3、4，阿迪不能去到在顶点2的餐厅。

在样例二中

QQ图片20210630152926.png

餐厅在顶点4、5、6、7，阿迪不能去到6和7。
* */