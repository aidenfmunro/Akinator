#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>


const int MAX_STR_SIZE = 100;

struct String
{
    char* str;
    size_t length;
};

struct Node
{
    String str;

    Node* parent;

    Node* left;
    Node* right;
};

struct Tree
{
    Node* root;
    size_t size;    
};  


#endif