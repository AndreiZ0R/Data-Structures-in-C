#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cchashtable.h"
#include "common.h"

#define INITIAL 31
#define RANGE 20000
static int gPrimeNums[RANGE];

static void Ciur()
{
    gPrimeNums[0] = 1;
    gPrimeNums[1] = 1;
    for (int i = 2; i < sqrt(RANGE); i++)
        if (gPrimeNums[i] == 0)
            for (int j = 2; j < RANGE / i; j++)
                gPrimeNums[i * j] = 1;
}

static int HashFunction(const char* String)
{
    int p = 1;
    int sum = 0;
    for (int i = 0; i < strlen(String); i++)
    {
        sum += (String[i] - '0') * p;
        p *= 2;
    }
    return sum;
}

static int ClosestPrime(int Number)
{
    int i = Number + 1;
    while (gPrimeNums[i] != 0)
        i++;
    return i;
}

static NODE* GiveNodeWithKey(int Value, char* Key)
{
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if (NULL == newNode)
    {
        printf("Allocation failed\n");
        free(newNode);
        return NULL;
    }
    newNode->value = Value;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->state = notVisited;

    newNode->key = (char*)malloc(strlen(Key) * sizeof(char));
    if (NULL == newNode->key)
    {
        printf("Allocation failed\n");
        free(newNode->key);
        return NULL;
    }
    strcpy(newNode->key, Key);
    
    return newNode;
}

