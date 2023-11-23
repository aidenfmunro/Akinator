#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"

const int MAX_STR_SIZE = 100;

typedef char Key;

enum Keys
{
    NO          = 'n',
    YES         = 'y',
    GUESS       = 'g',
    DEFINITION  = 'd',
    COMPARE     = 'c',
    DATABASE    = 'l',
    QUIT        = 'q',
};


ErrorCode Menu(const char* basefilename);

ErrorCode ProcessMode(const char* basefilename);

ErrorCode ChooseMode(char key);

Key getKey();

ErrorCode ConstructTree(Tree* tree, const char* basefilename);

ErrorCode getDefinition(Tree* tree, const char* name);

#endif