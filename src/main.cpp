#include "tree.h"
#include "akinator.h"

int main(void)
{
    Menu("ss");
    
    Tree tree = {};

    ConstructTree(&tree, "tree.txt");

    DumpTreeTxt(&tree, "out.txt");

    DumpTreeGraph(tree.root);

    printf("%d\n", tree.size);

    getDefinition(&tree, "aiden");

    DestroyTree(&tree);

    return OK;

}