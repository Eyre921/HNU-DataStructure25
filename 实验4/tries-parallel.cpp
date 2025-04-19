#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <thread>
#include <future>
#include <mutex>
#include <atomic>
#include <map>

using namespace std;

mutex mtx;  // 用于保护共享资源 wordFreq
atomic<int> linesProcessed(0);  // 记录处理的行数

void processLine(const string& line, unordered_map<string, int>& wordFreq, const regex& wordRegex) {
    string lowerLine = line;
    transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);  // 转换为小写

    auto words_begin = sregex_iterator(lowerLine.begin(), lowerLine.end(), wordRegex);
    auto words_end = sregex_iterator();

    // 临时局部 map，用于存储当前线程中统计的单词频率
    unordered_map<string, int> localWordFreq;

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        string word = (*i).str();
        localWordFreq[word]++;  // 统计当前线程中的单词频率
    }

    // 合并结果到共享的 wordFreq
    lock_guard<mutex> lock(mtx);
    for (const auto& entry : localWordFreq) {
        wordFreq[entry.first] += entry.second;
    }

    linesProcessed++;  // 计数处理的行数
}

int main() {
    //ifstream ifs("D:\\Eyre\\C++\\数据结构\\实验4\\in.txt");  // 打开文件
    ifstream ifs("in.txt");  // 打开文件
    if (!ifs.is_open()) {
        cerr << "Failed to open file." << endl;
        return -1;
    }

    unordered_map<string, int> wordFreq;  // 存储单词及其频率
    string line;
    regex wordRegex("[a-zA-Z]+");  // 匹配单词的正则表达式

    vector<future<void>> futures;  // 用于存储线程的结果

    // 使用多线程并行读取文件并处理每一行
    while (getline(ifs, line)) {
        futures.push_back(async(launch::async, processLine, line, ref(wordFreq), ref(wordRegex)));
    }

    // 等待所有线程完成
    for (auto& fut : futures) {
        fut.get();
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

    //cout << "Total lines processed: " << linesProcessed.load() << endl;

    return 0;
}
// 本代码有一个样例无法通过，出现运行时错误
