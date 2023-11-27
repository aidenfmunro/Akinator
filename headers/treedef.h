#ifndef TREEDEF_H
#define TREEDEF_H

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

#endif