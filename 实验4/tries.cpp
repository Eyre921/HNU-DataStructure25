#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

using namespace std;

int main() {
    ifstream ifs("in.txt");  // 打开文件
    if (!ifs.is_open()) {
        cerr << "Failed to open file." << endl;
        return -1;
    }

    unordered_map<string, int> wordFreq;  // 存储单词及其频率

    string line;
    regex wordRegex("[a-zA-Z]+");  // 匹配单词的正则表达式

    while (getline(ifs, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);  // 将整个行转换为小写字母

        auto words_begin = sregex_iterator(line.begin(), line.end(), wordRegex);
        auto words_end = sregex_iterator();

        // 遍历所有匹配到的单词
        for (sregex_iterator i = words_begin; i != words_end; ++i) {
            string word = (*i).str();
            wordFreq[word]++;  // 统计单词出现的频率
        }
    }

    ifs.close();  // 关闭文件

    // 将unordered_map转换为vector，并按频率排序
    vector<pair<string, int>> sortedWords;
    for (const auto &entry : wordFreq) {
        sortedWords.push_back(entry);  // 将unordered_map的每个键值对加入vector
    }

    sort(sortedWords.begin(), sortedWords.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        if (a.second != b.second)
            return a.second > b.second;  // 按频率降序排列
        else
            return a.first < b.first;  // 频率相同按字典顺序升序排列
    });

    // 输出前100个单词和它们的频率
    for (int i = 0; i < min(100, (int)sortedWords.size()); ++i) {
        cout << sortedWords[i].first << " " << sortedWords[i].second << endl;
    }

    return 0;
}
/// Created by tiny on 25-4-16.
//
