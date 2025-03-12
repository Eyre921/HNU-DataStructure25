#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 递归函数，用于检查是否可以将车厢按正确顺序输出
void process_output(queue<int>& input_queue, vector<queue<int>>& buffer_queues, int* current_output, int& num_buffers);

// 递归实现输出操作
void process_output(queue<int>& input_queue, vector<queue<int>>& buffer_queues, int* current_output, int& num_buffers) {
    // 如果输入队列的首元素等于当前输出车厢编号，则输出该车厢
    if (input_queue.front() == *current_output) {
        input_queue.pop();  // 从输入队列中移除
        (*current_output)++; // 当前输出车厢编号递增
        process_output(input_queue, buffer_queues, current_output, num_buffers);  // 递归调用
    }

    bool moved = false;
    // 检查缓冲轨道中的车厢，是否可以输出
    for (int i = 0; i < num_buffers; i++) {
        if (!buffer_queues[i].empty()) {
            if (buffer_queues[i].front() == *current_output) {
                buffer_queues[i].pop();  // 从缓冲轨道中移除
                (*current_output)++;     // 当前输出车厢编号递增
                moved = true;
                break;
            }
        }
    }

    if (moved) {
        process_output(input_queue, buffer_queues, current_output, num_buffers);  // 递归调用
    }
}

int main() {
    // 初始化当前输出车厢编号
    int current_output = 1;
    queue<int> input_queue;
    vector<int> carriages; // 存储车厢输入顺序
    int n;

    // 读取车厢编号输入
    while (scanf("%d", &n) != EOF) {
        carriages.push_back(n);
    }

    // 将车厢按逆序放入输入队列
    for (int i = carriages.size() - 1; i >= 0; --i) {
        input_queue.push(carriages[i]);
    }

    // 初始化缓冲轨道数量
    int num_buffers = input_queue.size();
    vector<queue<int>> buffer_queues(num_buffers);  // 缓冲轨道队列数组
    vector<int> use(num_buffers, 0); // 记录缓冲轨道的使用情况

    // 处理输入队列中的车厢
    while (!input_queue.empty()) {
        process_output(input_queue, buffer_queues, &current_output, num_buffers);

        if (!input_queue.empty()) {
            // 如果当前输入队列还有车厢，则将其放入合适的缓冲轨道
            for (int i = 0; i < num_buffers; i++) {
                if (buffer_queues[i].empty()) {
                    buffer_queues[i].push(input_queue.front());
                    input_queue.pop();
                    use[i] = 1;  // 标记该缓冲轨道已被使用
                    break;
                } else {
                    // 如果当前车厢编号大于缓冲轨道的最后一个车厢编号，则可以放入
                    if (input_queue.front() > buffer_queues[i].back()) {
                        buffer_queues[i].push(input_queue.front());
                        input_queue.pop();
                        break;
                    }
                }
            }
        }
    }

    // 计算最终使用的缓冲轨道数量
    for (int i = 1; i < num_buffers; i++) {
        use[i] += use[i - 1]; // 累加缓冲轨道使用情况
    }

    // 输出所需的最小缓冲轨道数量
    cout << use[num_buffers - 1] + 1 << endl;
    return 0;
}
