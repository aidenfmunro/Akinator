#include "tree.h"

int main(void)
{
    FILE* fp = fopen("tree.txt", "w+");

    FILE* graph = fopen("treegraph.dot", "w+");

    Tree tree = {};

    CreateTree(&tree);

    Node* node = InsertRight(&tree, tree.root, 10);

    node = InsertRight(&tree, node, 20);

    InsertLeft(&tree, node, 30);

    InsertRight(&tree, node, 50);

    InsertLeft(&tree, tree.root, 40);

    PrintTree(tree.root, fp);

    fprintf(graph, "digraph binary_tree {\n");
    fprintf(graph, "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n");
    fprintf(graph, "  rankdir = \"TB\";\n\n");
    fprintf(graph, "  label = \"Tree Dump\";\n");

    PrintTreeGraph(tree.root, graph);
    fprintf(graph, "}");
    fclose(graph);


    DestroyTree(&tree);

    fclose(fp);

}