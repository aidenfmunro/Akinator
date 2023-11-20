#include "tree.h"

int main(void)
{
    FILE* fp = fopen("tree.txt", "w+");

    Tree tree = {};

    CreateTree(&tree);

    Node* node = Insert(&tree, tree.root, "aboba", RIGHT);

    printf("%p\n", node);

    node = Insert(&tree, node, "bruh", LEFT);

    printf("%p\n", node);

    node = Insert(&tree, node, "omegalul", LEFT);

    printf("%p\n", node);

    DumpTreeGraph(tree.root, "treegraph.dot");

    PrintTree(tree.root, fp);

    // DestroyTree(&tree);

    fclose(fp);

}