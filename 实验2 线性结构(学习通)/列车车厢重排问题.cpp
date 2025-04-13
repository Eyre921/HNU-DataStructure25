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
/**
列车车厢重排问题
【问题描述】

    列车车厢重排时，位于入轨道和出轨道之间的缓冲轨道按照FIFO方式运作， 如下图所示，因此可将它们视为队列。禁止将车厢从缓冲轨道移到入轨道，或从出轨道移到缓冲轨道。所有的车厢移动都要按照图中箭头所示的方向进行。

微信图片_20240314101045.jpg

第k条轨道Hk可直接将车厢从入轨道移到出轨道。其余 k-1 条轨道用来缓存不能直接进入出轨道的车厢。



假定有9节车厢需要重排，其初始顺序5、8、1、7、4、2、9、6、3。假设k=3(见图)。3号车厢不能直接进人出轨道，因为1号车厢和2号车厢必须排在3号车厢之前。因此，3号车厢进入缓冲轨道H1。6号车厢可进人缓冲轨道H1，排在3号车厢之后，因为6号车厢是在3号车厢之后进人出轨道。9号车厢可以继续进入缓冲轨道H1，排在6号车厢之后。2号车厢不可排在9号车厢之后，因为它必须在9号车厢之前进入出轨道。因此，2号车厢进入缓冲轨道H2.排在第一。4号车厢可以进人缓冲轨道H2，排在2号车厢之后。7号车厢也可进入缓冲轨道H2，排在4号车厢之后。这时，1号车厢可通过缓冲轨道 H3直接进入出轨道。接下来2号车厢从缓冲轨道H2进入出轨道，3号厢从缓冲轨道H1进入出轨道，4号车厢从缓冲轨道H2进人出轨道。由于5号车厢此时仍在入轨道上，且排在8号车厢之后，所以8号车厢进人缓冲轨道H2，这样5号车厢可以通过缓冲轨道H3，直接从入轨道进入出轨道。然后，6号7号、8号和9号车厢依次从缓冲轨道进入出轨道。



当一节车厢c进人缓冲轨道时，依据如下的原则来选择缓冲轨道:

如果缓冲轨道上已有的尾部车厢编号均小于c；且有多个缓冲轨道都满足这一条件，则选择左端车厢编号最大的缓冲轨道

否则选择一个空的缓冲轨道(如果有的话)



基本算法如下：

    1 分别对k个队列初始化；

    2 初始化下一个要输出的车厢编号nowOut = 1;

    3 依次取入轨中的每一个车厢的编号；

    3.1 如果入轨中的车厢编号等于nowOut，则

        3.1.1 输出该车厢；

        3.1.2  nowOut++；

    3.2 否则，考察每一个缓冲轨队列

        for (j=1; j<=k; j++)

        3.2.1 取队列 j 的队头元素c；

        3.2.2 如果c=nowOut，则

        3.2.2.1 将队列 j 的队头元素出队并输出；

        3.2.2.2  nowOut++；

    3.3 如果入轨和缓冲轨的队头元素没有编号为nowOut的车厢，则

        3.3.1 求小于入轨中第一个车厢编号的最大队尾元素所在队列编号j;

        3.3.2 如果 j 存在，则把入轨中的第一个车厢移至缓冲轨 j；



给定入轨道车厢的一个排列，输出至少需要的缓冲轨道数量。

【输入形式】

5 8 1 7 4 2 9 6 3
【输出形式】

3
*/
