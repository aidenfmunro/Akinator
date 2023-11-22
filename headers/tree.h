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
    TREE_LINKING_ERROR,
    UNKNOWN_POSITION,
    UNABLE_TO_OPEN_FILE,
    UNRECOGNISED_TOKEN,
    SYNTAX_ERROR
};

ErrorCode CreateTree(Tree* tree);

ErrorCode DestroyTree(Tree* tree);

ErrorCode DumpTreeGraph(Node* node);

ErrorCode DumpTreeTxt(Tree* tree, const char* filename);

ErrorCode VerifyTree(Tree* tree);

ErrorCode deleteNode(Node* node);

#endif