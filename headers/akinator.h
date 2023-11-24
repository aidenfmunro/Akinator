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


ErrorCode Menu(const char* basefilename); // TODO: change to camel

ErrorCode ProcessMode(const char* basefilename);

ErrorCode Definition(const char* basefilename);

ErrorCode Guess(const char* basefilename);

ErrorCode ChooseMode(Key key, const char* basefilename);

Key getKey();

ErrorCode ConstructTree(Tree* tree, const char* basefilename);


#endif