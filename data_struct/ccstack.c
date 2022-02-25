#include "ccstack.h"
#include "common.h"
#include <string.h>
#include <stdio.h>

#define INITIAL 50

static void ReverseStack(CC_STACK* StackToReverse)
{
    int* arr = (int*)malloc(StackToReverse->Count * sizeof(int));
    int index = 0;
    int val;

    while (StIsEmpty(StackToReverse) != 1)
    {
        StPop(StackToReverse, &val);
        arr[index] = val;
        index++;
    }
    for (int i = 0; i < index; i++)
    {
        StPush(StackToReverse, arr[i]);
    }
}
//end of help functions


int StCreate(CC_STACK** Stack)
{
    if (NULL != *Stack)
    {
        printf("Already created!\n");
        return -1;
    }

    CC_STACK* st = NULL;
    if (NULL == Stack)
    {
        return -1;
    }
    st = (CC_STACK*)malloc(sizeof(CC_STACK));
    if (NULL == st)
    {
        return -1;
    }
    memset(st, 0, sizeof(*st));

    st->Count = 0;
    st->Size = INITIAL;
    st->Array = (int*)malloc(INITIAL * sizeof(int));
    if (NULL == st->Array)
    {
        free(st);
        return -1;
    }
    *Stack = st;

    return 0;
}

int StDestroy(CC_STACK** Stack)
{
    CC_STACK* st = *Stack;
    if (NULL == st)
    {
        return -1;
    }
    free(st->Array);
    free(st);
    *Stack = NULL;

    return 0;
}

int StPush(CC_STACK* Stack, int Value)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (Stack->Count >= Stack->Size)
    {
        int* newSize = (int*)realloc(Stack->Array, (Stack->Size + 50) * sizeof(int));
        if (NULL == newSize)
        {
            printf("Reallocation failed\n");
            free(newSize);
            return -1;
        }
        else
        {
            Stack->Size += 50;
            Stack->Array = newSize;
        }
    }

    Stack->Array[Stack->Count] = Value;
    Stack->Count++;

    return 0;
}

int StPop(CC_STACK* Stack, int* Value)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (Stack->Count > 0)
    {
        Stack->Array[Stack->Count] = 0;
        Stack->Count--;
        *Value = Stack->Array[Stack->Count];
    }
    else if (Stack->Count <= 0)
    {
        printf("Stack is empty\n");
        return -1;
    }


    return 0;
}

int StPeek(CC_STACK* Stack, int* Value)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (Stack->Count <= 0)
    {
        printf("Stack is empty!\n");
        return -1;
    }

    *Value = Stack->Array[Stack->Count - 1];

    return 0;
}

int StIsEmpty(CC_STACK* Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (Stack->Count > 0)
    {
        return 0;
    }
    else if (Stack->Count == 0)
    {
        return 1;
    }
    else
        return -1;
}

int StGetCount(CC_STACK* Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    if (Stack->Count < 0)
    {
        return -1;
    }

    return Stack->Count;
}

int StClear(CC_STACK* Stack)
{
    if (NULL == Stack)
    {
        return -1;
    }
    Stack->Count = 0;
    return 0;
}

int StPushStack(CC_STACK* Stack, CC_STACK* StackToPush)
{
    if (NULL == Stack || NULL == StackToPush)
    {
        return -1;
    }
    if (StackToPush->Count <= 0)
    {
        return -1;
    }

    int val = 0;
    ReverseStack(StackToPush);

    while (StIsEmpty(StackToPush) != 1)
    {
        StPop(StackToPush, &val);
        StPush(Stack, val);
    }

    return 0;
}

