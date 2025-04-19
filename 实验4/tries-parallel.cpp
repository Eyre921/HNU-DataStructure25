#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;  // 用于保护共享资源（wordFreq）的互斥量

// 词频统计函数
void process_chunk(const string& chunk, unordered_map<string, int>& wordFreq) {
    regex wordRegex("[a-zA-Z]+");  // 匹配单词的正则表达式
    string line;
    string transformed_chunk = chunk; // Create a temporary string to store the transformed chunk

    transform(transformed_chunk.begin(), transformed_chunk.end(), transformed_chunk.begin(), ::tolower);  // 将整个块转换为小写字母

    auto words_begin = sregex_iterator(transformed_chunk.begin(), transformed_chunk.end(), wordRegex);
    auto words_end = sregex_iterator();

    // 遍历所有匹配到的单词
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        string word = (*i).str();
        lock_guard<mutex> lock(mtx);  // 确保对共享资源wordFreq的访问是线程安全的
        wordFreq[word]++;  // 统计单词出现的频率
    }
}

// 分割文件并使用多线程处理
void process_file(const string& filename, unordered_map<string, int>& wordFreq, int numThreads) {
    ifstream ifs(filename);  // 打开文件
    if (!ifs.is_open()) {
        cerr << "Failed to open file." << endl;
        return;
    }

    // 获取文件内容
    string fileContent((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

    // 文件内容的大小
    size_t chunkSize = fileContent.size() / numThreads;

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? fileContent.size() : (i + 1) * chunkSize;
        string chunk = fileContent.substr(start, end - start);

        // 启动每个线程处理一个文件块
        threads.push_back(thread(process_chunk, chunk, ref(wordFreq)));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    ifs.close();  // 关闭文件
}

int main() {
    unordered_map<string, int> wordFreq;  // 存储单词及其频率

    string filename = "in.txt";  // 文件名
    int numThreads = 4;  // 使用四个线程，机器有四个核心

    // 使用多线程处理文件
    process_file(filename, wordFreq, numThreads);

    // 将unordered_map转换为vector，并按频率排序
    vector<pair<string, int>> sortedWords;
    for (const auto& entry : wordFreq) {
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
