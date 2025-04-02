#include<bits/stdc++.h>
using namespace std;

// 断言工具函数：如果val为假则输出错误信息并中断程序
void Assert(bool val, string s) {
    if (!val) {
        cout << "断言失败：" << s << endl;
    }
}

template <typename E> class List { // 定义线性表抽象数据类型（ADT）
private:
    void operator =(const List&) {}      // 防止赋值操作
    List(const List&) {}           // 防止拷贝构造函数
public:
    List() {}          // 默认构造函数
    virtual ~List() {} // 基类析构函数

    // 清空列表内容，使其变为空
    virtual void clear() = 0;

    // 在当前元素位置插入一个元素
    // item: 要插入的元素
    virtual void insert(const E& item) = 0;

    // 在列表末尾添加一个元素
    // item: 要添加的元素
    virtual void append(const E& item) = 0;

    // 删除并返回当前元素
    // 返回值：被删除的元素
    virtual E remove() = 0;

    // 将当前位置设置为列表的开始位置
    virtual void moveToStart() = 0;

    // 将当前位置设置为列表的末尾位置
    virtual void moveToEnd() = 0;

    // 将当前位置向左移动一步。如果已经在开始位置，则不变
    virtual void prev() = 0;

    // 将当前位置向右移动一步。如果已经在末尾位置，则不变
    virtual void next() = 0;

    // 返回列表中的元素个数
    virtual int length() const = 0;

    // 返回当前元素的位置
    virtual int currPos() const = 0;

    // 设置当前的位置
    // pos: 要设置的目标位置
    virtual void moveToPos(int pos) = 0;

    // 返回当前元素
    virtual const E& getValue() const = 0;
};

// 单向链表节点定义
template <typename E> class Link {
public:
    E element;      // 当前节点的值
    Link *next;     // 指向下一个节点的指针
    // 构造函数
    Link(const E& elemval, Link* nextval = NULL) {
        element = elemval;
        next = nextval;
    }
    Link(Link* nextval = NULL) {
        next = nextval;
    }
};

template <typename E> class LList: public List<E> {
private:
    Link<E>* head;       // 指向链表头部的指针
    Link<E>* tail;       // 指向链表尾部的指针
    Link<E>* curr;       // 当前元素的指针
    int cnt;             // 链表中元素的个数

    // 初始化辅助方法
    void init() {
        curr = head = tail = new Link<E>;
        cnt = 0;
    }

    // 清除所有节点，释放内存
    void removeall() {
        while (head != NULL) {
            curr = head;
            head = head->next;
            delete curr;
        }
    }

public:
    LList(int size = 100) {
        init();    // 构造函数，初始化链表
    }
    ~LList() {
        removeall();    // 析构函数，清除链表
    }

    // 打印链表中所有元素，元素之间用空格分隔，最后换行
    // 如果链表为空，则直接输出一个空行
    void print() {
        curr = head->next;  // 从第一个有效元素开始
        while (curr != NULL) {
            cout << curr->element << " ";  // 输出当前元素
            curr = curr->next;  // 移动到下一个节点
        }
        cout << endl;  // 打印换行
    }

    // 清空链表并重新初始化
    void clear() {
        removeall();    // 清除链表
        init();         // 重新初始化链表
    }

    // 在当前元素位置插入元素it
    void insert(const E& it) {
        curr->next = new Link<E>(it, curr->next);  // 创建新节点并插入
        if (tail == curr)  // 如果当前元素是尾部节点，更新尾部
            tail = curr->next;
        cnt++;  // 增加链表的元素个数
    }

    // 在链表末尾添加元素it
    void append(const E& it) {
        tail = tail->next = new Link<E>(it, NULL);  // 创建新节点并添加到尾部
        cnt++;  // 增加链表的元素个数
    }

    // 删除并返回当前元素
    E remove() {
        // 如果当前元素不存在，则断言失败
        Assert(curr->next != NULL, "没有元素");
        E it = curr->next->element;  // 获取当前元素的值
        Link<E>*ltemp = curr->next;  // 临时保存当前元素
        if (tail == curr->next)  // 如果要删除的是尾部元素，更新尾部
            tail = curr;
        curr->next = ltemp->next;  // 更新当前节点的指针
        delete ltemp;  // 删除原节点
        cnt--;  // 减少链表的元素个数
        return it;  // 返回被删除的元素
    }

    // 将当前位置设置为链表的起始位置
    void moveToStart() {
        curr = head;
    }

    // 将当前位置设置为链表的末尾位置
    void moveToEnd() {
        curr = tail;
    }

    // 将当前位置向左移动一步；如果已经在头部则不变
    void prev() {
        if (curr == head) return;  // 如果已经在头部，则不移动
        Link<E>*temp = head;
        while (temp->next != curr)  // 找到当前元素的前一个节点
            temp = temp->next;
        curr = temp;  // 更新当前元素的位置
    }

    // 将当前位置向右移动一步；如果已经在尾部则不变
    void next() {
        if (curr != tail)  // 如果当前元素不是尾部，则向右移动
            curr = curr->next;
    }

    // 返回链表中的元素个数
    int length() const  {
        return cnt;
    }

    // 返回当前元素的位置
    int currPos() const {
        Link<E>*temp = head;
        int i;
        for (i = 0; curr != temp; i++)  // 遍历链表找到当前位置
            temp = temp->next;
        return i;  // 返回当前位置
    }

    // 将当前位置移动到指定的位置pos
    void moveToPos(int pos) {
        // 检查位置是否有效
        Assert((pos >= 0) && (pos <= cnt), "位置越界");
        curr = head;
        for (int i = 0; i < pos; i++)  // 移动到指定位置
            curr = curr->next;
    }

    // 返回当前元素的值
    const E& getValue() const {
        // 如果当前元素不存在，则断言失败
        Assert(curr->next != NULL, "没有值");
        return curr->next->element;  // 返回当前元素的值
    }
};

// 读取测试指令并进行测试
void test(LList<int> &llist) {
    int act;
    int pos, value;
    do {
        // 读取指令说明：1 插入 2 删除 3 获取值 4 查找
        cin >> act;
        switch (act) {
            case 1:  // 在pos位置插入值value
                cin >> pos >> value;
                llist.moveToPos(pos);  // 移动到指定位置
                llist.insert(value);   // 插入元素
                llist.print();         // 打印链表
                break;
            case 2:  // 删除pos位置的元素
                cin >> pos;
                llist.moveToPos(pos);  // 移动到指定位置
                llist.remove();        // 删除元素
                llist.print();         // 打印链表
                break;
            case 3:  // 获取指定位置的值
                cin >> pos;
                llist.moveToPos(pos);  // 移动到指定位置
                cout << llist.getValue() << endl;  // 输出当前值
                break;
            case 4:  // 查询特定值所在位置，如果存在则输出位置，否则不输出
                cin >> value;
                for (llist.moveToStart(); llist.currPos() < llist.length(); llist.next()) {
                    if (llist.getValue() == value) {
                        cout << llist.currPos() << endl;  // 输出位置
                        break;
                    }
                }
                break;
            case 0:
                return;
        }
    } while (true);
}

int main() {
    LList <int> llist;
    test(llist);  // 执行测试
    return 0;
}
