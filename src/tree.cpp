#include "tree.h"
#include "stackfuncs.h"
#include "utils.h"

static int DUMP_NUM = 0;

const int MAX_COMMAND_LENGTH = 256;

const int MAX_FILENAME_LENGTH = 256;

static ErrorCode _dumpTreeDot(Node* node, FILE* outFile);

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

    tree->size = 1;

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

    free(node->data);

    free(node); 

    return OK;
}

ErrorCode DestroyTree(Tree* tree)
{
    AssertSoft(tree, NULL_PTR);

    deleteNode(tree->root);

    tree->root = NULL;

    tree->error = 0;

    tree->size = 0;

    return OK;
}

Node* Insert(Tree* tree, Node* node, NodeElem_t data, Side side) // TODO: dsl for left and right 
{                                                   
    AssertSoft(tree, NULL);

    if (side == RIGHT && node->right)
    {
        tree->error = REPEAT_INSERT_RIGHT; 

        return NULL;
    }

    if (side == LEFT && node->left)
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

    if      (side == LEFT)
        node->left = newNode;
    else if (side == RIGHT)
        node->right = newNode;
    else
    {
        tree->error = UNKNOWN_POSITION;

        return NULL;
    }
    
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
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    if (node == NULL)
    {
        fprintf(outFile, "nil ");
        return OK;
    }

    fprintf(outFile, "( ");

    fprintf(outFile, ""SPECIFIER" ", node->data); 

    PrintTree(node->left, outFile);

    PrintTree(node->right, outFile);

    fprintf(outFile, ") ");

    return OK;
}

#define FONT_COLOR "\"#000000\""

#define BACKGROUND_PARENT_COLOR "\"aqua\""

#define BACKGROUND_CHILD_COLOR "\"coral\""

#define DUMP_NAME "\"Tree Dump\""

#define SHAPE "\"circle\""

#define STYLE "\"filled\""

#define SPACE_BETWEEN_CONTENTS "\"0.075\""

#define RANK "\"TB\""

#define dumpGraph(...) fprintf(outFile, __VA_ARGS__);

ErrorCode DumpTreeGraph(Node* node)
{
    char filename[MAX_FILENAME_LENGTH] = "";

    sprintf(filename, "log/dot/treegraph_%d.dot", DUMP_NUM);

    myOpen(filename, "w", outFile);

    dumpGraph("  digraph tree"
              "  {\n"
              "  node[ "
              "  shape     = "SHAPE"," 
              "  style     = "STYLE"," 
              "  fillcolor = "BACKGROUND_PARENT_COLOR"," 
              "  fontcolor = "FONT_COLOR","
              "  margin    = "SPACE_BETWEEN_CONTENTS"];\n"
              "  rankdir   = "RANK";\n\n"
              "  label     = "DUMP_NAME";\n");

    _dumpTreeDot(node, outFile);

    dumpGraph("  }");

    myClose(outFile);

    char command[MAX_COMMAND_LENGTH] = "";

    sprintf(command, "dot -Tpng log/dot/treegraph_%d.dot -o log/img/treeimg_%d.png", DUMP_NUM, DUMP_NUM);

    system(command);

    DUMP_NUM++;

    return OK;
}

static ErrorCode _dumpTreeDot(Node* node, FILE* outFile) // TODO: wtf make it more clean, // create_node, connect_node
{ 
    if (node->left == NULL && node->right == NULL)
    {
        dumpGraph(" \"" SPECIFIER "\" [shape = \"record\", fillcolor = "BACKGROUND_CHILD_COLOR"," 
                  " label = \"{<name>" SPECIFIER " | parent\\n%p | <f0> address\\n%p|"
                  " {<left>left\\n%p | <right>right\\n%p\\n}}\"];\n", 
                  node->data,
                  node->data, node->parent, node,
                  node->left, node->right);
    }    
    else
    {
        dumpGraph(" \"" SPECIFIER "\" [shape = \"record\"," 
                  " label = \"{<name>" SPECIFIER " | parent\\n%p | <f0> address\\n%p|"
                  " {<left>left\\n%p | <right>right\\n%p\\n}}\"];\n",
                  node->data, 
                  node->data, node->parent, node, 
                  node->left, node->right);
    }
    
    if (node->left != NULL)
    {
        if (node->left->parent == node)
        {
            dumpGraph("  \"" SPECIFIER "\":left->\"" SPECIFIER "\";\n", node->data, node->left->data);
            // fprintf(outFile, "  \"" SPECIFIER "\":name->\"" SPECIFIER "\";\n", node->left->data, node->data);
        }
        else
        {
            dumpGraph("  edge [color=\"#FE6200\"];\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n"
                      "  edge [color=\"#000000\"];\n",
                      node->data, node->left->data,
                      node->left->data, node->left->parent->data);
        }

        _dumpTreeDot(node->left, outFile); // TODO: get rid off copy paste bruh
    }

    if (node->right != NULL)
    {
        if (node->right->parent == node)
        {
            dumpGraph("  \"" SPECIFIER "\":right->\"" SPECIFIER "\";\n", node->data, node->right->data);
           //  fprintf(outFile, "  \"" SPECIFIER "\":name->\"" SPECIFIER "\";\n", node->right->data, node->data);
        }
        else
        {
            dumpGraph("  edge [color=\"#FE6200\"];\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n" 
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n",
                      "  edge [color=\"#000000\"];\n", 
                      node->data, node->right->data, node->right->data, node->right->parent->data);
        }

        _dumpTreeDot(node->right, outFile);
    }
    
    return OK;
}

static ErrorCode verifyTree(Tree* tree) // TODO: make proper verify 
{
    size_t count = 0;
    checkTreeLinks(tree, tree->root, &count);

    return OK;
} 