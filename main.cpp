#include "tree.h"

int main(void)
{
    FILE* fp = fopen("tree.txt", "w+");

    Tree tree = {};

    CreateTree(&tree);

    Node* node = InsertRight(&tree, tree.root, 10);

    node = InsertRight(&tree, node, 20);

    node = InsertLeft(&tree, node, 30);

    PrintTree(tree.root, fp);

    DestroyTree(&tree);

    fclose(fp);
}