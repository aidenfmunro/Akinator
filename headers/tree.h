#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef int ErrorCode;

#define STRING_T

#ifdef INT_T
    #define SPECIFIER "%d"
    typedef int NodeElem_t;
#endif

#ifdef FLOAT_T
    #define SPECIFIER "%f"
    typedef float NodeElem_t;
#endif

#ifdef DOUBLE_T
    #define SPECIFIER "%lg"
    typedef double NodeElem_t;
#endif

#ifdef CHAR_T
    #define SPECIFIER "%c"
    typedef char NodeElem_t;
#endif

#ifdef STRING_T
    #define SPECIFIER "%s"
    typedef char* NodeElem_t;
    const int MIN_SIZE_STR  = 7;
#endif

struct String
{
    char* string;
    size_t length;
};

struct Node
{
    NodeElem_t data;

    Node* parent;

    Node* left;

    Node* right;
};

struct Tree
{
    Node* root;
    
    size_t size;  

    ErrorCode error;  
};  

enum Error
{
    OK,
    NULL_PTR,
    NO_MEMORY,
    REPEAT_INSERT_LEFT,
    REPEAT_INSERT_RIGHT,
    TREE_LINKING_ERROR
};

enum Side
{
    LEFT,
    RIGHT
};

ErrorCode CreateTree(Tree* tree);

ErrorCode DestroyTree(Tree* tree);

ErrorCode PrintTree(Node* node, FILE* outFile);

ErrorCode DumpTreeGraph(Node* node, const char* filename);

Node* Insert(Tree* tree, Node* node, NodeElem_t data, Side side);

ErrorCode PrintTree(Node* node, FILE* outFile);

ErrorCode deleteNode(Node* node);

#endif