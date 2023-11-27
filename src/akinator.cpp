#include "akinator.h"
#include "textfuncs.h"
#include "stackfuncs.h"
#include "utils.h"
#include "locale.h"
#include "colors.h"

const int MAX_KEY_SIZE = 256;

static ErrorCode connectNode(Node* node, Node* leftChild, Node* rightChild);

static Node* readTree_        (Tree* tree, Text* base, size_t* curTokenNum);

static Node* reaNode_         (Tree* tree, Text* base, size_t* curTokenNum);

static Node* createNode_      (NodeElem_t data, Node* left, Node* right);

static bool  searchNode_      (const char* name, Node* node, Stack* path);

ErrorCode    addQuestion      (Node* node);

ErrorCode    processMode      (Tree* tree, const char* baseFileName);

ErrorCode    giveDefinition   (Tree* tree, const char* baseFileName);

ErrorCode    guess            (Tree* tree, const char* baseFileName);

ErrorCode    compare          (Tree* tree, const char* baseFileName);

ErrorCode    givePartNameDef  (Stack* path, size_t index);

ErrorCode    chooseMode       (Key key, Tree* tree, const char* baseFileName);

Key          getKey           (void);

char*        getAnswer        (void);

ErrorCode Menu(const char* baseFileName)
{
    AssertSoft(baseFileName, NULL_PTR);

    printf("\n" BOLD  "Добро пожаловать в игру Акинатор!                    \n\n" COLOR_RESET
                GREEN "Выберите соответствующий режим:                      \n\n" COLOR_RESET
                      "Отгадывание:          ["BLUE"g"COLOR_RESET"]         \n\n"
                      "Определение:          ["BLUE"d"COLOR_RESET"]         \n\n"
                      "Сравнение объектов:   ["BLUE"c"COLOR_RESET"]         \n\n"
                      "Просмотр базы данных: ["BLUE"l"COLOR_RESET"]         \n\n"
                      "Выход:                ["BLUE"q"COLOR_RESET"]         \n\n");

    Tree tree = {};

    ConstructTree(&tree, baseFileName);

    processMode(&tree, baseFileName);

    DestroyTree(&tree);

    return OK;
}

