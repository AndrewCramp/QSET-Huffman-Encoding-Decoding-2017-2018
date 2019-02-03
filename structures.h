struct node{
    char characterValue; //Character value if node is a terminal node
    float value;
    int hasParent;
    int number;
    struct node *rightChild; //Value of 1 in binary tree
    struct node *leftChild;  //Value of 0 in binary tree
};

