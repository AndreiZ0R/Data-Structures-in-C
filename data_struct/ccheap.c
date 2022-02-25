#include "ccheap.h"
#include "common.h"
#include "stdio.h"


static void HeapifyDown(CC_HEAP* Heap, int Target)
{
    int rightChild = (Target * 2) + 1;
    int leftChild = (Target * 2) + 2;
    int okayChild;

    if (Heap->caracteristic == Min)
    {
        if (Heap->Array[rightChild] < Heap->Array[leftChild])
            okayChild = rightChild;
        else
            okayChild = leftChild;

        if (okayChild < Heap->size)
            if (Heap->Array[Target] > Heap->Array[okayChild])
            {
                int aux = Heap->Array[Target];
                Heap->Array[Target] = Heap->Array[okayChild];
                Heap->Array[okayChild] = aux;

                HeapifyDown(Heap, okayChild);
            }
    }
    else if (Heap->caracteristic == Max)
    {
        if (Heap->Array[rightChild] > Heap->Array[leftChild])
            okayChild = rightChild;
        else
            okayChild = leftChild;

        if (okayChild < Heap->size)
            if (Heap->Array[Target] < Heap->Array[okayChild])
            {
                int aux = Heap->Array[Target];
                Heap->Array[Target] = Heap->Array[okayChild];
                Heap->Array[okayChild] = aux;

                HeapifyDown(Heap, okayChild);
            }
    }
}

static void HeapifyUp(CC_HEAP* Heap, int Target)
{
    int par = (Target - 1) / 2;

    if (Heap->caracteristic == Min)
    {
        if (Heap->Array[Target] < Heap->Array[par])
        {
            int aux = Heap->Array[Target];
            Heap->Array[Target] = Heap->Array[par];
            Heap->Array[par] = aux;

            HeapifyUp(Heap, par);
        }
    }
    else
    {
        if (Heap->Array[Target] > Heap->Array[par])
        {
            int aux = Heap->Array[Target];
            Heap->Array[Target] = Heap->Array[par];
            Heap->Array[par] = aux;

            HeapifyUp(Heap, par);
        }
    }
}

static int CompareFunction(const void* NumA, const void* NumB)
{
    int aa = *(int*)NumA;
    int bb = *(int*)NumB;
    return aa - bb;
}
//end of help functions


int HpCreateMaxHeap(CC_HEAP** MaxHeap, CC_VECTOR* InitialElements)
{
    if (NULL != *MaxHeap && (*MaxHeap)->caracteristic == Max)
    {
        printf("Already created!\n");
        return -1;
    }

    if (NULL == MaxHeap)
    {
        printf("MinHeap creation fail\n");
        return -1;
    }
    if (NULL == InitialElements)
    {
        InitialElements = NULL;
    }
    CC_HEAP* heap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    heap->Array = (int*)malloc(INIT_SIZE * sizeof(int));
    if (NULL == heap || NULL == heap->Array)
    {
        free(heap->Array);
        free(heap);
        return -1;
    }


    heap->size = 0;
    heap->capacity = INIT_SIZE;
    heap->caracteristic = Max;

    //elements from vector to heap->array
    if (NULL != InitialElements)
    {
        //VecSort(InitialElements);
        for (int i = 0; i < InitialElements->Count; i++)
        {
            int val = InitialElements->Array[i];
            HpInsert(heap, val);
        }
    }

    *MaxHeap = heap;

    return 0;
}

int HpCreateMinHeap(CC_HEAP** MinHeap, CC_VECTOR* InitialElements)
{
    if (NULL != *MinHeap && (*MinHeap)->caracteristic == Min)
    {
        printf("Already created!\n");
        return -1;
    }

    if (NULL == MinHeap)
    {
        printf("MinHeap creation fail\n");
        return -1;
    }
    if (NULL == InitialElements)
    {
        InitialElements = NULL;
    }

    CC_HEAP* heap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    heap->Array = (int*)malloc(INIT_SIZE * sizeof(int));
    if (NULL == heap || NULL == heap->Array)
    {
        free(heap->Array);
        free(heap);
        return -1;
    }

    heap->size = 0;
    heap->capacity = INIT_SIZE;
    heap->caracteristic = Min;

    //elemnts from vector to heap->array
    if (NULL != InitialElements)
    {
        for (int i = 0; i < InitialElements->Count; i++)
        {
            int val = InitialElements->Array[i];
            HpInsert(heap, val);
        }
    }

    *MinHeap = heap;

    return 0;
}

int HpDestroy(CC_HEAP** Heap)
{
    if (NULL == Heap)
    {
        return -1;
    }
    CC_HEAP* hp = *Heap;
    free(hp->Array);
    free(hp);
    *Heap = NULL;
    return 0;
}

int HpInsert(CC_HEAP* Heap, int Value)
{
    if (NULL == Heap)
    {
        return -1;
    }

    if (Heap->size >= Heap->capacity)
    {
        int* newSize = (int*)realloc(Heap->Array, (Heap->capacity + 50) * sizeof(int));
        if (NULL == newSize)
        {
            printf("Realloc failed\n");
            free(newSize);
            return -1;
        }
        else
        {
            Heap->capacity += 50;
            Heap->Array = newSize;
        }
    }

    Heap->Array[Heap->size] = Value;
    Heap->size++;
    HeapifyUp(Heap, Heap->size - 1);

    return 0;
}

int HpRemove(CC_HEAP* Heap, int Value)
{
    if (NULL == Heap)
    {
        return -1;
    }

    int index = -1;
    int left = 0;
    int right = Heap->size - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (Heap->Array[mid] == Value)
        {
            index = mid;
            break;
        }
        else if (Heap->Array[mid] > Value)
        {
            left = mid + 1;
        }
        else
            right = mid - 1;
    }

    if (index == -1)
    {
        printf("Value not found\n");
        return -1;
    }
    else
    {
        int aux = Heap->Array[index];
        Heap->Array[index] = Heap->Array[Heap->size - 1];
        Heap->Array[Heap->size - 1] = aux;

        Heap->size--;
        HeapifyUp(Heap, Heap->size - 1);
    }


    return 0;
}

int HpGetExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
    if (NULL == Heap)
    {
        return -1;
    }
    *ExtremeValue = Heap->Array[0];

    return 0;
}

int HpPopExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
    if (NULL == Heap)
    {
        return -1;
    }
    *ExtremeValue = Heap->Array[0];

    int aux = Heap->Array[0];
    Heap->Array[0] = Heap->Array[Heap->size - 1];
    Heap->Array[Heap->size - 1] = aux;

    Heap->size--;
    HeapifyDown(Heap, 0);


    return 0;
}

int HpGetElementCount(CC_HEAP* Heap)
{
    if (NULL == Heap)
    {
        return -1;
    }

    return Heap->size;
}

int HpSortToVector(CC_HEAP* Heap, CC_VECTOR* SortedVector)
{
    if (NULL == Heap)
    {
        return -1;
    }
    if (NULL == SortedVector)
    {
        printf("Vector uninitialised\n");
        return -1;
    }
    VecClear(SortedVector);
    for (int i = 0; i < Heap->size; i++)
        VecInsertTail(SortedVector, Heap->Array[i]);

    qsort(SortedVector->Array, Heap->size, sizeof(int), CompareFunction);

    return 0;
}