static int InsertAtTail(NODE** Head, int NewValue, char* Key)
{
    NODE* new_node = (NODE*)malloc(sizeof(NODE));
    new_node->key = (char*)malloc(strlen(Key) * sizeof(char));

    NODE* last = *Head;
    new_node->value = NewValue;
    strcpy(new_node->key, Key);
    new_node->next = NULL;

    if (NULL == *Head)
    {
        new_node->prev = NULL;
        *Head = new_node;
        return 1;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
    new_node->prev = last;

    return 0;
}

static void DeleteNode(NODE** Head, NODE* Delete)
{
    if (*Head == NULL || Delete == NULL)
        goto cleanup;

    if (*Head == Delete) // case head node
        *Head = Delete->next;

    if (Delete->next != NULL) //if not tail node
        Delete->next->prev = Delete->prev;

    if (Delete->prev != NULL) //if not head node
        Delete->prev->next = Delete->next;

cleanup:
    {
        free(Delete);
    };
}

static NODE* SearchKey(CC_HASH_TABLE* Table, char* Key, int* ReturnedIndex)
{
    for (int i = 0; i < Table->capacity; i++)
    {
        NODE* itr = Table->array[i];
        if (itr != NULL)
        {
            while (itr != NULL)
            {
                if (strcmp(itr->key, Key) == 0)
                {
                    *ReturnedIndex = i;
                    return itr;
                }
                itr = itr->next;
            }
        }
    }
    *ReturnedIndex = -1;
    return NULL;
}

static char* AllocateKey(const char* Key)
{
    char* newString = (char*)malloc(strlen(Key) * sizeof(char));
    if (NULL == newString)
    {
        printf("Allocation failed\n");
        free(newString);
        return NULL;
    }
    return newString;
}

static void ResetCheckedNodes(CC_HASH_TABLE* Table)
{
    for (int i = 0; i < Table->capacity; i++)
    {
        NODE* itr = Table->array[i];
        if (itr != NULL)
        {
            while (itr != NULL)
            {
                itr->state = notVisited;
                itr = itr->next;
            }
        }
    }
}
//end of help functions

int HtCreate(CC_HASH_TABLE** HashTable)
{
    if (NULL != *HashTable)
    {
        printf("Already created\n");
        return -1;
    }

    CC_HASH_TABLE* Tab = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));
    if (NULL == Tab)
    {
        printf("Allocation failed\n");
        free(Tab);
        return -1;
    }
    Tab->capacity = INITIAL;
    Tab->keyCount = 0;
    Tab->fillFactor = 0.0f;
    Tab->size = 0;
    Tab->array = (NODE**)malloc(Tab->capacity * sizeof(NODE*));
    if (NULL == Tab->array)
    {
        printf("Allocation failed\n");
        return -1;
    }

    for (int i = 0; i < Tab->capacity; i++)
    {
        Tab->array[i] = (NODE*)malloc(sizeof(NODE));
        Tab->array[i] = NULL;
    }
    Ciur();
    *HashTable = Tab;
    return 0;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }

    CC_HASH_TABLE* tab = *HashTable;
    for (int i = 0; i < tab->capacity; i++)
        free(tab->array[i]);
    free(tab->array);

    tab->keyCount = -1;
    tab->fillFactor = -1;
    tab->capacity = -1;
    tab->size = -1;

    *HashTable = NULL;

    return 0;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int position = 0;
    NODE* finder = SearchKey(HashTable, Key, &position);
    if (finder != NULL)
    {
        return -1;
    }

    HashTable->fillFactor = (100 * (float)HashTable->size) / (float)HashTable->capacity;
    int oldCapacity = HashTable->capacity;

    if (HashTable->fillFactor >= 75)
    {
        //realocarea cu cel mai apropiat numar prim mai mare decat 2*capacitatea actuala
        HashTable->capacity = ClosestPrime(2 * HashTable->capacity);
        NODE** re = realloc(HashTable->array, HashTable->capacity * sizeof(NODE*));
        if (NULL == re)
        {
            printf("Reallocation failed\n");
            HashTable->capacity = oldCapacity;
            return -1;
        }

        HashTable->array = re;
        for (int i = oldCapacity; i < HashTable->capacity; i++)
        {
            //initializarea noilor noduri
            HashTable->array[i] = (NODE*)malloc(sizeof(NODE));
            HashTable->array[i] = NULL;
        }
    }
    

    int index = HashFunction(Key) % HashTable->capacity;
    NODE* new = GiveNodeWithKey(Value, Key);
    NODE* itr = HashTable->array[index];
    if (HashTable->array[index] == NULL)
    {
        HashTable->array[index] = new;
    }
    else
    {
        InsertAtTail(&itr, Value, Key);
    }

    HashTable->keyCount++;
    HashTable->size++;
    HashTable->fillFactor = (100 * (float)HashTable->size) / (float)HashTable->capacity;

    return 0;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int* Value)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int pos = 0;
    NODE* finder = SearchKey(HashTable, Key, &pos);
    if (finder == NULL)
    {
        *Value = -1;
        return -1;
    }

    *Value = finder->value;

    return 0;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int posKey = 0;
    NODE* finder = SearchKey(HashTable, Key, &posKey);
    if (NULL == finder)
    {
        printf("Key %s doesnt exist!\n", Key);
        return -1;
    }

    DeleteNode(&HashTable->array[posKey], finder);
    HashTable->keyCount--;
    HashTable->size--;

    return 0;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int pos;
    NODE* finder = SearchKey(HashTable, Key, &pos);
    return (NULL == finder ? 0 : 1);

}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR** Iterator, char** Key)
{
    CC_HASH_TABLE_ITERATOR* iterator = NULL;

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
    if (NULL == iterator)
    {
        printf("Allocation failed\n");
        free(iterator);
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    iterator->HashTable = HashTable;
    iterator->key = NULL;
    iterator->value = -1;
    iterator->index = -1;

    *Iterator = iterator;

    ResetCheckedNodes(HashTable); //se seteaza un nou inceput de iteratie
    int ok = -2;
    for (int i = 0; i < HashTable->capacity; i++)
    {
        NODE* itr = HashTable->array[i];
        if (itr != NULL)
        {
            ok = 1;
            iterator->value = itr->value;
            iterator->index = i;
            iterator->key = AllocateKey(itr->key);
            strcpy(iterator->key, itr->key);
            itr->state = visited;

            break;
        }
    }

    if (ok == 1)
    {
        strcpy(*Key, iterator->key);
        return 1;
    }

    iterator->value = -1;
    iterator->index = -1;
    *Key = NULL;
    return -2;
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR* Iterator, char** Key)
{
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }
    if (NULL == Iterator->HashTable)
    {
        return -1;
    }

    int ok = 0;
    for (int i = Iterator->index; i < Iterator->HashTable->capacity; i++)
    {
        NODE* itr = Iterator->HashTable->array[i];
        if (itr != NULL)
        {
            while (itr != NULL && itr->state == visited)
            {
                itr = itr->next;
            }

            if (itr != NULL)
            {
                ok = 1;
                Iterator->value = itr->value;
                Iterator->index = i;
                Iterator->key = AllocateKey(itr->key);
                strcpy(Iterator->key, itr->key);
                itr->state = visited;

                break;
            }
        }
    }

    if (ok == 1)
    {
        strcpy(*Key, Iterator->key);
        return 1;
    }

    Iterator->value = -1;
    Iterator->index = -1;
    *Key = NULL;
    return -2;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR** Iterator)
{
    if (NULL == Iterator)
    {
        return -1;
    }

    CC_HASH_TABLE_ITERATOR* iterator = *Iterator;
    iterator->index = -1;
    iterator->value = -1;
    iterator->HashTable = NULL;
    iterator->key = NULL;
    free(iterator);

    *Iterator = NULL;

    return 0;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }

    // ITERATOR METHOD
//    CC_HASH_TABLE_ITERATOR *iterator = NULL;
//    char *firstKey = AllocateKey("00000000000");
//    HtGetFirstKey(HashTable, &iterator, &firstKey);
//    char *itrString = AllocateKey(firstKey);
//
//    int limit = HashTable->keyCount;
//    for (int i = 0; i < limit; i++)
//    {
//        HtGetNextKey(iterator, &itrString);
//        HtRemoveKey(HashTable, itrString);
//    }
//    HtRemoveKey(HashTable, firstKey);
//
//    HtReleaseIterator(&iterator);


    for (int i = 0; i < HashTable->capacity; i++)
    {
        NODE* itr = HashTable->array[i];
        if (NULL != itr)
        {
            while (NULL != itr)
            {
                NODE* last = itr;
                itr = itr->next;
                DeleteNode(&HashTable->array[i], last);
            }
        }
    }

    HashTable->size = 0;
    HashTable->fillFactor = 0.0f;
    HashTable->keyCount = 0;

    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
    if (NULL == HashTable)
    {
        return -1;
    }

    return HashTable->keyCount;
}
