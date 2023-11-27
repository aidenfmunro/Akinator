#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "treedef.h"
#include "stackfuncs.h"

enum Error
{
    OK,
    NULL_PTR,
    NO_MEMORY,
    REPEAT_INSERT_LEFT,
    REPEAT_INSERT_RIGHT,
    TREE_LINKING_ERROR,
    UNKNOWN_POSITION,
    UNABLE_TO_OPEN_FILE,
    UNRECOGNISED_TOKEN,
    SYNTAX_ERROR,
    UNKNOWN_NAME,
    UNKNOWN_MODE,
    INDEX_OUT_OF_RANGE
};

// ErrorCode CreateTree(Tree* tree);

ErrorCode DestroyTree(Tree* tree);

ErrorCode DumpTreeGraph(Node* node);

ErrorCode DumpTreeTxt(Tree* tree, const char* filename);

ErrorCode VerifyTree(Tree* tree);

ErrorCode deleteNode(Node* node);

Node* createNode_      (NodeElem_t data, Node* left, Node* right);

bool  searchNode_      (const char* name, Node* node, Stack* path);

ErrorCode connectNode(Node* node, Node* leftChild, Node* rightChild);

#endif