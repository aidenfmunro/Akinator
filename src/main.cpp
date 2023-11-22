#include "tree.h"
#include "akinator.h"

int main(void)
{
    Tree tree = {};

    ConstructTree(&tree, "tree.txt");

    FILE* fp = fopen("out.txt", "w+");

    PrintTree(tree.root, fp);

    fclose(fp);

    DumpTreeGraph(tree.root);

    DestroyTree(&tree);

    return OK;

}