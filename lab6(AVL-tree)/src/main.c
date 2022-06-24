#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct node
{ //структура для узлов дерева
    struct node *right;
    struct node *left;
    int val;
    unsigned char height;
} AVLnode;

unsigned char Height(AVLnode *p)
{ //высота для узла дерева
    return p ? p->height : 0;
}

int BalanceFactor(AVLnode *p)
{ //разница между высотами поддеревьев
    return Height(p->right) - Height(p->left);
}

void FixHeight(AVLnode *p)
{
    unsigned char hl = Height(p->left);
    unsigned char hr = Height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

AVLnode *RightRotate(AVLnode *p)
{ //правый  поворот
    AVLnode *q = p->left;
    p->left = q->right;
    q->right = p;
    FixHeight(p);
    FixHeight(q);
    return q;
}

AVLnode *LeftRotate(AVLnode *q)
{ //левый поворот
    AVLnode *p = q->right;
    q->right = p->left;
    p->left = q;
    FixHeight(q);
    FixHeight(p);
    return p;
}

AVLnode *Balance(AVLnode *p)
{ //балансировка
    FixHeight(p);
    if (BalanceFactor(p) == 2)
    {
        if (BalanceFactor(p->right) < 0)
            p->right = RightRotate(p->right);
        return LeftRotate(p);
    }
    if (BalanceFactor(p) == -2)
    {
        if (BalanceFactor(p->left) > 0)
            p->left = LeftRotate(p->left);
        return RightRotate(p);
    }
    return p; //балансировка не нужна
}

AVLnode *CreateNode(AVLnode *tree_node, int value)
{ //создание нового узла
    if (tree_node != NULL)
    {
        tree_node->val = value;
        tree_node->left = tree_node->right = NULL;
        tree_node->height = 1;
    }
    return tree_node;
}

AVLnode *Insert(AVLnode *p, AVLnode *tree_node, int k)
{ //вставка ключа к в дерево с вершиной p на место tree_node
    if (!p)
        return CreateNode(tree_node, k);
    if (k < p->val)
        p->left = Insert(p->left, tree_node, k);
    else
        p->right = Insert(p->right, tree_node, k);
    return Balance(p);
}

int main(void)
{
    int n;
    if (!scanf("%d", &n))
        return 0;

    if (n == 0)
    {
        printf("0");
        return 0;
    }

    AVLnode *tree = (AVLnode *)malloc(sizeof(AVLnode) * n);
    if (!tree)
    {
        free(tree);
        return EXIT_SUCCESS;
    }

    int tmp;
    if (!scanf("%d", &tmp))
    {
        free(tree);
        return EXIT_SUCCESS;
    }
    tree[0].val = tmp;
    tree[0].height = 1;
    tree[0].left = tree[0].right = NULL;

    AVLnode *head = &tree[0];

    for (int i = 1; i < n; i++)
    {
        if (!scanf("%d", &tmp))
        {
            free(tree);
            return EXIT_SUCCESS;
        }
        head = Insert(head, &tree[i], tmp);
    }
    printf("%d", Height(head));
    free(tree);
    return EXIT_SUCCESS;
}
