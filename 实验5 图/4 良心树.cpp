#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>

// 使用标准命名空间
using namespace std;

// 全局变量
vector<int> parent_of;         // parent_of[i] 存储节点 i 的父节点
vector<int> respect_val;       // respect_val[i] = 1 表示节点 i 不尊重祖先, 0 表示尊重
vector<set<int>> children_of;  // children_of[i] 存储节点 i 的当前子节点集合
vector<bool> is_deleted;       // is_deleted[i] = true 表示节点 i 已被删除
int root_node_id = -1;         // 树的根节点 ID

vector<int> num_respectful_children; // num_respectful_children[i] 存储节点 i 的子节点中 c_j=0 的数量
set<int> current_deletable_candidates; // 当前满足删除条件的候选节点 (按 ID 排序)

// 更新节点是否满足删除条件的函数
void update_candidacy(int u_node) {
    // 根节点、已删除节点或无效节点(如-1父节点)不参与候选
    if (u_node == -1 || (root_node_id != -1 && u_node == root_node_id) || is_deleted[u_node]) {
        return;
    }

    // 条件: c_i=1 (不尊重父/祖先), 且所有孩子 c_j=1 (即 num_respectful_children[i]==0)
    bool can_be_candidate = (respect_val[u_node] == 1 && num_respectful_children[u_node] == 0);

    if (can_be_candidate) {
        current_deletable_candidates.insert(u_node);
    } else {
        current_deletable_candidates.erase(u_node); // 如果存在则移除，不存在不影响
    }
}


