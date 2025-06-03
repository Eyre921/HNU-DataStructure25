#include <iostream> // 用于标准输入输出流
#include <vector>   // 用于动态数组 vector

using namespace std;
// 辅助函数 check:
// 判断是否可以将数组 arr 分成至多 M_segments 段，
// 使得每段的和都不超过 candidate_max_sum。
// N_elements 是数组 arr 的大小。
bool check(int candidate_max_sum, int M_segments, int N_elements, const vector<int>& arr) {
    int segments_needed = 1;          // 初始化所需段数为1
    int current_sum_in_segment = 0; // 当前段的累积和

    for (int i = 0; i < N_elements; ++i) {
        // 如果单个元素已经大于候选的最大段和，则此候选值不可行
        if (arr[i] > candidate_max_sum) {
            return false;
        }

        // 如果当前元素加入当前段后，和不超过候选的最大段和
        if (current_sum_in_segment + arr[i] <= candidate_max_sum) {
            current_sum_in_segment += arr[i]; // 将元素加入当前段
        } else {
            // 当前元素无法加入当前段 (会导致超额)
            segments_needed++;                // 需要开启一个新段
            current_sum_in_segment = arr[i]; // 新段的第一个元素是当前元素
        }
    }

    // 如果最终需要的段数小于或等于 M_segments，则此候选值可行
    return segments_needed <= M_segments;
}

int main() {
    // 使用快速I/O，加速输入输出
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M; // N: 数列长度, M: 要分的段数
    cin >> N >> M;

    vector<int> A(N); // 存储数列A的元素

//    long long total_sum_of_A = 0; // 数列A的总和 (使用long long以防万一，但题目约束A_i之和不超过10^9，int也够)
                                 // 经过仔细分析，A_i之和不超过10^9，最大元素也不超过10^9，
                                 // 二分查找的上下界以及段内和都不会超过10^9，int是足够的。
                                 // 这里将 total_sum_of_A 改回 int 类型。
    int sum_of_A = 0;
    int max_element_in_A = 0; // 数列A中的最大元素 (A_i为非负整数，初值为0是安全的)

    // 读取数列元素，同时计算总和与最大值
    // 题目保证 N 是正整数 (N >= 1)
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        sum_of_A += A[i];
        if (A[i] > max_element_in_A) {
            max_element_in_A = A[i];
        }
    }

    // 设置二分查找的范围
    int low = max_element_in_A; // 解的下界：不能小于数列中的最大单个元素
    int high = sum_of_A;        // 解的上界：不可能大于数列的总和 (M=1的情况)
    int ans = high;             // 初始化答案为可能的最大值

    // 开始二分查找
    while (low <= high) {
        // 计算中间值，这种写法可以防止 (low + high) 溢出 (虽然在此题int范围内不太可能)
        int mid = low + (high - low) / 2;

        if (check(mid, M, N, A)) {
            // 如果 mid 是一个可行的“最大段和”
            ans = mid;          // 更新答案为 mid
            high = mid - 1;     // 尝试寻找更小的可行“最大段和” (即在左半区间查找)
        } else {
            // 如果 mid 不可行 (太小了)
            low = mid + 1;      // 需要增大“最大段和” (即在右半区间查找)
        }
    }

    cout << ans << endl; // 输出最终找到的最小的“最大段和”

    return 0;
}
/*
4. 数列分段
【问题描述】

对于给定的一个长度为 N 的正整数数列 A ，现要将其分成 M 段，并要求每段连续，且每段和的最大值最小。
例如，将数列 4 2 4 5 1 要分成 3 段：
若分为 [4 2][4 5][1]，各段的和分别为 6, 9, 1 ，和的最大值为 9；
若分为 [4][2 4][5 1]，各段的和分别为 4, 6, 6 ，和的最大值为 6；
并且无论如何分段，最大值不会小于 6。
所以可以得到要将数列 4 2 4 5 1 要分成 3 段，每段和的最大值最小为 6 。
【输入形式】

第 1 行包含两个正整数 N，M；

第 2 行包含 N 个空格隔开的非负整数 A_i，含义如题目所述。
N ≤ 10⁵，M ≤ N， A_i 之和不超过 10⁹ 。
【输出形式】

仅包含一个正整数，即每段和最大值最小为多少。

【样例输入】


5 3
4 2 4 5 1
【样例输出】

6
 */