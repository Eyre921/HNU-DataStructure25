#include <iostream>
#include <vector>
#include <queue>
#include <limits>     // 用于 numeric_limits，获取类型的最大值
#include <algorithm>  // 用于 min 函数

// 为了方便，全局使用 long long 类型来存储成本和距离，以防止溢出
using namespace std;

// 全局定义 INF (无穷大) 常量，表示不可达
const long long INF = numeric_limits<long long>::max();

// Dijkstra 算法函数，用于找到从单个源点出发到所有其他单元格的最短路径
// R, C: 网格的行数和列数
// W_cost: 在相邻单元格之间移动的成本
// grid_cell_values: 存储网格单元的值，包括传送门的成本 (a_ij)，空地 (0)，或障碍物 (-1)
// start_r, start_c: 起始单元格的坐标 (0-indexed)
vector<vector<long long>> run_dijkstra(int start_r, int start_c, int R, int C, long long W_cost, const vector<vector<int>>& grid_cell_values) {
    // dist[r][c] 存储从起点到单元格 (r,c) 的最短距离
    vector<vector<long long>> dist(R, vector<long long>(C, INF));

    // 优先队列 (最小堆) 用于 Dijkstra 算法
    // 存储元素格式: {当前路径成本, {行坐标, 列坐标}}
    // 使用 pair 来存储坐标: {row, col}
    priority_queue<pair<long long, pair<int, int>>,  // 元素类型
                   vector<pair<long long, pair<int, int>>>, // 底层容器
                   greater<pair<long long, pair<int, int>>>> pq; // 比较器，使其成为最小堆

    // 检查起点是否有效 (题目保证起点和终点是空闲的，但这是一个通用的好习惯)
    if (start_r < 0 || start_r >= R || start_c < 0 || start_c >= C || grid_cell_values[start_r][start_c] == -1) {
        // 如果起点无效，直接返回充满 INF 的距离数组
        return dist;
    }

    // 起点到自身的距离为 0
    dist[start_r][start_c] = 0;
    // 将起点加入优先队列
    pq.push({0, {start_r, start_c}});

    // 定义在网格中移动的四个方向：上, 下, 左, 右
    int dr[] = {-1, 1, 0, 0}; // 行坐标变化量
    int dc[] = {0, 0, -1, 1}; // 列坐标变化量

    // Dijkstra 主循环
    while (!pq.empty()) {
        // 取出当前队列中路径成本最小的单元格
        long long current_dist = pq.top().first;    // 到达该单元格的当前已知最短距离
        pair<int, int> current_cell = pq.top().second; // 该单元格的坐标
        int r = current_cell.first;  // 当前行
        int c = current_cell.second; // 当前列
        pq.pop(); // 从队列中移除

        // 优化：如果当前取出的路径成本已经大于之前记录的到该点的最短路径成本，
        // 说明这是一个过时的条目 (因为我们可能已经通过其他路径找到了更短的到达 (r,c) 的方式)，跳过处理
        if (current_dist > dist[r][c]) {
            continue;
        }

        // 探索当前单元格的所有相邻单元格
        for (int i = 0; i < 4; ++i) { // 遍历四个方向
            int nr = r + dr[i]; // 相邻单元格的行坐标
            int nc = c + dc[i]; // 相邻单元格的列坐标

            // 检查相邻单元格是否在网格边界内，并且不是障碍物 (-1)
            if (nr >= 0 && nr < R && nc >= 0 && nc < C && grid_cell_values[nr][nc] != -1) {
                // 如果通过当前单元格 (r,c) 到达相邻单元格 (nr,nc) 的路径更短
                // (即 dist[r][c] + W_cost < dist[nr][nc])
                if (dist[nr][nc] > current_dist + W_cost) {
                    // 更新到相邻单元格 (nr,nc) 的最短距离
                    dist[nr][nc] = current_dist + W_cost;
                    // 将更新后的路径信息加入优先队列
                    pq.push({dist[nr][nc], {nr, nc}});
                }
            }
        }
    }
    // 返回从起点到所有其他单元格的最短距离数组
    return dist;
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_rows, m_cols;         // n: 网格行数, m: 网格列数
    long long move_w_cost;      // w: 相邻单元格移动成本
    cin >> n_rows >> m_cols >> move_w_cost;

    // grid_values[i][j] 存储单元格 (i,j) 的描述信息
    // -1: 障碍物
    // 0: 空闲单元格
    // x > 0: 包含时空通道，成本为 x
    vector<vector<int>> grid_values(n_rows, vector<int>(m_cols));

    // portal_coords 存储所有时空通道单元格的坐标 {行, 列}
    vector<pair<int, int>> portal_coords;

    // 读取网格信息
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            cin >> grid_values[i][j];
            // 如果单元格包含时空通道 (其值 > 0)
            if (grid_values[i][j] > 0) {
                portal_coords.push_back({i, j});
            }
        }
    }

    // 计算从起点 (0,0) （对应题目中的 (1,1)）出发，仅通过步行到达图中所有其他单元格的最短时间
    // 注意：输入的坐标是 1-indexed，程序内部使用 0-indexed
    vector<vector<long long>> dist_from_start_node = run_dijkstra(0, 0, n_rows, m_cols, move_w_cost, grid_values);

    // 计算从终点 (n_rows-1, m_cols-1) （对应题目中的 (n,m)）出发，仅通过步行到达图中所有其他单元格的最短时间
    // 由于步行成本是双向相同的 (图是无向的)，这等价于从任意单元格 (i,j) 步行到终点 (n_rows-1, m_cols-1) 的最短时间
    vector<vector<long long>> dist_to_end_node = run_dijkstra(n_rows - 1, m_cols - 1, n_rows, m_cols, move_w_cost, grid_values);

    // 初始化总的最小时间为一个非常大的值 (INF)
    long long overall_min_time = INF;

    // 方案一：纯粹步行路径 (不使用任何时空通道)
    // 目标是到达右下角单元格 (n_rows-1, m_cols-1)
    if (dist_from_start_node[n_rows - 1][m_cols - 1] != INF) { // 如果终点可达
        overall_min_time = dist_from_start_node[n_rows - 1][m_cols - 1];
    }

    // 方案二：使用一次时空通道跳跃的路径
    // 总成本 = (从起点步行到入口时空通道P1的成本) + (P1的通道成本) + (出口时空通道P2的成本) + (从P2步行到终点的成本)
    // 我们可以分别找到：
    // 1. min_P1 { (从起点步行到P1的成本) + (P1的通道成本) }
    // 2. min_P2 { (P2的通道成本) + (从P2步行到终点的成本) }
    // 然后将这两个最小值相加。

    long long min_walk_plus_entry_portal_cost = INF; // 从起点步行到任一入口传送门并加上该传送门自身成本的最小值

    // 只有当存在时空通道时才进行计算
    if (!portal_coords.empty()) {
        for (const auto& p_coord : portal_coords) { // 遍历所有时空通道
            int r = p_coord.first;  // 时空通道的行坐标
            int c = p_coord.second; // 时空通道的列坐标
            long long portal_value = grid_values[r][c]; // 该时空通道的成本 a_rc

            // 如果该时空通道 (r,c) 可以从起点步行到达
            if (dist_from_start_node[r][c] != INF) {
                min_walk_plus_entry_portal_cost = min(min_walk_plus_entry_portal_cost,
                                                          dist_from_start_node[r][c] + portal_value);
            }
        }
    }

    long long min_exit_portal_plus_walk_cost = INF; // 从任一出口传送门出发步行到终点并加上该传送门自身成本的最小值

    // 只有当存在时空通道时才进行计算
    if (!portal_coords.empty()) {
        for (const auto& p_coord : portal_coords) { // 遍历所有时空通道
            int r = p_coord.first;  // 时空通道的行坐标
            int c = p_coord.second; // 时空通道的列坐标
            long long portal_value = grid_values[r][c]; // 该时空通道的成本 a_rc

            // dist_to_end_node[r][c] 是从终点 (n_rows-1,m_cols-1) 步行到时空通道 (r,c) 的成本,
            // 这也等于从时空通道 (r,c) 步行到终点 (n_rows-1,m_cols-1) 的成本。
            if (dist_to_end_node[r][c] != INF) {  // 如果从该时空通道 (r,c) 可以步行到达终点
                min_exit_portal_plus_walk_cost = min(min_exit_portal_plus_walk_cost,
                                                        dist_to_end_node[r][c] + portal_value);
            }
        }
    }

    // 如果通过时空通道的路径是可能的
    // (即，存在一个从起点可达的入口传送门，并且存在一个可到达终点的出口传送门)
    if (min_walk_plus_entry_portal_cost != INF && min_exit_portal_plus_walk_cost != INF) {
        long long portal_route_total_time = min_walk_plus_entry_portal_cost + min_exit_portal_plus_walk_cost;
        // 更新总的最小时间
        overall_min_time = min(overall_min_time, portal_route_total_time);
    }

    // 输出结果
    if (overall_min_time == INF) { // 如果 overall_min_time 仍然是 INF，说明无法到达学校
        cout << -1 << endl;
    } else {
        cout << overall_min_time << endl;
    }

    return 0;
}

