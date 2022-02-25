#include "cctree.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

//Balanced BST

static int gPreorderNum = 0;
static int gInorderNum = 0;
static int gPostorderNum = 0;

typedef enum _CALLS
{
    firstCall, anotherCall
} CALL;

static int BinSearch(int* Vector, int Left, int Right, int Val)
{
    if (Left <= Right)
    {
        int mid = (Left + Right) / 2;
        if (Val == Vector[mid])
        {
            return 1;
        }
        else if (Val > Vector[mid])
        {
            return BinSearch(Vector, mid + 1, Right, Val);
        }
        else if (Val < Vector[mid])
        {
            return BinSearch(Vector, Left, mid - 1, Val);
        }
    }

    return 0;
}

static int GetHeight(NOD* Root)
{
    if (NULL == Root) 
    { 
        return -1; 
    }
    if (NULL == Root->left && NULL == Root->right) 
    { 
        return 0; 
    }

    int leftHeight = GetHeight(Root->left);
    int rightHeight = GetHeight(Root->right);


    return (leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1);
}

static void PreOrder(NOD* Root, int* Vect, CALL MyCall)
{
    if (MyCall == firstCall)
        gPreorderNum = 0;

    if (NULL != Root)
    {
        // root left right
        Vect[gPreorderNum++] = Root->value;
        PreOrder(Root->left, Vect, anotherCall);
        PreOrder(Root->right, Vect, anotherCall);
    }
}

static void InOrder(NOD* Root, int* Vect, CALL MyCall)
{
    if (MyCall == firstCall)
        gInorderNum = 0;

    if (NULL != Root)
    {
        // left root right
        InOrder(Root->left, Vect, anotherCall);
        Vect[gInorderNum++] = Root->value;
        InOrder(Root->right, Vect, anotherCall);
    }


}

static void PostOrder(NOD* Root, int* Vect, CALL MyCall)
{
    if (MyCall == firstCall)
        gPostorderNum = 0;

    if (NULL != Root)
    {
        //left right root
        PostOrder(Root->left, Vect, anotherCall);
        PostOrder(Root->right, Vect, anotherCall);
        Vect[gPostorderNum++] = Root->value;
    }
}

static NOD* MinValueNode(NOD* Nod)
{
    NOD* current = Nod;

    //cel mai din stanga fiu
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

static NOD* GetNode(int Val)
{
    NOD* newNode = (NOD*)malloc(sizeof(NOD));
    if (NULL == newNode)
    {
        printf("Node allocation failed\n");
        free(newNode);
        return NULL;
    }
    newNode->value = Val;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

static NOD* InsertNode(NOD* Root, int Val)
{
    if (Root == NULL)
    {
        Root = GetNode(Val);
    }
    else if (Val <= Root->value)
    {
        Root->left = InsertNode(Root->left, Val);
    }
    else
    {
        Root->right = InsertNode(Root->right, Val);
    }
    return Root;
}

static NOD* RemoveNode(NOD* Root, int Val)
{
    
    if (NULL == Root)
    {
        return Root;
    }

    if (Val < Root->value) // valoare mai mica, merg in stanga
    {
        Root->left = RemoveNode(Root->left, Val);
    }

    else if (Val > Root->value) //valoare mai mare, merg in dreapta
    {
        Root->right = RemoveNode(Root->right, Val);
    }
    else //am gasit valoarea
    {
        if (Root->left == NULL && Root->right == NULL)
        {
            return NULL; //daca nu are urmasi, se sterge direct
        }
        else if (Root->left == NULL)
        {
            NOD* temp = Root->right; //are un singur urmas, nodul drept
            free(Root);
            return temp;
        }
        else if (Root->right == NULL)
        {
            NOD* temp = Root->left; //are un singur urmas, nodul stang
            free(Root);
            return temp;
        }

        NOD* temp = MinValueNode(Root->right); // daca sunt doi urmasi, il voi lua pe cel mai mic in ordine
        Root->value = temp->value;
        Root->right = RemoveNode(Root->right, temp->value);
    }
    return Root;

}

static void DeleteTree(CC_TREE* Tree, NOD* Root)
{
    if (NULL != Root)
    {
        DeleteTree(Tree, Root->left);
        DeleteTree(Tree, Root->right);
        TreeRemove(Tree, Root->value);
    }

}

static void FreeLeft(NOD* Root)
{
    if (Root != NULL)
    {
        FreeLeft(Root->left);
        //printf("Freeing %d...\n", root->value);
        free(Root);
    }
}

static void FreeRight(NOD* Root)
{
    if (Root != NULL)
    {
        FreeRight(Root->right);
        //printf("Freeing %d...\n", root->value);
        free(Root);
    }
}

static NOD* Builder(int* Nodes, int Left, int Right)
{
    if (Left > Right)
        return NULL;

    int mid = (Left + Right) / 2;
    NOD* root = (NOD*)malloc(sizeof(NOD));
    root->value = Nodes[mid];

    root->left = Builder(Nodes, Left, mid - 1);
    root->right = Builder(Nodes, mid + 1, Right);

    return root;
}

static NOD* BuildBalancedTree(CC_TREE* Tree, NOD* Root)
{
    int* nodes = (int*)malloc(Tree->count * sizeof(int));
    InOrder(Root, nodes, firstCall);

    return Builder(nodes, 0, Tree->count - 1);
}
//end of help functions


int TreeCreate(CC_TREE** Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }

    CC_TREE* aux = (CC_TREE*)malloc(sizeof(CC_TREE));
    if (NULL == aux)
    {
        printf("Malloc failed\n");
        free(aux);
        return -1;
    }

    aux->root = NULL;
    aux->count = 0;
    aux->height = -1;

    *Tree = aux;
    return 0;
}

int TreeDestroy(CC_TREE** Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }

    CC_TREE* aux = *Tree;
    if (NULL == aux->root)
    {
        free(aux->root);
        free(aux);
        Tree = NULL;
        return 0;
    }

    aux->height = -1;
    aux->count = 0;

    FreeLeft(aux->root->left);
    FreeRight(aux->root->right);

    free(aux->root);
    free(aux);

    Tree = NULL;

    return 0;
}

