#include "tree.h"
#include "akinator.h"

int main(void)
{
    Tree tree = {};

    ConstructTree(&tree, "tree.txt");

    DumpTreeTxt(&tree, "out.txt");

    DumpTreeGraph(tree.root);

    printf("%d\n", tree.size);

    DestroyTree(&tree);

    return OK;

}