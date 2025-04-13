#include<iostream>
using namespace std;

// 定义最大节点数
int Left[100005];
int Right[100005];
int num[100005];

// 定义节点结构体
struct node{
    int id;         // 节点编号
    int weight;     // 节点的权重
}heap[10005];      // 存储堆中的节点

// 最小堆类
class minheap{
    private:
        int cnt;     // 当前堆中的节点数
    public:
        minheap(int n){cnt=n;}    // 构造函数，初始化堆的大小
        void buildheap();         // 构建堆
        void siftdown(int index); // 向下调整堆
        void push(node a);        // 向堆中插入新节点
        void pop();               // 弹出堆顶节点
        node& front();            // 返回堆顶元素
        bool Comp(const node& a,const node& b); // 比较两个节点的权重
        void swap(node& a,node& b); // 交换两个节点
        int size();               // 返回当前堆的大小
};

// 初始化左右子节点数组
void inital(int n)
{
    for(int i=0;i<=n;i++)
        Left[i]=Right[i]=0;
    return ;
}

int ans=0; // 存储最终结果

// 递归计算Huffman树的深度和权重
void solve(int index,int depth)
{
    // 如果是叶子节点，计算其深度*权重并加到答案中
    if(Left[index]==0 && Right[index]==0)
    {
        ans+=(depth*num[index]);
        return ;
    }
    
    // 递归访问左右子树，深度加1
    solve(Left[index],depth+1);
    solve(Right[index],depth+1);
    
    return ;
}

int main()
{
    while(1)
    {
        int n;
        cin >> n;  // 输入节点数
        
        if(n == 0)
            return 0;  // 如果n为0，结束程序
        
        // 输入每个节点的权重
        for(int i=1;i<=n;i++)
        {
            cin >> heap[i].weight;
            heap[i].id = i;
            num[i] = heap[i].weight;
        }
        
        inital(n);  // 初始化左右子节点
        minheap q(n);  // 创建最小堆
        q.buildheap();  // 构建堆
        
        int size = n;
        node tmp1, tmp2, tmp3;
        
        // 通过最小堆逐步合并节点
        while(q.size() > 1)
        {
            tmp1 = q.front();  // 获取堆顶节点
            q.pop();            // 弹出堆顶节点
            tmp2 = q.front();  // 获取下一个堆顶节点
            q.pop();            // 弹出堆顶节点
            
            // 合并两个节点，生成新节点
            tmp3.weight = tmp1.weight + tmp2.weight;
            tmp3.id = ++size;
            Left[size] = tmp1.id;
            Right[size] = tmp2.id;
            num[size] = tmp3.weight;
            
            q.push(tmp3);  // 将新节点插入堆中
        }
        
        tmp1 = q.front();  // 获取最终的堆顶节点（即根节点）
        ans = 0;
        
        solve(tmp1.id, 0);  // 从根节点开始计算树的深度和权重
        
        cout << ans << '\n';  // 输出结果
    }
    return 0;
}

// 构建堆，从最后一个非叶节点开始调整堆
void minheap::buildheap()
{
    for(int i = cnt / 2; i >= 1; i--)
        siftdown(i);  // 向下调整堆
    return ;
}

// 弹出堆顶元素
void minheap::pop()
{
    heap[1] = heap[cnt--];  // 将堆顶节点与最后一个节点交换，并减少堆大小
    siftdown(1);            // 调整堆
    return ;
}

// 向堆中插入一个新节点，并保持堆的性质
void minheap::push(node a)
{
    heap[++cnt] = a;  // 将新节点放入堆末尾
    int root = cnt;
    int father = root / 2;
    
    // 向上调整堆，直到堆的性质被满足
    while(root != 1)
    {
        if(Comp(heap[root], heap[father]))
            swap(heap[root], heap[father]);
        else return;
        
        root = father;
        father = root / 2;
    }
    return ;
}

// 获取堆顶元素
node& minheap::front()
{
    return heap[1];
}

// 比较两个节点的权重
bool minheap::Comp(const node& a, const node& b)
{
    return a.weight < b.weight;  // 如果a的权重大于b的权重，返回false
}

// 交换两个节点的位置
void minheap::swap(node& a, node& b)
{
    int index = a.id;
    int wgt = a.weight;
    
    a.id = b.id; a.weight = b.weight;
    b.id = index; b.weight = wgt;
    return ;
}

// 向下调整堆
void minheap::siftdown(int index)
{
    int root = index;
    int l = root << 1, r = root << 1 | 1;
    
    while(l <= cnt)
    {
        // 如果右子树存在且右子树的权重小于左子树，则选择右子树
        if(r <= cnt && Comp(heap[r], heap[l]))
            l = r;
        
        // 如果当前节点的权重大于等于左子树，则调整堆
        if(Comp(heap[root], heap[l]))
            return ;
        
        swap(heap[root], heap[l]);  // 交换节点
        
        root = l;  // 更新根节点为当前子节点
        l = root << 1; r = root << 1 | 1;  // 更新左右子节点
    }
    return ;
}

// 返回堆的大小
int minheap::size()
{
    return cnt;
}
/*
【问题描述】

    树的路径长度是从树根到树中每一叶子结点的路径长度之和.在结点数目相同的二叉树中,完全二叉树的路径长度最短。

结点的权：赋予二叉树树中结点的一个有某种意义的实数。

结点的带权路径长度：结点到树根之间的路径长度与该结点上权的乘积。

树的带权路径长度(Weighted Path Length of Tree)：定义为树中所有叶结点的带权路径长度之和,通常记为：

T=∑Wi×li  i=1,2，……，n   i表示叶子节点的编号。

其中：n表示叶子结点的数目Wi和li分别表示叶结点i的权值和根到结点i之间的路径长度。树的带权路径长度亦称为树的代价。

现在给定n个叶子节点的权值，求出一颗二叉树，使得这棵二叉树的带权路径长度WPL最小，并且求出这个最小带权路径长度。

比如3个叶子节点的权值是1,2,3，其具有最小带权路径长度如图C_1所示，其最小带权路径长度WPL=1×2+2×2+3×1=9。


【输入形式】

有多组测试数据，每组测试数据占两行，第一行是一个正整数n，表示叶子节点个数， n=0意味着输入结束并且不需要处理。

第二行是n个正整数，表示每个叶子节点的权值Wi，所有的Wi≤10000。

40%的测试数据3 ≤ n≤ 10

30%的测试数据3 ≤ n≤ 102

20%的测试数据3 ≤ n≤ 103

10%的测试数据3 ≤ n≤ 104

【输出形式】

    对于每组测试数据，输出一行，包含一个正整数，表示最小带权路径长度WPL。测试数据保证正确的结果不超过231。

【样例输入】


3
1 2 3
4
1 1 1 1 
0
【样例输出】 


9
8
*/
