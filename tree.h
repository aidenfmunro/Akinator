#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int ErrorCode;

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
    char* data;

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

#endif