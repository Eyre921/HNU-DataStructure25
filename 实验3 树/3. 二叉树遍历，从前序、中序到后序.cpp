#include<bits/stdc++.h>
using namespace std;

string result;  // 用于存储最终的后序遍历结果

// 递归构建后序遍历
void buildPostOrder(string& preorder, string& inorder) {
	if (inorder.size() < 1) // 如果中序遍历为空，说明已经遍历到叶子节点
		return;

	char root = preorder[0];  // 前序遍历的第一个节点就是根节点
	string leftInorder, rightInorder;
	int rootIndex = inorder.find(root);  // 在中序遍历中找到根节点的位置

	// 将中序遍历分为左子树和右子树的部分
	for (int i = 0; i < rootIndex; i++)
		leftInorder.push_back(inorder[i]);  // 左子树的中序部分

	for (int i = rootIndex + 1; i < int(inorder.size()); i++)
		rightInorder.push_back(inorder[i]);  // 右子树的中序部分

	preorder.erase(preorder.begin());  // 删除前序遍历中的根节点

	// 递归构建左子树和右子树的后序遍历
	buildPostOrder(preorder, leftInorder);
	buildPostOrder(preorder, rightInorder);

	result.push_back(root);  // 将当前根节点加入后序遍历结果中
}

int main() {
	while (true) {
		int n;
		cin >> n;  // 输入节点数

		if (n == 0) // 如果n为0，结束程序
			return 0;

		string preorder, inorder;
		cin >> preorder >> inorder;  // 输入前序遍历和中序遍历

		result.clear();  // 清空结果字符串
		buildPostOrder(preorder, inorder);  // 调用函数构建后序遍历

		cout << result << '\n';  // 输出后序遍历结果
	}
	return 0;
}

/*
【问题描述】

二叉树是一种非常重要的数据结构，非常多其他数据结构都是基于二叉树的基础演变而来的。对于二叉树，深度遍历有前序、中序以及后序三种遍历方法。

三种基本的遍历思想为：

前序遍历：根结点 ---> 左子树 ---> 右子树

中序遍历：左子树---> 根结点 ---> 右子树

后序遍历：左子树 ---> 右子树 ---> 根结点

比如，求以下二叉树的各种遍历

hhh.png

前序遍历：1  2  4  5  7  8  3  6

中序遍历：4  2  7  5  8  1  3  6

后序遍历：4  7  8  5  2  6  3  1

需要你编写程序解决的问题是：已知一个二叉树的前序遍历和中序遍历的结果，给出该二叉树的后序遍历的结果。

【输入形式】

有多组测试数据，每组测试数据三行，每组测试数据第一行只有一个正整数n，表示二叉树节点的数目，n=0意味着输入结束并且不需要处理。

每组测试数据第二行是二叉树的前序遍历的结果，是一个长度为n的字符串，每个节点由一个字符表示，字符是大小写英文字母及10个数字,不同的节点用不同的字符表示，也即无论前序遍历和中序遍历的字符串中没有重复的字符。

每组测试数据第二行是二叉树的中序遍历的结果，也是一个长度为n的字符串。

40%的测试数据1 ≤ n≤ 10；

30%的测试数据1 ≤ n≤ 20；


20%的测试数据1 ≤ n≤ 40；

10%的测试数据1 ≤ n≤ 62；

【输出形式】

对于每组测试数据，输出一行，是一个长度为n的字符串，表示二叉树后序遍历的结果。

【样例输入】


8
12457836
42758136
4
abcd
abcd
4
abcd
dcba
0
【样例输出】


47852631
dcba
dcba
*/
