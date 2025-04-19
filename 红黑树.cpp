const static bool RED = 0;
const static bool BLACK = 1;
struct Node
{
    int val;
    bool color; //颜色
    Node *left,*right, *p; //左，右孩子，父节点
    Node(const int &v,const bool &c=RED,Node *l=nullptr, Node *r=nullptr, Node *_p = nullptr):
        val(v),color(c),left(l),right(r),p(_p){}
};
struct RBTree
{
    Node *root; //树根
    Node *nil; //外部节点, color: 黑色
    RBTree()
    {
        nil = new Node(-1, BLACK, nil, nil, nil);
        root = nil; }
};

