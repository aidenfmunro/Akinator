#include "tree.h"
#include "akinator.h"

int main(void)
{
    Tree tree = {};

    ConstructTree(&tree, "tree.txt");

    DumpTreeTxt(&tree, "out.txt");

    DumpTreeGraph(tree.root);

    DestroyTree(&tree);

    return OK;

}