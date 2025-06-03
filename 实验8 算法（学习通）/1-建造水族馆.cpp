#include <iostream>
#include <vector>

using namespace std;

// 函数：检查给定的高度 'h_candidate' 是否可行
// （即用水量不超过 'max_water'）
// h_candidate: 拟议的水族箱高度。
// n: 珊瑚柱的数量。
// max_water: 可用的最大水量。
// corals: 包含珊瑚柱高度的向量。
bool can_build(long long h_candidate, int n, long long max_water, const vector<long long>& corals) {
    long long water_needed = 0; // 需要的水量
    for (int i = 0; i < n; ++i) {
        // 如果珊瑚高度小于拟议的水族箱高度，
        // 则需要水来填充差额。
        if (corals[i] < h_candidate) {
            water_needed += (h_candidate - corals[i]);
        }

        // 优化：如果所需水量已超过最大可用水量，
        // 则无需继续累加。此候选高度不可行。
        if (water_needed > max_water) {
            return false;
        }
    }
    // 如果总需水量在允许范围内，则此高度可行。
    return water_needed <= max_water;
}

// 函数：解决单个测试用例。
void solve() {
    int n;          // 珊瑚柱的数量
    long long x;    // 最大可用水量
    cin >> n >> x;
    vector<long long> a(n); // 存储珊瑚高度的向量
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    long long low = 1; // 水族箱可能的最小高度 (h >= 1)
    // 确定二分搜索的安全上界。

    long long high = 2000000000LL + 100LL;
    long long ans = 0; // 存储目前找到的有效最大高度。由于h>=1且x>=1，ans至少会是1。

    // 二分搜索最大可能的高度 h。
    while (low <= high) {
        long long mid = low + (high - low) / 2; // 计算中点，避免溢出

        if (can_build(mid, n, x, a)) {
            // 如果 'mid' 是一个可能的高度，它可能是我们的答案。
            // 尝试寻找一个更大的高度。
            ans = mid;
            low = mid + 1;
        } else {
            // 如果 'mid' 不是一个可能的高度（需要太多水），
            // 我们需要尝试较小的高度。
            high = mid - 1;
        }
    }
    cout << ans << endl;
}

int main() {
    // 为更快的 I/O 优化 C++ 标准流。
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t; // 测试用例的数量
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}

/*1. 建造水族馆

【问题描述】

            小希非常喜欢鱼，因此决定建造一个水族箱。他有一块由n列组成的珊瑚礁，第i列的高度为 ai。接下来，他将围绕这块珊瑚礁建造一个水族箱，具体步骤如下：

选择一个整数 h ≥ 1 作为水族箱的高度。

在水族箱的两侧建造高度为 h 的墙壁。

然后，向水族箱中注水，使得每一列的水位高度为 h，除非珊瑚的高度超过 h，此时该列不注水。

如测试样例的第一组，允许使用的水量为x=9个单位，当珊瑚高度为a=[3,1,2,4,6,2,5]且选择水族箱高度h=4时，最终将使用总共w=8单位的水，如图所示。



            他可以使用最多x个单位的水来注满这个水族箱，他也希望建造一个最大的水族箱，那么他可以选择的高度h的最大值是多少？

【输入形式】

        输入的第一行为一个正整数t(1≤t≤104)，表示测试数据的组数。

        每个测试用例的第一行为两个正整数n和x(1≤n≤2×105、1≤x≤109), 表示珊瑚的列数以及小希可以使用的水的最大量。

        每个测试用例的第二行为n个正整数ai(1≤ai≤109), 表示每列珊瑚的高度。

        所有的输入数据在满足约束条件下，保证都能输出一个有效的最大高度。

【输出形式】

        输出为t行，每行一个正整数，表示每个水族箱满足条件的最大高度h。    

【样例输入】

5

7 9

3 1 2 4 6 2 5

3 10

1 1 1

4 1

1 4 3 4

6 1984

2 6 5 9 1 8

1 1000000000

1

【样例输出】

4

4

2

335

1000000001

【样例说明】

第一个测试用例已在题目描述中图示说明。当选择h=4时，需要8单位的水量；但如果将h提高到5，则需要13单位的水量，这超过了给定的x=9单位。因此h=4是最优解。

在第二个测试用例中，我们可以选择h=4，为每列添加3单位水量，总共使用9单位的水。可以证明这是最优方案。

第三个测试用例中，我们可以选择h=2并完全用完所有的水量，因此这是最优解。*/