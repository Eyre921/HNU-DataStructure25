#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;
string ErrSign = "NO";
// 判断字符是否合法
bool isValidExpression(const string& expr) {
    for (char c : expr) {
        if (!(isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '#')) {
            return false;
        }
    }
    return true;
}

// 判断运算符的优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 计算两个数的运算
int applyOperation(int a, int b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b; // 假设输入表达式合法，不会有除零错误
    return 0;
}

// 计算表达式的值
int evaluateExpression(const string& expr) {
    stack<int> values;  // 存储操作数
    stack<char> ops;    // 存储运算符

    size_t i = 0;
    while (i < expr.length()) {
        if (isspace(expr[i])) {
            // 跳过空格
            i++;
            continue;
        }

        if (isdigit(expr[i])) {
            // 处理数字
            int num = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            values.push(num);
        } else if (expr[i] == '(') {
            // 左括号直接压入栈
            ops.push('(');
            i++;
        } else if (expr[i] == ')') {
            // 处理右括号，直到遇到左括号
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top();
                ops.pop();
                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.pop(); // 弹出 '('
            i++;
        } else {
            // 处理运算符
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                char op = ops.top();
                ops.pop();
                int b = values.top();
                values.pop();
                int a = values.top();
                values.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.push(expr[i]);
            i++;
        }
    }

    // 计算剩下的运算符
    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();
        int b = values.top();
        values.pop();
        int a = values.top();
        values.pop();
        values.push(applyOperation(a, b, op));
    }

    // 最终栈顶元素即为结果
    return values.top();
}

int main() {
    string expr;
    getline(cin, expr);

    // 去掉末尾的 '#'
    if (expr.back() == '#') {
        expr.pop_back();
    } else {
        cout << ErrSign << endl;
        return 0;
    }

    // 检查表达式是否合法
    if (!isValidExpression(expr)) {
        cout << ErrSign << endl;
        return 0;
    }

    // 计算表达式并输出结果
    try {
        int result = evaluateExpression(expr);
        cout << result << endl;
    } catch (const exception& e) {
        cout << ErrSign << endl;
    }

    return 0;
}
/*
【问题描述】

设计一个程序，对输入的以#为结束的算术表达式（包括+，-，*，/，(，) ），首先判断表达式是否含有非法字符（即非+，-，*，/， (，) 之外的字符），

如果含有非法字符，则报错误信息；

如果正确，计算并输出这个表示式的值。

本题希望利用算符优先关系，实现对算术四则混合运算表达式的求值。



【输入格式】

以#为结束符的算术表达式。

【输出格式】

　　对于每组测试数据算术表达式，如果含有非法字符，输出“NO”，否则输出表达式的值，行尾不得有多余的空格。

要求：

使用STL求解表达式的值。

【样例输入】

3+4*(5-3)#

【样例输出】

   11

【样例说明】

　　输入样例是合法的表达式，因此求表达式的值，并输出11。

【样例输入】

3+;4*(5-3)#

【样例输出】

表达式不合法

【样例说明】

输入样例含不合法的字符';'，因此输出“NO”。


【特别说明】

建议使用STL
 * */