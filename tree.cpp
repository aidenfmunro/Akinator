#include "tree.h"
#include "utils.h"

ErrorCode CreateTree(Tree* tree)
{
    AssertSoft(tree, NULL_PTR);

    SafeCalloc(tempNode, 1, Node, NO_MEMORY);

    tree->root = tempNode;
    tree->size = 0;

    tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->parent = NULL;

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

Node* insertLeft(Tree* tree, Node* node, char* data)
{
    AssertSoft(tree, NULL);
    AssertSoft(node, NULL);

    if (tree->size == 0)
    {
        SafeCalloc(tempData, strlen(data) + 1, char, NULL);

        tree->root->data = tempData;
        
        strcpy(tree->root->data, data);

        tree->size++;

        return tree->root;
    }

    if (! node->left)
    {
        tree->error = REPEAT_INSERT_LEFT;

        return NULL;
    }

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->left = NULL;
    newNode->right = NULL;

    SafeCalloc(newData, strlen(data) + 1, char, NULL);

    newNode->data = newData;

    strcpy(newNode->data, newData);

    node->left = newNode;
    newNode->parent = node;

    tree->size++;

    return newNode;
}

Node* insertRight(Tree* tree, Node* node, char* data)
{
    AssertSoft(tree, NULL);
    AssertSoft(node, NULL);

    if (tree->size == 0)
    {
        SafeCalloc(tempData, strlen(data) + 1, char, NULL);

        tree->root->data = tempData;

        strcpy(tree->root->data, data);

        tree->size++;

        return tree->root;
    }

    if (! node->right)
    {
        tree->error = REPEAT_INSERT_RIGHT;

        return NULL;
    }

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->left = NULL;
    newNode->right = NULL;

    SafeCalloc(newData, strlen(data) + 1, char, NULL);

    newNode->data = newData;

    strcpy(newNode->data, newData);

    node->right = newNode;
    newNode->parent = node;

    tree->size++;

    return newNode;
}

ErrorCode checkTreeLinks(Tree* tree, Node* node, size_t* counter)
{
    AssertSoft(tree, NULL_PTR);
    
    AssertSoft(node, NULL_PTR); // should I assert here?

    (*counter)++;

    if (! node->left && (node->left->parent != node || node->right->parent != node))
    {
        tree->error = TREE_LINKING_ERROR;
        return TREE_LINKING_ERROR;
    }

    if (node->left)
        checkTreeLinks(tree, node->left, counter);
    
    if (node->right)
        checkTreeLinks(tree, node->right, counter);

    return OK;
}

ErrorCode PrintTree(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);

    if (! node)
    {
        fprintf(outFile, "nil");
        return OK;
    }

    fprintf(outFile, "(");

    PrintTree(node->left, outFile);

    fprintf(outFile, "%s", node->data);

    PrintTree(node->right, outFile);

    fprintf(outFile, ")");

    return OK;
}

ErrorCode PrintTreeGraph(Node* node, FILE* outFile)
{
    if (! node->left && ! node->right)
    {
        
    }
}


