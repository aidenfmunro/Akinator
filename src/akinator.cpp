#include "akinator.h"
#include "textfuncs.h"
#include "utils.h"

ErrorCode ConstructTree  (Tree* tree, const char* basefilename);
Node*     _recursiveReadTree (Tree* tree, Text* base, size_t* curTokenNum);
Node*     _recursiveReadNode (Tree* tree, Text* base, size_t* curTokenNum);
Node*     _createNode(NodeElem_t data, Node* left, Node* right);

ErrorCode ConstructTree(Tree* tree, const char* basefilename)
{
    AssertSoft(tree,         NULL);
    AssertSoft(basefilename, NULL);

    Text base = {};

    CreateText(&base, basefilename, NONE);

    printf("%d\n", base.numTokens);

    for (size_t i = 0; i < base.numTokens; i++)
    {
        printf("token [%d]: %s\n", i, base.tokens[i].string);
    }

    size_t curTokenNum = 0;

    tree->root = _recursiveReadTree(tree, &base, &curTokenNum);

    DestroyText(&base);

    return OK;   
}

Node* _recursiveReadTree(Tree* tree, Text* base, size_t* curTokenNum)
{   
    AssertSoft(tree,                           NULL);
    AssertSoft(base,                           NULL);
    AssertSoft(*curTokenNum < base->numTokens, NULL);

    const Token* token = &base->tokens[(*curTokenNum)++];

    const char* openBracket = strchr(token->string, '(');
    if (openBracket)
        return _recursiveReadNode(tree, base, curTokenNum);

    const char* nil = strstr(token->string, "nil");

    if (nil)
        return NULL;
    
    tree->error = SYNTAX_ERROR;

    return NULL;
}

Node* _recursiveReadNode(Tree* tree, Text* base, size_t* curTokenNum)
{
    AssertSoft(tree,                           NULL);
    AssertSoft(base,                           NULL);
    AssertSoft(*curTokenNum < base->numTokens, NULL);

    const Token* token = &base->tokens[(*curTokenNum)++];

    int countChars = 0;

    SafeCalloc(data, token->length + 1, char, NULL);

    if (sscanf(token->string, SPECIFIER "%n", data, &countChars) != 1 || StringIsEmpty(token))
    {
        tree->error = UNRECOGNISED_TOKEN;
        return NULL;
    }

    Node* leftSubTree  = _recursiveReadTree(tree, base, curTokenNum);

    Node* rightSubTree = _recursiveReadTree(tree, base, curTokenNum); 

    Node* newNode      = _createNode(data, leftSubTree, rightSubTree);

    token = &base->tokens[(*curTokenNum)++];

    const char* closeBracket = strchr(token->string, ')');

    if (!closeBracket)
    {
        tree->error = SYNTAX_ERROR;
        return NULL;
    }

    return newNode;
}

Node* _createNode(NodeElem_t data, Node* left, Node* right)
{
    SafeCalloc(newNode, 1, Node, NULL);

    newNode->data = data;

    if (left)
        left->parent  = newNode; // TODO: acquire parents 
    if (right)
        right->parent = newNode;
    
    newNode->left = left;
    
    newNode->right = right;

    return newNode;    
}