int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n; // 树的顶点数
    cin >> n;

    // 初始化容器 (节点编号从 1 到 n)
    parent_of.resize(n + 1);
    respect_val.resize(n + 1);
    children_of.resize(n + 1);
    is_deleted.resize(n + 1, false);
    num_respectful_children.resize(n + 1, 0);

    // 读取树的结构和每个节点的 c_i 值
    for (int i = 1; i <= n; ++i) {
        int p, c;
        cin >> p >> c;
        parent_of[i] = p;
        respect_val[i] = c;
        if (p == -1) {
            root_node_id = i;
        } else {
            // 确保父节点 p 是有效的 (在1到n之间)
            if (p >= 1 && p <=n) {
                 children_of[p].insert(i);
            }
        }
    }

    // 初始化 num_respectful_children 计数
    // 遍历所有节点 u，计算其初始的“尊重”子节点数量
    for (int u = 1; u <= n; ++u) {
        for (int child_node : children_of[u]) { // children_of[u] 是 u 的初始子节点集合
            if (respect_val[child_node] == 0) { // 如果子节点是“尊重”的
                num_respectful_children[u]++;
            }
        }
    }

    // 初始化第一批候选删除节点
    for (int i = 1; i <= n; ++i) {
        if (i == root_node_id) continue; // 根节点不能删除
        update_candidacy(i); // 使用辅助函数来决定是否加入候选集合
    }

    vector<int> deleted_sequence_output; // 存储删除的节点顺序

    while (!current_deletable_candidates.empty()) {
        int v_to_delete = *current_deletable_candidates.begin(); // 选择 ID 最小的候选节点
        current_deletable_candidates.erase(current_deletable_candidates.begin()); // 从候选中移除

        // 如果节点在被选作删除之前因为其他操作（理论上不应发生）而被标记为已删除，则跳过
        if(is_deleted[v_to_delete]) continue;

        deleted_sequence_output.push_back(v_to_delete);
        is_deleted[v_to_delete] = true;

        int p_of_v_deleted = parent_of[v_to_delete];
        // 复制一份原始子节点集合，因为 children_of[v_to_delete] 稍后会被清空或修改
        set<int> original_children_of_v_deleted = children_of[v_to_delete];

        // --- 更新父节点 p_of_v_deleted 的状态 ---
        // 父节点必须是有效节点ID，且未被删除，才有意义去更新它的子节点列表和候选状态
        if (p_of_v_deleted != -1 && !is_deleted[p_of_v_deleted]) {

            // 1. v_to_delete 从 p_of_v_deleted 的子节点中移除
            children_of[p_of_v_deleted].erase(v_to_delete);
            if (respect_val[v_to_delete] == 0) { // 如果被删除的 v_to_delete 是一个“尊重”的子节点
                num_respectful_children[p_of_v_deleted]--;
            }

            // 2. v_to_delete 的子节点成为 p_of_v_deleted 的新子节点
            for (int child_node : original_children_of_v_deleted) {
                if (is_deleted[child_node]) continue; // 跳过那些可能已被删除的子节点

                parent_of[child_node] = p_of_v_deleted; // 更新这些子节点的父指针
                children_of[p_of_v_deleted].insert(child_node); // 添加到新父节点的子节点列表

                if (respect_val[child_node] == 0) { // 如果这个新加入的子节点是“尊重”的
                    num_respectful_children[p_of_v_deleted]++;
                }
            }
            // 在所有结构和计数更改后，更新 p_of_v_deleted 的候选状态
            update_candidacy(p_of_v_deleted);
        } else {
            // 如果 v_to_delete 的父节点是根的标记(-1)或者父节点本身已被删除
            // 它的子节点将直接更新父指针。它们不会被添加到已删除父节点的 children_of 列表。
            for (int child_node : original_children_of_v_deleted) {
                if (is_deleted[child_node]) continue;
                parent_of[child_node] = p_of_v_deleted;
                // 这些子节点实际上变成了孤儿或连接到一个概念上的点（如果p_of_v_deleted是-1）
                // 或者连接到一个已删除的节点。它们不会成为任何 *活跃* 节点的子节点，
                // 因此不会影响任何活跃节点的 num_respectful_children 计数。
            }
        }
        children_of[v_to_delete].clear(); // 被删除的节点不再拥有子节点
    }


    if (deleted_sequence_output.empty()) {
        cout << -1 << endl;
    } else {
        for (size_t i = 0; i < deleted_sequence_output.size(); ++i) {
            cout << deleted_sequence_output[i] << (i == deleted_sequence_output.size() - 1 ? "" : " ");
        }
        cout << endl;
    }

    return 0;
}
/*【问题描述】

给定一颗有根树，顶点编号为1~n，树是一个无环的连通图，有根树有一个特定的顶点，称为根。

顶点 i 的祖先是从根到顶点 i 的路径上除顶点 i 以外的所有顶点，顶点 i 的父母是 i 的祖先中最接近 i 的顶点，每个顶点都是它父母的孩子。在给定的树中，顶点 i 的父母是顶点pi，对于根，pi为-1。例如：

QQ图片20210629145621.png

这是一个n=8个顶点的树，根为5， 顶点2的父母为3，顶点1的父母为5,6的祖先为4和5，7的祖先为8、3和5。

在树中，其中一些顶点不尊重其他一些顶点，实际上，如果ci =1，表示顶点 i 不尊重它的所有祖先，而如果ci =0，则表示它尊重它所有的祖先。

你需要一个一个地删除一些顶点，在每一步中，选择一个非根顶点，它不尊重它的父母并且它的所有孩子顶点也不尊重它。如果有几个这样的顶点，你需要选择具有最小编号的顶点。当你删除了这样的一个顶点v , 则v 的所有子顶点与v 的父母顶点相连。

QQ图片20210629150825.png

上图是删除顶点7的示例。

直到树中无满足删除标准的顶点，则上述过程停止。按顺序输出你删除的所有顶点，注意这个顺序的唯一的。

【输入形式】

输入的第一行为一个整数 n (1≤ n ≤105)，表示树的顶点数。

接下来的 n 行描述了整颗树：第 i 行包含两个整数 pi 和 ci (1≤ pi ≤ n,  0≤  ci ≤1)，这里 pi 是顶点i 的父母，若ci=0，表示顶点 i 尊重它的父母，ci=1，表示顶点 i 不尊重它的父母，pi=-1时，表示顶点 i 是树的根，同时 ci=0。

【输出形式】

如果树中至少有一个顶点被删除，则按照顺序输出顶点编号，否则输入-1。

【样例输入1】

5
3 1
1 1
-1 0
2 1
3 0
【样例输出1】

1 2 4
【样例输入2】

5
-1 0
1 1
1 1
2 0
3 0
【样例输出2】

-1
【样例输入3】

8
2 1
-1 0
1 0
1 1
1 1
4 0
5 1
7 0
【样例输出3】

5
【样例说明】
第一个样例的删除过程如下（在图中，ci=1的顶点是黄色的）

首先删除顶点1，因为它不尊重祖先并且它的所有孩子也不尊重它，而1是这样的顶点中编号最小的

删除后顶点2将连接到顶点3

然后删除顶点2，因为它不尊重祖先并且它的所有孩子也不尊重它。

顶点4将连接到顶点3

然后删除顶点4，因为它不尊重祖先，并且它的所有孩子也不尊重它（无孩子）

无更多顶点可删

QQ图片20210629153821.png

在第二个样例中，无需删除顶点

顶点2和3的孩子尊重它们

顶点4和5尊重它们的祖先

QQ图片20210629154127.png

在第三个样例中显示如下

*/