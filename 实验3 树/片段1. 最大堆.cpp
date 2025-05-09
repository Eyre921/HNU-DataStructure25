#include  <bits/stdc++.h>

using  namespace  std;

class  heap {
	private:
		int  n;  // 记录堆中元素的数量
		int  *elem;  // 存储堆元素的数组
		void  shiftdown(int  pos) {  // 下沉操作，保证堆的性质

			int left = leftChild(pos);  // 获取左子节点的位置
			int right = rightChild(pos);  // 获取右子节点的位置
			int largest = pos;  // 假设当前节点是最大的
			if (left < n && elem[left] > elem[largest]) largest = left;  // 如果左子节点更大，更新largest
			if (right < n && elem[right] > elem[largest]) largest = right;  // 如果右子节点更大，更新largest
			if (largest != pos) {  // 如果子节点更大，则交换并继续下沉
				swap(elem[pos], elem[largest]);
				shiftdown(largest);  // 递归下沉
			}

		}
	public:
		heap() {}  // 默认构造函数
		heap(int  m) {  // 构造函数，指定堆的大小
			n = m;
			elem = new  int[n];  // 动态分配堆数组
		}
		~heap() {  // 析构函数，释放内存
			delete[]  elem;
		}
		void  initial() {  // 初始化堆数组，输入堆元素
			for (int  i = 0;  i < n;  i++)
				cin >> elem[i];  // 输入元素
		}

		void  out() {  // 输出堆数组
			for (int  i = 0;  i < n;  i++) {
				cout << elem[i];  // 输出元素
				if  (i <= n - 2)  // 判断是否是最后一个元素
					cout << "  ";  // 如果不是，输出空格
				else
					cout << endl;  // 如果是最后一个元素，换行
			}
		}

		bool  isLeaf(int  pos)  const {  // 判断给定位置是否为叶子节点
			return  ((pos >= n / 2)  &&  pos < n);  // 叶子节点的位置范围
		}

		int  leftChild(int  pos) {  // 返回左子节点的位置
			return  2 * pos + 1;
		}

		int  rightChild(int  pos) {  // 返回右子节点的位置
			return  2 * pos + 2;
		}

		void  arrange() {  // 调整堆，使其满足堆性质
			for (int  i = (n - 1) / 2; i >= 0;  i--) {  // 从最后一个非叶子节点开始下沉
				shiftdown(i);
			}
		}

		void  insertElem(int  x) {  // 插入新元素到堆中

			elem[n] = x;  // 将新元素放到数组的末尾
			n++;  // 堆的大小加一
			for (int i = n - 1; i >= 0; i--) shiftdown(i);  // 从插入位置开始下沉调整堆
		}
};

int  main() {
	int  m;
	cin >> m;  // 输入堆的大小
	heap  *ex = new  heap(m);  // 创建堆对象
	ex->initial();  // 初始化堆
	ex->arrange();  // 调整堆
	ex->out();  // 输出堆
	int  insData;
	cin >> insData;  // 输入待插入的元素
	ex->insertElem(insData);  // 插入元素
	ex->out();  // 输出插入后的堆
	return  0;
}

/*
1. 最大堆
【问题描述】

     堆在实际应用中有非常重要的作用，例如优先队列和堆排序。

     堆实际上也是一个完全二叉树，而完全二叉树可以使用一个一维数组来保存，如下图a所示，对于一个具有n个节点的堆(完全二叉树)， 其顶点按照从上而下从左至右的顺序排列保存到数组中，下标为i=0、1、2、...、n-1。

微信图片_20240402100829.jpg

     很显然，对于数组下标为k的元素(顶点)，其子节点的下标分别为2k+1和2k+2，其父节点的下标为(k-1)/2。

     对于输入的一组数据，可以将其视为一个完全二叉树的顶点，以下代码将会将该二叉树整理为一个最大堆，同时也实现了向堆中插入元素的代码。

     请将相关代码补充完整，可参考教材115页有关的代码。

     其中类方法shiftdown用于实现对当前位置的元素向下递推，insertElem实现向堆中插入新的节点。
【输入形式】

    输入的第一行为一个整数n，表示最开始时创建的完全二叉树顶点元素的个数

    接下来的一行为n个整数，表示最开始时创建的完全二叉树顶点元素，其顺序为从上而下从左至右

    接下来的一行为一个整数，表示向创建的最大堆中插入的新元素。

【输出形式】

    输出为两行，第一行n个数，表示创建的最大堆的顶点值，其顺序为从上而下从左至右， 第二行n+1个数，表示最大堆中插入元素后的顶点值，其顺序为从上而下从左至右

【样例输入】

10
20 12 35 15 10 80 30 17 2 1
75
【样例输出】

80 17 35 15 10 20 30 12 2 1
80 75 35 15 17 20 30 12 2 1 10
*/
