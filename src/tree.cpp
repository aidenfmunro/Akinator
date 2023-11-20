#include "tree.h"
#include "stackfuncs.h"
#include "utils.h"

static ErrorCode printTreeGraph(Node* node, FILE* outFile);

static ErrorCode verifyTree(Tree* tree);

static ErrorCode checkTreeLinks(Tree* tree, Node* node, size_t* counter);

ErrorCode CreateTree(Tree* tree)
{
    AssertSoft(tree, NULL_PTR);

    SafeCalloc(tempNode, 1, Node, NO_MEMORY);

    tree->root = tempNode;
    tree->size = 0;

    tree->root->left   = NULL;
    tree->root->right  = NULL;
    tree->root->parent = NULL;

    SafeCalloc(tempData, MIN_SIZE_STR, char, NO_MEMORY);

    tempData = "WHO?";

    tree->root->data = tempData;

    tree->error = 0;

    return OK;
}

ErrorCode deleteNode(Node* node)
{
    if (node->left)
        deleteNode(node->left);
    
    if (node->right)
        deleteNode(node->right);

    node->right  = NULL;
    node->left   = NULL;
    node->parent = NULL;

    free(node); 

    free(node->data);

    return OK;
}

ErrorCode DestroyTree(Tree* tree)
{
    AssertSoft(tree, NULL);

    deleteNode(tree->root);

    tree->root = NULL;

    tree->error = 0;

    tree->size = 0;

    return OK;
}

Node* Insert(Tree* tree, Node* node, NodeElem_t data, Side side) // TODO: dsl for left and right 
{                                                   
    AssertSoft(tree, NULL);

    if (tree->size == 0)
    {
        tree->root->data = data;

        tree->size++;

        return tree->root;
    }

    if (node->right)
    {
        tree->error = REPEAT_INSERT_RIGHT; 

        return NULL;
    }

    if (node->right)
    {
        tree->error = REPEAT_INSERT_LEFT;

        return NULL;
    }

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->left = NULL;
    newNode->right = NULL;

    SafeCalloc(newData, strlen(data) + 1, char, NULL);

    newNode->data = newData;

    strcpy(newNode->data, data);

    if (side == LEFT)
        node->left = newNode;
    else if (side == RIGHT)
        node->right = newNode;
    
    newNode->parent = node;

    tree->size++;

    return newNode;
}

static ErrorCode checkTreeLinks(Tree* tree, Node* node, size_t* counter)
{
    AssertSoft(tree, NULL_PTR);
    
    AssertSoft(node, NULL_PTR);  // TODO: counter check

    (*counter)++; // TODO: make it in 

    if (node->left != NULL && (node->left->parent != node || node->right->parent != node))
    {
        tree->error = TREE_LINKING_ERROR;
        return TREE_LINKING_ERROR;
    }

    if (node->left != NULL)
        checkTreeLinks(tree, node->left, counter);
    
    if (node->right != NULL)
        checkTreeLinks(tree, node->right, counter);

    return OK;
}

ErrorCode searchNode(const char* name, Node* node, Stack* path)
{
    AssertSoft(name, NULL_PTR);
    AssertSoft(node, NULL_PTR);
    AssertSoft(path, NULL_PTR);
    
}



ErrorCode PrintTree(Node* node, FILE* outFile) // TODO: create tree with txt file
{
    if (node == NULL)
    {
        fprintf(outFile, "nil");
        return OK;
    }

    fprintf(outFile, "(");

    PrintTree(node->left, outFile);

    fprintf(outFile, " "SPECIFIER" ", node->data); 

    PrintTree(node->right, outFile);

    fprintf(outFile, ")");

    return OK;
}

ErrorCode DumpTreeGraph(Node* node, const char* filename)
{
    FILE* outFile = fopen(filename, "w+");

    fprintf(outFile, "  digraph tree"
                     "  {\n"
                     "  node [shape = \"circle\", style = \"filled\", fillcolor = \"blue\", fontcolor = \"#FFFFFF\", margin = \"0.01\"];\n"
                     "  rankdir = \"TB\";\n\n"
                     "  label = \"Tree Dump\";\n");

    printTreeGraph(node, outFile);

    fprintf(outFile, "  }");

    fclose(outFile);

    return OK;
}

// #define outFile "graph.txt"

// #define dumpGraph(...) fprintf(outFile, __VA_ARGS__);

static ErrorCode printTreeGraph(Node* node, FILE* outFile) // TODO: wtf make it more clean
{ // create_node, connect_node
    if (node->left == NULL && node->right == NULL)
        fprintf(outFile, "  \"" SPECIFIER "\" [shape = \"record\", fillcolor = \"red\", label = \"{" SPECIFIER " | parent\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", node->data, node->data, node->parent, node, node->left, node->right);
    else
        fprintf(outFile, "  \"" SPECIFIER "\" [shape = \"record\", label = \"{" SPECIFIER " | parent\\n%p | <f0> pos\\n%p| left\\n%p | right\\n%p\\n}\"];\n", node->data, node->data, node->parent, node, node->left, node->right);

    if (node->left != NULL)
    {
        if (node->left->parent == node)
        {
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->data, node->left->data);
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->left->data, node->data);
        }
        else
        {
            fprintf(outFile, "  edge [color=\"#FE6200\"];\n");
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->data, node->left->data);
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->left->data, node->left->parent->data); // dump(...)
            fprintf(outFile, "  edge [color=\"#000000\"];\n");
        }

        printTreeGraph(node->left, outFile); // TODO: get rid off copy paste bruh
    }

    if (node->right != NULL)
    {
        if (node->right->parent == node)
        {
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->data, node->right->data);
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->right->data, node->data);
        }
        else
        {
            fprintf(outFile, "  edge [color=\"#FE6200\"];\n");
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->data, node->right->data);
            fprintf(outFile, "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n", node->right->data, node->right->parent->data);
            fprintf(outFile, "  edge [color=\"#000000\"];\n");
        }

        printTreeGraph(node->right, outFile);
    }
    
    return OK;
}

static ErrorCode verifyTree(Tree* tree) // TODO: make proper verify 
{
    size_t count = 0;
    checkTreeLinks(tree, tree->root, &count);

    return OK;
} 