// This is where function that can be used by multiple files are defined.
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <proto/alib.h>
#include <proto/utility.h>
#include <proto/dos.h>
#include <proto/exec.h>

#include <exec/types.h>

#include "funcs.h"

long get_file_size(char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    if (fseek(fp, 0, SEEK_END) < 0)
    {
        fclose(fp);
        return -1;
    }

    long size = ftell(fp);
    // release the resources when not required
    fclose(fp);
    return size;
}

int GetListLength(struct List *list)
{
    struct Node *node;
    int count = 0;
    for (node = list->lh_Head; node->ln_Succ; node = node->ln_Succ)
        count++;
    return count;
}

void getParentPath(char *filename, char *result, int resultSize)
{
    BPTR fileLock = Lock(filename, SHARED_LOCK);
    if (fileLock)
    {
        BPTR folderLock = ParentDir(fileLock);
        NameFromLock(folderLock, result, resultSize);

        UnLock(folderLock);
        UnLock(fileLock);
    }
}
void getNameFromPath(char *path, char *result, unsigned int resultSize)
{
    BPTR pathLock = Lock(path, SHARED_LOCK);
    if (pathLock)
    {
        struct FileInfoBlock *FIblock = (struct FileInfoBlock *)AllocVec(sizeof(struct FileInfoBlock), MEMF_CLEAR);

        if (Examine(pathLock, FIblock))
        {
            strncpy(result, FIblock->fib_FileName, resultSize);
            FreeVec(FIblock);
        }
        UnLock(pathLock);
    }
}

STRPTR longToString(long num)
{
    STRPTR buffer = AllocVec(64, MEMF_ANY);
    SNPrintf(buffer, 64, "%ld", num);
    return buffer;
}

STRPTR intToString(int num)
{
    STRPTR buffer = AllocVec(64, MEMF_ANY);
    SNPrintf(buffer, 64, "%d", num);
    return buffer;
}

int presentageFromInts(int num1, int num2)
{
    // int presentage = (num1 * 100) / num2;
    int presentage = (  num1 * 200 + num2 ) / ( num2 * 2 );

    // printf("presentageDec: %d\n", result);
    // if (presentage == 0)
    //     return 1;
    return presentage;
}


int stringToInt(char *string)
{
    int result = 0;
    int i = 0;
    while (string[i] != '\0')
    {
        if (isdigit(string[i]))
        {
            result *= 10;
            result += string[i] - '0';
        }
        i++;
    }
    return result;
}

int longToInt(long num)
{
    int result = 0;
    int i = 0;
    char *buffer = AllocVec(64, MEMF_ANY);
    SNPrintf(buffer, 64, "%ld", num);
    while (buffer[i] != '\0')
    {
        if (isdigit(buffer[i]))
        {
            result *= 10;
            result += buffer[i] - '0';
        }
        i++;
    }
    FreeVec(buffer);
    return result;
}

BOOL clearList(struct List list){
    struct Node *node = list.lh_Head;
    while (node->ln_Succ)
    {
        struct Node *nextNode = node->ln_Succ;
        Remove(node);
        FreeVec(node);
        node = nextNode;
    }
    return TRUE;
}

BOOL clearPointerList(struct List *list){
    struct Node *node = list->lh_Head;
    while (node->ln_Succ)
    {
        struct Node *nextNode = node->ln_Succ;
        Remove(node);
        FreeVec(node);
        node = nextNode;
    }
    return TRUE;
}