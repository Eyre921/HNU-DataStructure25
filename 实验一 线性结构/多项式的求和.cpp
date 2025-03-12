#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 定义一个结构体来存储多项式的每一项：系数和指数
struct Term {
    int coefficient;  // 系数
    int exponent;     // 指数
};

// 比较函数，用于按指数降序排列
bool compare(const Term& a, const Term& b) {
    return a.exponent > b.exponent;
}

int main() {
    int n, m;

    // 输入第一个多项式
    cin >> n;
    vector<Term> poly1(n);
    for (int i = 0; i < n; ++i) {
        cin >> poly1[i].coefficient >> poly1[i].exponent;
    }

    // 输入第二个多项式
    cin >> m;
    vector<Term> poly2(m);
    for (int i = 0; i < m; ++i) {
        cin >> poly2[i].coefficient >> poly2[i].exponent;
    }

    // 用于存储加法结果
    vector<Term> result;

    // 将两个多项式的项合并
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (poly1[i].exponent > poly2[j].exponent) {
            result.push_back(poly1[i]);
            ++i;
        } else if (poly1[i].exponent < poly2[j].exponent) {
            result.push_back(poly2[j]);
            ++j;
        } else {
            // 如果指数相同，系数相加
            int new_coefficient = poly1[i].coefficient + poly2[j].coefficient;
            if (new_coefficient != 0) { // 如果结果不为0，则加入结果
                result.push_back({new_coefficient, poly1[i].exponent});
            }
            ++i;
            ++j;
        }
    }

    // 将剩余的项加入结果
    while (i < n) {
        result.push_back(poly1[i]);
        ++i;
    }
    while (j < m) {
        result.push_back(poly2[j]);
        ++j;
    }

    // 输出结果
    for (const auto& term : result) {
        cout << term.coefficient << " " << term.exponent << endl;
    }

    return 0;
}
/*
【问题描述】

在数学上，一个一元n次多项式Pn(x)可按降序写成：



它是由n＋1个系数唯一确定。因此，在计算机里它可以用一个线性表P来表示：

P=（Pn, Pn-1, …, P1, Po）

一元多项式的运算包括加法、减法和乘法，而多项式的减法和乘法都可以用加法来实现。

利用实验一实现的线性表AD，设计和实现两个一元多项式的加法运算。

提示：用有序线性表表示一元多项式。表内元素按照多项式的次数递减的次序排列。

第一个多项式：3x^5-2x+4；则在计算机中输入：

3

3 5

-2 1

4 0

第二个多项式：2x^3-x^2+x+3；则在计算机中输入：

4

2 3

-1 2

1 1

3 0

输出计算结果：3x^5+2x^3-x^2-x+7；则在计算机中输出：

3 5

2 3

-1 2

-1 1

7 0

————————————————

【输入形式】

  　　输入第一行包含一个整数n,代表第一个多项式有n项。
  　　第二行起的包含n行整数对，每行的2个整数之间使用一个空格分隔；

  　　接下来一行包含一个整数m,代表第二个多项式有m项。
  　　此下包含m行整数对，每行的2个整数之间使用一个空格分隔；

【输出形式】

       输出多行整数对，表示答案。

【样例输入】

3

3 5

-2 1

4 0

4

2 3

-1 2

1 1

3 0

【样例输出】

3 5

2 3

-1 2

-1 1

7 0

【样例说明】

    多项式3x^5-2x+4与多项式2x^3-x^2+x+3相加，计算结果：3x^5+2x^3-x^2-x+7。


【样例输入】


    2

    3 5

    3 1

    0

【样例输出】


     3 5

    3 1

【样例说明】

    多项式3x^5+3x与多项式0相加，计算结果：3x^5+3x。

【数据规模和约定】

    对于所有评测用例，2 ≤ n ≤ 100，每个给定的整数都是不超过100的正整数。
 */