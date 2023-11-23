#include "tree.h"
#include "akinator.h"

int main(void)
{
    Menu("tree1.txt");
    
    Tree tree = {};

    ConstructTree(&tree, "tree1.txt");

    DumpTreeTxt(&tree, "out.txt");

    DumpTreeGraph(tree.root);

    DestroyTree(&tree);

    return OK;

}