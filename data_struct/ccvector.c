#include <stdio.h>
#include "ccvector.h"
#include "common.h"
#include "string.h"

#define INITIAL_SIZE    50

static int CompareFunction(const void* NumA, const void* NumB)
{
    return (*(int*)NumB - *(int*)NumA);
}
//end of help functions


int VecCreate(CC_VECTOR** Vector)
{
    if (NULL != *Vector)
    {
        printf("Already created\n");
        return -1;
    }

    CC_VECTOR* vec = NULL;
    if (NULL == Vector)
    {
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (NULL == vec)
    {
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc(sizeof(int) * INITIAL_SIZE);
    if (NULL == vec->Array)
    {
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}

int VecDestroy(CC_VECTOR** Vector)
{
    CC_VECTOR* vec = *Vector;

    if (NULL == Vector)
    {
        return -1;
    }

    free(vec->Array);
    free(vec);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR* Vector, int Value)
{

    if (NULL == Vector)
    {
        return -1;
    }
    //realocare
    if (Vector->Count >= Vector->Size)
    {
        int* newSize = (int*)realloc(Vector->Array, (Vector->Size + 50) * sizeof(int));
        if (NULL == newSize)
        {
            printf("Vector reallocation failed\n");
            return -1;
        }
        else
        {
            Vector->Size += 50;
            Vector->Array = newSize;
        }
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertHead(CC_VECTOR* Vector, int Value)
{

    if (NULL == Vector)
    {
        return -1;
    }
    //realocarea
    if (Vector->Count >= Vector->Size)
    {
        int* newSize = (int*)realloc(Vector->Array, (Vector->Size + 50) * sizeof(int));
        if (NULL == newSize)
        {
            printf("Vector reallocation failed\n");
            return -1;
        }
        else
        {
            Vector->Size += 50;
            Vector->Array = newSize;
        }
    }

    for (int i = Vector->Count - 1; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[0] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertAfterIndex(CC_VECTOR* Vector, int Index, int Value)
{

    if (NULL == Vector)
    {
        return -1;
    }

    //realocarea
    if (Vector->Count >= Vector->Size)
    {
        int* newSize = (int*)realloc(Vector->Array, (Vector->Size + 50) * sizeof(int));
        if (NULL == newSize)
        {
            printf("Vector reallocation failed\n");
            return -1;
        }
        else
        {
            Vector->Size += 50;
            Vector->Array = newSize;
        }
    }

    for (int i = Vector->Count - 1; i >= Index; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[Index + 1] = Value;
    Vector->Count++;

    return 0;
}

int VecRemoveByIndex(CC_VECTOR* Vector, int Index)
{
    if (NULL == Vector)
    {
        return -1;
    }
    if (Index > Vector->Count || Index < 0)
    {
        return -1;
    }

    for (int i = Index; i < Vector->Count; i++)
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }
    Vector->Count--;


    return 0;
}

int VecGetValueByIndex(CC_VECTOR* Vector, int Index, int* Value)
{
    if (NULL == Vector)
    {
        return -1;
    }
    if (Vector->Count < Index || Index < 0)
    {
        printf("Out of range\n");
        return -1;
    }
    *Value = Vector->Array[Index];
    return 0;
}

int VecGetCount(CC_VECTOR* Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }
    if (Vector->Count < 0)
    {
        printf("Vector is empty\n");
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR* Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }
    //    for (int i = 0; i < Vector->Count; i++)
    //        Vector->Array[i] = 0;
    Vector->Count = 0;
    return 0;
}

int VecSort(CC_VECTOR* Vector)
{
    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count <= 1)
    {
        return 0;
    }
    else
    {
        qsort(Vector->Array, Vector->Count, sizeof(int), CompareFunction);
    }

    return 0;
}

int VecAppend(CC_VECTOR* DestVector, CC_VECTOR* SrcVector)
{
    // DIN SRC IN DEST
    if (NULL == DestVector || NULL == SrcVector)
    {
        return -1;
    }
    if (SrcVector->Count != 0)
    {
        for (int i = 0; i < SrcVector->Count; i++)
        {
            VecInsertTail(DestVector, SrcVector->Array[i]);
        }
    }

    return 0;
}


