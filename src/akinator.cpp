#include "akinator.h"
#include "textfuncs.h"
#include "stackfuncs.h"
#include "utils.h"

const int MAX_KEY_SIZE = 256;

ErrorCode  ConstructTree      (Tree* tree, const char* basefilename);
Node*      _recursiveReadTree (Tree* tree, Text* base, size_t* curTokenNum);
Node*      _recursiveReadNode (Tree* tree, Text* base, size_t* curTokenNum);
Node*      _createNode        (NodeElem_t data, Node* left, Node* right);
ErrorCode  _searchName        (Tree* tree, const char* name, Stack* path);
Node*      _searchNode        (const char* name, Node* node);

#define BOLD  "\e[1m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

ErrorCode Menu(const char* basefilename)
{
    AssertSoft(basefilename, NULL_PTR);

    printf("\n" BOLD  "Добро пожаловать в игру Акинатор!                    \n\n" COLOR_RESET
                GREEN "Выберите соответствующий режим:                      \n\n" COLOR_RESET
                      "Отгадывание:          ["BLUE"g"COLOR_RESET"]         \n\n"
                      "Определение:          ["BLUE"d"COLOR_RESET"]         \n\n"
                      "Сравнение объектов:   ["BLUE"c"COLOR_RESET"]         \n\n"
                      "Просмотр базы данных: ["BLUE"l"COLOR_RESET"]         \n\n"
                      "Выход:                ["BLUE"q"COLOR_RESET"]         \n\n");

    return ProcessMode(basefilename);
}

ErrorCode ProcessMode(const char* basefilename)
{
    AssertSoft(basefilename, NULL_PTR);

    int key = getKey();

    return ChooseMode(key);

}

Key getKey(void)
{
    char key = 0;

    scanf("%c", &key);
    getchar(); 

    while ((key != GUESS     ) &&
           (key != DEFINITION) &&
           (key != COMPARE   ) &&
           (key != DATABASE  ) &&
           (key != QUIT      ) &&
           (key != YES       ) &&
           (key != NO        )) 
    {
        printf("Unknown key: %c! Please try again\n", key);
        
        scanf("%c", &key);
        getchar(); 
    }

    return key;
}


ErrorCode ChooseMode(char key)
{
    switch (key)
    {
        case GUESS:
        {
            break;
        }
        case DEFINITION:
        {
            break;
        }
        case COMPARE:
        {
            break;
        }
        case DATABASE:
        {
            break;
        }
        case QUIT:
        {
            printf("Goodbye!\n");
            return QUIT;
            break;
        }
        
        default:
            printf("Unknown mode: %c! Please try again\n", key);
            break;
    }

    return OK;
}

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

    tree->size++; 

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
        left->parent  = newNode; 
        
    if (right)
        right->parent = newNode;
    
    newNode->left  = left;
    
    newNode->right = right;

    return newNode;    
}

ErrorCode getDefinition(Tree* tree, const char* name)
{
    Stack path = {};

    CreateStack(path);

    tree->error = _searchName(tree, name, &path);

    AssertSoft(! tree->error, UNKNOWN_NAME);

    Node* curNode = tree->root;

    printf("%d\n", path.size);

    printf("%s ", curNode->data);

    for (size_t i = 0; i <= path.size; i++)
    {
        int turn = Pop(&path);

        if (turn == 1)
        {
            curNode = curNode->right;
        }
        else if (turn == 0)
        {
            curNode = curNode->left;
        }

        printf("-> %s ", curNode->data);
    }

    DestroyStack(&path);

    return OK;
}

ErrorCode _searchName(Tree* tree, const char* name, Stack* path)
{
    AssertSoft(tree, NULL_PTR);
    AssertSoft(name, NULL_PTR);
    AssertSoft(path, NULL_PTR);

    Node* curNode = _searchNode(name, tree->root);

    if (!curNode)
        return UNKNOWN_NAME;

    while (curNode != tree->root)
    {
        if      (curNode == curNode->parent->left)
        {
            Push(path, 0);
        }

        else if (curNode == curNode->parent->right)
        {
            Push(path, 1);
        }

        curNode = curNode->parent;
    }

    return OK;
}

Node* _searchNode(const char* name, Node* node)
{
    AssertSoft(name, NULL);

    if (!node)
    {
        return NULL;
    }    

    if (strcmp(node->data, name) == 0)
    {
        return node;
    }

    Node* leftSubTree = _searchNode(name, node->left);

    if (leftSubTree)
    {
        return leftSubTree;
    }

    Node* rightSubTree = _searchNode(name, node->right);

    if (rightSubTree)
    {
        return rightSubTree;
    }

    return NULL;
}