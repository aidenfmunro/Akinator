#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tree.h"

const int MAX_STR_SIZE = 100;

ErrorCode ConstructTree(Tree* tree, const char* basefilename);

ErrorCode getDefinition(Tree* tree, const char* name);

#endif