#include "tree.h"
#include "akinator.h"

int main(void)
{
    FILE* fp = fopen("tree.txt", "w+");

    Tree tree = {};

    CreateTree(&tree);

    Insert(&tree, tree.root, "abobaaaaaaaaaaaaaaaaa", RIGHT);

    Insert(&tree, tree.root, "ASd", LEFT);

    DumpTreeGraph(tree.root, "treegraph.dot");

    PrintTree(tree.root, fp);

    fclose(fp);

    ConstructTree(&tree, "tree.txt");

    DestroyTree(&tree);

    return OK;
}