#include<iostream>
using namespace std;

// �������ڵ���
int Left[100005];
int Right[100005];
int num[100005];

// ����ڵ�ṹ��
struct node{
    int id;         // �ڵ���
    int weight;     // �ڵ��Ȩ��
}heap[10005];      // �洢���еĽڵ�

// ��С����
class minheap{
    private:
        int cnt;     // ��ǰ���еĽڵ���
    public:
        minheap(int n){cnt=n;}    // ���캯������ʼ���ѵĴ�С
        void buildheap();         // ������
        void siftdown(int index); // ���µ�����
        void push(node a);        // ����в����½ڵ�
        void pop();               // �����Ѷ��ڵ�
        node& front();            // ���ضѶ�Ԫ��
        bool Comp(const node& a,const node& b); // �Ƚ������ڵ��Ȩ��
        void swap(node& a,node& b); // ���������ڵ�
        int size();               // ���ص�ǰ�ѵĴ�С
};

// ��ʼ�������ӽڵ�����
void inital(int n)
{
    for(int i=0;i<=n;i++)
        Left[i]=Right[i]=0;
    return ;
}

int ans=0; // �洢���ս��

// �ݹ����Huffman������Ⱥ�Ȩ��
void solve(int index,int depth)
{
    // �����Ҷ�ӽڵ㣬���������*Ȩ�ز��ӵ�����
    if(Left[index]==0 && Right[index]==0)
    {
        ans+=(depth*num[index]);
        return ;
    }
    
    // �ݹ����������������ȼ�1
    solve(Left[index],depth+1);
    solve(Right[index],depth+1);
    
    return ;
}

int main()
{
    while(1)
    {
        int n;
        cin >> n;  // ����ڵ���
        
        if(n == 0)
            return 0;  // ���nΪ0����������
        
        // ����ÿ���ڵ��Ȩ��
        for(int i=1;i<=n;i++)
        {
            cin >> heap[i].weight;
            heap[i].id = i;
            num[i] = heap[i].weight;
        }
        
        inital(n);  // ��ʼ�������ӽڵ�
        minheap q(n);  // ������С��
        q.buildheap();  // ������
        
        int size = n;
        node tmp1, tmp2, tmp3;
        
        // ͨ����С���𲽺ϲ��ڵ�
        while(q.size() > 1)
        {
            tmp1 = q.front();  // ��ȡ�Ѷ��ڵ�
            q.pop();            // �����Ѷ��ڵ�
            tmp2 = q.front();  // ��ȡ��һ���Ѷ��ڵ�
            q.pop();            // �����Ѷ��ڵ�
            
            // �ϲ������ڵ㣬�����½ڵ�
            tmp3.weight = tmp1.weight + tmp2.weight;
            tmp3.id = ++size;
            Left[size] = tmp1.id;
            Right[size] = tmp2.id;
            num[size] = tmp3.weight;
            
            q.push(tmp3);  // ���½ڵ�������
        }
        
        tmp1 = q.front();  // ��ȡ���յĶѶ��ڵ㣨�����ڵ㣩
        ans = 0;
        
        solve(tmp1.id, 0);  // �Ӹ��ڵ㿪ʼ����������Ⱥ�Ȩ��
        
        cout << ans << '\n';  // ������
    }
    return 0;
}

// �����ѣ������һ����Ҷ�ڵ㿪ʼ������
void minheap::buildheap()
{
    for(int i = cnt / 2; i >= 1; i--)
        siftdown(i);  // ���µ�����
    return ;
}

// �����Ѷ�Ԫ��
void minheap::pop()
{
    heap[1] = heap[cnt--];  // ���Ѷ��ڵ������һ���ڵ㽻���������ٶѴ�С
    siftdown(1);            // ������
    return ;
}

// ����в���һ���½ڵ㣬�����ֶѵ�����
void minheap::push(node a)
{
    heap[++cnt] = a;  // ���½ڵ�����ĩβ
    int root = cnt;
    int father = root / 2;
    
    // ���ϵ����ѣ�ֱ���ѵ����ʱ�����
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

// ��ȡ�Ѷ�Ԫ��
node& minheap::front()
{
    return heap[1];
}

// �Ƚ������ڵ��Ȩ��
bool minheap::Comp(const node& a, const node& b)
{
    return a.weight < b.weight;  // ���a��Ȩ�ش���b��Ȩ�أ�����false
}

// ���������ڵ��λ��
void minheap::swap(node& a, node& b)
{
    int index = a.id;
    int wgt = a.weight;
    
    a.id = b.id; a.weight = b.weight;
    b.id = index; b.weight = wgt;
    return ;
}

// ���µ�����
void minheap::siftdown(int index)
{
    int root = index;
    int l = root << 1, r = root << 1 | 1;
    
    while(l <= cnt)
    {
        // �����������������������Ȩ��С������������ѡ��������
        if(r <= cnt && Comp(heap[r], heap[l]))
            l = r;
        
        // �����ǰ�ڵ��Ȩ�ش��ڵ������������������
        if(Comp(heap[root], heap[l]))
            return ;
        
        swap(heap[root], heap[l]);  // �����ڵ�
        
        root = l;  // ���¸��ڵ�Ϊ��ǰ�ӽڵ�
        l = root << 1; r = root << 1 | 1;  // ���������ӽڵ�
    }
    return ;
}

// ���ضѵĴ�С
int minheap::size()
{
    return cnt;
}
/*
������������

    ����·�������Ǵ�����������ÿһҶ�ӽ���·������֮��.�ڽ����Ŀ��ͬ�Ķ�������,��ȫ��������·��������̡�

����Ȩ��������������н���һ����ĳ�������ʵ����

���Ĵ�Ȩ·�����ȣ���㵽����֮���·��������ý����Ȩ�ĳ˻���

���Ĵ�Ȩ·������(Weighted Path Length of Tree)������Ϊ��������Ҷ���Ĵ�Ȩ·������֮��,ͨ����Ϊ��

T=��Wi��li  i=1,2��������n   i��ʾҶ�ӽڵ�ı�š�

���У�n��ʾҶ�ӽ�����ĿWi��li�ֱ��ʾҶ���i��Ȩֵ�͸������i֮���·�����ȡ����Ĵ�Ȩ·���������Ϊ���Ĵ��ۡ�

���ڸ���n��Ҷ�ӽڵ��Ȩֵ�����һ�Ŷ�������ʹ����ö������Ĵ�Ȩ·������WPL��С��������������С��Ȩ·�����ȡ�

����3��Ҷ�ӽڵ��Ȩֵ��1,2,3���������С��Ȩ·��������ͼC_1��ʾ������С��Ȩ·������WPL=1��2+2��2+3��1=9��


��������ʽ��

�ж���������ݣ�ÿ���������ռ���У���һ����һ��������n����ʾҶ�ӽڵ������ n=0��ζ������������Ҳ���Ҫ����

�ڶ�����n������������ʾÿ��Ҷ�ӽڵ��ȨֵWi�����е�Wi��10000��

40%�Ĳ�������3 �� n�� 10

30%�Ĳ�������3 �� n�� 102

20%�Ĳ�������3 �� n�� 103

10%�Ĳ�������3 �� n�� 104

�������ʽ��

    ����ÿ��������ݣ����һ�У�����һ������������ʾ��С��Ȩ·������WPL���������ݱ�֤��ȷ�Ľ��������231��

���������롿


3
1 2 3
4
1 1 1 1 
0
����������� 


9
8
*/
