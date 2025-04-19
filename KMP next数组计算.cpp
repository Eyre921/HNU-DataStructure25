#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 函数用于计算字符串t的next数组
void GetNext(const string& t, vector<int>& next) {
    int m = t.length();
    next[0] = -1;
    for (int i = 1; i < m; ++i) {
        int j = next[i - 1];
        while (j >= 0 && t[i] != t[j + 1]) {
            j = next[j];
        }
        if (t[i] == t[j + 1]) {
            next[i] = j + 1;
        } else {
            next[i] = -1;
        }
    }
}

int main() {
    string t;
    cout << "请输入待计算的字符串:" << endl;
    cin >>t;
    vector<int> next(t.length());
    GetNext(t, next);
    // 输出next数组
    for (int val : next) {
        cout << val << " ";
    }
    cout << endl;
    return 0;
}