/*
*
【问题描述】

       阿迪通常开着闹钟睡觉，这样他才不至于上课迟到。

       他想知道能否赶上第一节课，为了不迟到，他需要知道从家到学校所需要的最少时间是多少。

       阿迪生活的城市是一个 n×m 的矩形区域，其中每个单元( i, j )由一个数字 aij 来表示

数字为 -1 时表示该单元被占用，禁止通行

数字为 0 时表示该单元是空闲的，阿迪可以穿过

数字为 x (1≤x ≤109) 时表示该单元包含时空通道，需要耗费的时间成本为 x，包含时空通道的单元也是空闲的，可以自由通行

      从任何包含时空通道的单元出发，阿迪可以去往任何包含时空通道的其他单元，从时空通道( i, j )到时空通道( x, y ) 的时间成本总和为 aij + axy。

      除了在两个包含时空通道的单元之间移动，他也可以在具有相邻边的未被占用的单元之间移动，耗费的时间为 w 。实际上，他也可以进入一个包含时空通道的单元而不使用它这个通道。

      开始时，阿迪处在左上角单元 (1, 1)，而学校位于右下角(n, m)。

【输入形式】

       输入的第一行包含三个整数 n、m 和 w (2 ≤ n、m ≤ 2×103，1≤ w ≤109)，此处 n 和 m 是城市的大小，w 是在未被占用的单元之间移动所需要的时间。

       接下来的 n 行每行包含 m 个数 (-1 ≤ ai ≤ 109)，表示对单元的描述。

       输入保证单元(1, 1)和(n, m)是空闲的。

【输出形式】

       输出为一个数，表示阿迪去往学校需要花费的最少时间，如果他不能去到学校，则输出-1。

【样例输入】

5 5 1
0 -1 0 1 -1
0 20 0 0 -1
-1 -1 -1 -1 -1
3 0 0 0 0
-1 0 0 0 0
【样例输出】

14
【样例说明】

*/