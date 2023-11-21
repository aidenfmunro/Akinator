#include "akinator.h"
#include "textfuncs.h"
#include "utils.h"

Node* _createNode(Tree* tree, Node* node, char* buffer, int* counter);

ErrorCode ConstructTree(Tree* tree, const char* basefilename)
{
    AssertSoft(tree,         NULL);
    AssertSoft(basefilename, NULL);

    Text base = {};

    CreateText(&base, basefilename, NONE);

    for (size_t i = 0; i < base.numLines; i++)
    {
        printf("token [%d]: %s\n", i, base.lines[i].string);
    }

    size_t curToken = 0;

    DestroyText(&base);

    return OK;   
}

Node* _recursiveRead(Text* base, size_t* curToken)
{   
    AssertSoft(base, NULL);
    AssertSoft(*curToken < base->numLines, NULL);
}

Node* _createNode(Tree* tree, Node* node, char* buffer, int* counter)
{
    AssertSoft(tree,   NULL);
    AssertSoft(buffer, NULL);

    SafeCalloc(newNode, 1, Node, NULL);

    (tree->size)++;


}
