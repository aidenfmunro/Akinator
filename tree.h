#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int ErrorCode;

#define SPECIFIER "%d"

enum Error
{
    OK,
    NULL_PTR,
    NO_MEMORY,
    REPEAT_INSERT_LEFT,
    REPEAT_INSERT_RIGHT,
    TREE_LINKING_ERROR
};

struct String
{
    char* str;
    size_t length;
};

struct Node
{
    /* char* */ int data;

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

ErrorCode CreateTree(Tree* tree);

ErrorCode DestroyTree(Tree* tree);

Node* InsertLeft(Tree* tree, Node* node, /* char* */ int data);

Node* InsertRight(Tree* tree, Node* node, /* char* */ int data);

ErrorCode PrintTree(Node* node, FILE* outFile);

ErrorCode PrintTreeGraph(Node* node, FILE* outFile);

ErrorCode checkTreeLinks(Tree* tree, Node* node, size_t* counter);

ErrorCode deleteNode(Node* node);

#endif