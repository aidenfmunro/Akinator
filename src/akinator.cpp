#include "akinator.h"
#include "textfuncs.h"
#include "stackfuncs.h"
#include "utils.h"

const int MAX_KEY_SIZE = 256;

Node*      _recursiveReadTree (Tree* tree, Text* base, size_t* curTokenNum);
Node*      _recursiveReadNode (Tree* tree, Text* base, size_t* curTokenNum);
Node*      _createNode        (NodeElem_t data, Node* left, Node* right);
bool       _searchNode        (const char* name, Node* node, Stack* path);
ErrorCode  addQuestion        (Tree* tree, Node* node, const char* basefilename);

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

    Key key = getKey();

    return ChooseMode(key, basefilename);

}

Key getKey(void)
{
    Key key = 0;

    while (scanf("%c", &key) == 1 && (key != GUESS)      &&
                                     (key != DEFINITION) &&
                                     (key != COMPARE   ) &&
                                     (key != DATABASE  ) &&
                                     (key != QUIT      ) &&
                                     (key != YES       ) &&
                                     (key != NO        )) 
    {
        bufferCleaner();
        printf("Unknown key: %c! Please try again\n", key);
    }

    bufferCleaner();

    return key;
}

ErrorCode ChooseMode(Key key, const char* basefilename)
{
    switch (key)
    {
        case GUESS:
        {
            return Guess(basefilename);
        }
        case DEFINITION:
        {
            return Definition(basefilename);
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
            return OK;
        }
        
        default:
            printf("Unknown mode: %c! Please try again\n", key);
            break;
    }

    return UNKNOWN_MODE;
}

ErrorCode ConstructTree(Tree* tree, const char* basefilename)
{
    AssertSoft(tree,         NULL);
    AssertSoft(basefilename, NULL);

    Text base = {};

    CreateText(&base, basefilename, NONE);

    // --------> DEBUG INFO
    /*
    printf("%d\n", base.numTokens);

    for (size_t i = 0; i < base.numTokens; i++)
    {
        printf("token [%d]: %s, %d\n", i, base.tokens[i].string, base.tokens[i].length);
    }
    */

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

    if (strcmp(token->string, "nil") == 0)
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
    
    strcpy(data, token->string);

    if (StringIsEmpty(token))
    {
        tree->error = UNRECOGNISED_TOKEN;
        AssertSoft(! tree->error, NULL);
    }

    Node* leftSubTree  = _recursiveReadTree(tree, base, curTokenNum);

    Node* rightSubTree = _recursiveReadTree(tree, base, curTokenNum); 

    Node* newNode      = _createNode(data, leftSubTree, rightSubTree);

    tree->size++; 

    token = &base->tokens[(*curTokenNum)++]; // DSL

    const char* closeBracket = strchr(token->string, ')'); // TODO: closeBracket = token->string, closeBracke == ')'

    if (!closeBracket)
    {
        tree->error = SYNTAX_ERROR;

        return NULL;
    }

    return newNode;
}

Node* _createNode(NodeElem_t data, Node* left, Node* right) // TODO: put in tree.cpp
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

ErrorCode Definition(const char* basefilename)
{
    Tree tree = {};

    ConstructTree(&tree, basefilename);

    Stack path = {};

    SafeCalloc(name, MAX_STR_SIZE, char, NULL_PTR);

    printf("Type in the name that you want to find:\n\n");
    scanf("%s", name); 
    printf("\n");

    CreateStack(path);

    bool isNodeFound = _searchNode(name, tree.root, &path);

    if (! isNodeFound) // wrap into function 
    {
        printf("The name doesn't exist!\n");
    }
    else
    {
        Node* curNode = Pop(&path);

        printf("%d\n", path.size);

        printf("%s ", curNode->data);

        for (size_t i = path.size; i >= 0; i--)
        {
            curNode = Pop(&path);

            printf("-> %s ", curNode->data);
        }

        printf("\n\n");
    }

    free(name);

    DestroyTree(&tree);

    DestroyStack(&path);

    Menu(basefilename);

    return OK;
}

bool _searchNode(const char* name, Node* node, Stack* path) // TODO: pop if subtree doesn't contain name, 1 way down
{
    AssertSoft(name, NULL);

    if (! node)
        return false;

    Push(path, node);

    if (strcmp(node->data, name) == 0)
        return true;

    if (_searchNode(name, node->left, path) || _searchNode(name, node->right, path))
        return true;

    Pop(path);

    return false;
}

ErrorCode Guess(const char* basefilename)
{
    AssertSoft(basefilename, NULL_PTR);

    Tree tree = {};

    ConstructTree(&tree, basefilename);

    Node* curNode = tree.root;

    char answer = 0;

    while (curNode)
    {
        if (curNode->right == NULL && curNode->left == NULL)
        {   
            printf("Is it %s?\n", curNode->data);

            answer = getKey();

            if (answer == NO)
            {
                printf("Would you like to add your answer?\n");

                answer = getKey();

                if (answer == NO)
                {
                    Menu(basefilename);
                }
                else
                {
                    addQuestion(&tree, curNode, basefilename);
                }
            }
            else if (answer == YES)
            {
                printf(YELLOW "Gotcha!\n" COLOR_RESET);
            }

            break;
        }

        printf("%s [y] or [n] \n", curNode->data);

        answer = getKey();

        if (answer == NO)
        {
            curNode = curNode->left;
        }
        else if (answer == YES)
        {
            curNode = curNode->right;
        }
    }
    
    // DestroyTree(&tree);

    return OK;
}

ErrorCode addQuestion(Tree* tree, Node* node, const char* basefilename) // TODO: rename to add question 
{
    AssertSoft(tree, NULL_PTR);                                         // TODO: remove tree from parametr
    AssertSoft(node, NULL_PTR);
    AssertSoft(basefilename, NULL_PTR);

    printf("what's the difference between %s and your answer?"
            BOLD " (write in question form)" COLOR_RESET "\n\n", node->data);

    char question[MAX_STR_SIZE] = {};

    char answer[MAX_STR_SIZE]   = {};

    scanf("%s", question);

    SafeCalloc(tempDataAnswer, MAX_STR_SIZE, char, NO_MEMORY);

    strcpy(tempDataAnswer, answer);

    printf("what's your answer? \n\n");
    
    scanf("%s", answer);

    SafeCalloc(tempDataQuestion, MAX_STR_SIZE, char, NO_MEMORY);

    strcpy(tempDataQuestion, question);

    Node* tempNode2 = _createNode(node->data, NULL, NULL);

    node->left = tempNode2;

    node->data = tempDataQuestion;

    Node* tempNode1 = _createNode(answer, NULL, NULL);

    node->right = tempNode1;

    DumpTreeTxt(tree, basefilename);

    // DumpTreeGraph(tree->root);

    return OK;
}