int TreeInsert(CC_TREE* Tree, int Value)
{
    if (NULL == Tree)
    {
        printf("Tree not created\n");
        return -1;
    }
    else
    {
        Tree->root = InsertNode(Tree->root, Value);
        Tree->count++;
    }

    int leftHeight = GetHeight(Tree->root->left);
    int rightHeight = GetHeight(Tree->root->right);

    if (abs(leftHeight - rightHeight) >= 1)
        Tree->root = BuildBalancedTree(Tree, Tree->root);

    return 0;
}

int TreeRemove(CC_TREE* Tree, int Value)
{
    if (NULL == Tree || NULL == Tree->root)
    {
        return -1;
    }

    int verify = TreeContains(Tree, Value);
    if (verify == 1)
    {
        int all = 1;
        while (all == 1)
        {
            Tree->root = RemoveNode(Tree->root, Value);
            Tree->count--;
            all = TreeContains(Tree, Value);
        }

    }
    else
    {
        return -1;
    }

    return 0;
}

int TreeContains(CC_TREE* Tree, int Value)
{
    if (NULL == Tree)
    {
        return -1;
    }
    if (Tree->count <= 0 || Tree->root == NULL)
        return 0;

    int* v = (int*)malloc(Tree->count * sizeof(int));
    InOrder(Tree->root, v, firstCall);

    int returnedVal = BinSearch(v, 0, Tree->count, Value);
    if (returnedVal == 1)
    {
        return 1;
    }
    return 0;
}

int TreeGetCount(CC_TREE* Tree)
{
    if (NULL == Tree)
    {
        printf("GetCount failed\n");
        return -1;
    }
    else if (NULL == Tree->root)
    {
        return 0;
    }
    else
        return Tree->count;

}

int TreeGetHeight(CC_TREE* Tree)
{
    if (NULL == Tree || NULL == Tree->root)
    {
        return -1;
    }

    int height = GetHeight(Tree->root);
    return height;

}

int TreeClear(CC_TREE* Tree)
{
    if (NULL == Tree)
    {
        return -1;
    }
    DeleteTree(Tree, Tree->root->left);
    DeleteTree(Tree, Tree->root->right);
    Tree->root->left = NULL;
    Tree->root->right = NULL;
    Tree->root = NULL;

    return 0;
}

int TreeGetNthPreorder(CC_TREE* Tree, int Index, int* Value)
{
    if (NULL == Tree || Index > Tree->count || Index <= 0)
    {
        *Value = -1;
        return -1;
    }
    if (Tree->count <= 0)
    {
        printf("Nth Preorder failed\n");
        return -1;
    }

    int* vectorPreorder = (int*)malloc(Tree->count * sizeof(int));
    if (NULL == vectorPreorder)
    {
        free(vectorPreorder);
        return -1;
    }
    PreOrder(Tree->root, vectorPreorder, firstCall);
    *Value = vectorPreorder[Index - 1];

    free(vectorPreorder);
    return 0;
}

int TreeGetNthInorder(CC_TREE* Tree, int Index, int* Value)
{
    if (NULL == Tree || Index > Tree->count || Index <= 0)
    {
        *Value = -1;
        return -1;
    }
    if (Tree->count <= 0)
    {
        printf("Nth Inorder failed\n");
        return -1;
    }

    int* vectorInorder = (int*)malloc(Tree->count * sizeof(int));
    if (NULL == vectorInorder)
    {
        free(vectorInorder);
        return -1;
    }
    InOrder(Tree->root, vectorInorder, firstCall);
    *Value = vectorInorder[Index - 1];

    free(vectorInorder);
    return 0;
}

int TreeGetNthPostorder(CC_TREE* Tree, int Index, int* Value)
{
    if (NULL == Tree || Index > Tree->count || Index <= 0)
    {
        *Value = -1;
        return -1;
    }
    if (Tree->count <= 0)
    {
        printf("Nth postorder failed\n");
        return -1;
    }

    int* vectorPostorder = (int*)malloc(Tree->count * sizeof(int));
    if (NULL == vectorPostorder)
    {
        free(vectorPostorder);
        return -1;
    }
    PostOrder(Tree->root, vectorPostorder, firstCall);
    *Value = vectorPostorder[Index - 1];

    free(vectorPostorder);
    return 0;
}