ErrorCode processMode(Tree* tree, const char* baseFileName)
{
    AssertSoft(baseFileName, NULL_PTR);

    Key key = getKey();

    chooseMode(key, tree, baseFileName);

    return OK;
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

ErrorCode chooseMode(Key key, Tree* tree, const char* baseFileName)
{
    switch (key)
    {
        case GUESS:
        {
            return guess(tree, baseFileName);
        }
        case DEFINITION:
        {
            return giveDefinition(tree, baseFileName);
        }
        case COMPARE:
        {
            return compare(tree, baseFileName);
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

ErrorCode ConstructTree(Tree* tree, const char* baseFileName)
{
    AssertSoft(tree,         NULL);
    AssertSoft(baseFileName, NULL);

    Text base = {};

    CreateText(&base, baseFileName, NONE);

    // --------> DEBUG INFO
    /*
    printf("%d\n", base.numTokens);

    for (size_t i = 0; i < base.numTokens; i++)
    {
        printf("token [%d]: %s, %d\n", i, base.tokens[i].string, base.tokens[i].length);
    }
    */

    size_t curTokenNum = 0;

    tree->root = readTree_(tree, &base, &curTokenNum);

    DestroyText(&base);

    return OK;   
}

static Node* readTree_(Tree* tree, Text* base, size_t* curTokenNum)
{   
    AssertSoft(tree,                           NULL);
    AssertSoft(base,                           NULL);
    AssertSoft(*curTokenNum < base->numTokens, NULL);

    const Token* token = &base->tokens[(*curTokenNum)++];

    const char* openBracket = strchr(token->string, '(');

    if (openBracket)
        return reaNode_(tree, base, curTokenNum);

    if (strcmp(token->string, "nil") == 0)
        return NULL;
    
    tree->error = SYNTAX_ERROR;

    return NULL;
}

static Node* reaNode_(Tree* tree, Text* base, size_t* curTokenNum)
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

    Node* leftSubTree  = readTree_(tree, base, curTokenNum);

    Node* rightSubTree = readTree_(tree, base, curTokenNum); 

    Node* newNode      = createNode_(data, leftSubTree, rightSubTree);

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

static Node* createNode_(NodeElem_t data, Node* left, Node* right) // TODO: put in tree.cpp
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

ErrorCode giveDefinition(Tree* tree, const char* baseFileName)
{
    AssertSoft(tree,         NULL_PTR);
    AssertSoft(baseFileName, NULL_PTR);

    Stack path = {};

    printf("Type in the name that you want to find:\n\n");

    char* name = getAnswer();

    CreateStack(path);

    bool isNodeFound = searchNode_(name, tree->root, &path);

    if (! isNodeFound) // wrap into function 
    {
        printf("The name doesn't exist!\n");
    }
    else
    {
        Node* curNode = Pop(&path);

        printf("%s это: ", curNode->data);

        size_t pathSize = path.size;

        for (size_t i = 0; i <= pathSize + 2; i++)
        {

            if (curNode->parent)
            {
                if (curNode->parent->right == curNode)
                {
                    printf("%s ", curNode->parent->data);
                }
                else if (curNode->parent->left == curNode)
                {
                    printf("не %s ", curNode->parent->data);
                }
            }

            curNode = Pop(&path);
        }

        printf("\n\n");
    }

    free(name);

    DestroyStack(&path);

    Menu(baseFileName);

    return OK;
}

static bool searchNode_(const char* name, Node* node, Stack* path) // TODO: pop if subtree doesn't contain name, 1 way down
{
    AssertSoft(name, false);
    AssertSoft(path, false);

    if (! node)
        return false;

    Push(path, node);

    if (strcmp(node->data, name) == 0)
        return true;

    if (searchNode_(name, node->left, path) || searchNode_(name, node->right, path))
        return true;

    Pop(path);

    return false;
}

ErrorCode guess(Tree* tree, const char* baseFileName)
{
    AssertSoft(baseFileName, NULL_PTR);
    AssertSoft(tree,         NULL_PTR);

    Node* curNode = tree->root;

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

                if (answer == YES)
                {
                    addQuestion(curNode);
                }
            }
            else if (answer == YES)
            {
                printf(YELLOW "Gotcha!\n" COLOR_RESET);
            }

            break;
        }

        printf("%s? [y] or [n] \n", curNode->data);

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

    DumpTreeTxt(tree, baseFileName);

    DumpTreeGraph(tree->root);

    Menu(baseFileName);

    return OK;
}

ErrorCode compare(Tree* tree, const char* baseFileName)
{
    AssertSoft(tree,         NULL_PTR);
    AssertSoft(baseFileName, NULL_PTR);

    Stack path1 = {};
    Stack path2 = {};

    CreateStack(path1);
    CreateStack(path2);

    printf("Please type in the name of the 2 objects\n\n");

    char* name1 = getAnswer();

    char* name2 = getAnswer();

    bool isFoundNode1 = searchNode_(name1, tree->root, &path1);

    bool isFoundNode2 = searchNode_(name2, tree->root, &path2);

    size_t index1 = 0;

    size_t index2 = 0;

    if (isFoundNode1 && isFoundNode2)
    {   
        printf("\nThe similarities are: ");

        while (path1.data[index1 + 1] == path2.data[index2 + 1] && (index1 < path1.size - 1)
                                                                && (index2 < path2.size - 1))
        {
            printf("%s ", path1.data[index1]->data);

            index1++;
            index2++;
        }

        printf("\nThe differences are ");

        printf("\n%s: ", name1);

        givePartNameDef(&path1, index1);

        printf("\n%s: ", name2);

        givePartNameDef(&path2, index2);
    }
    else
    {
        printf("One of the names you typed doesn't exist!\n\n");
    }

    DestroyStack(&path1);

    DestroyStack(&path2);

    free(name1);

    free(name2);

    Menu(baseFileName);

    return OK;
}

ErrorCode givePartNameDef(Stack* path, size_t index)
{
    for (; index < path->size - 1; index++)
        {
            if (path->data[index]->left == path->data[index + 1])
            {
                printf("not %s ", path->data[index]->data);
            }
            if (path->data[index]->right == path->data[index + 1])
            {
                printf("%s ", path->data[index]->data);
            }
        }
}

ErrorCode addQuestion(Node* node) 
{
    AssertSoft(node,         NULL_PTR);

    printf("what's the difference between %s and your answer?"
           "\n\n", node->data);

    char* difference = getAnswer();

    printf("what's your answer? \n\n");

    char* answer = getAnswer();

    connectNode(node, createNode_(node->data, NULL, NULL), createNode_(answer, NULL, NULL));

    node->data = difference;

    return OK;
}

char* getAnswer(void)
{
    char answer[MAX_STR_SIZE] = {};

    scanf("%s", answer);

    SafeCalloc(tempAnswer, MAX_STR_SIZE, char, NULL);

    strcpy(tempAnswer, answer);

    return tempAnswer;
}

static ErrorCode connectNode(Node* node, Node* leftChild, Node* rightChild)
{
    AssertSoft(node, NULL_PTR);

    node->left = leftChild;

    if (leftChild)
        leftChild->parent = node;

    node->right = rightChild;
    
    if (rightChild)
        rightChild->parent = node;

    return OK;
}
