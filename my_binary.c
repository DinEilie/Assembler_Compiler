#include "header.h"

struct node_binary
{
    int index;
    char *code;
    char *label;
    struct node_binary *next;
};

/* Create a new my_binary structure with the given parameters and return a pointer to it */
my_binary *newBinary(int index, char *code, char *label)
{
    my_binary *myNewBinary = (my_binary *)malloc(sizeof(my_binary));
    if (myNewBinary == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    if (code != NULL)
        myNewBinary->code = duplicateString(code);
    else
        myNewBinary->code = NULL;

    if (label != NULL)
        myNewBinary->label = duplicateString(label);
    else
        myNewBinary->label = NULL;
    myNewBinary->index = index;
    myNewBinary->next = NULL;
    return myNewBinary;
}

/* Return the index property */
int getBinaryIndex(my_binary *binary)
{
    return binary->index;
}

/* Return a duplicate string of the code property */
char *getBinaryCode(my_binary *binary)
{
    if (binary->code != NULL)
        return duplicateString(binary->code);
    return NULL;
}

/* Return a duplicate string of the label property */
char *getBinaryLabel(my_binary *binary)
{
    if (binary->label != NULL)
        return duplicateString(binary->label);
    return NULL;
}

/* Return a pointer to the next my_binary node */
my_binary *getBinaryNext(my_binary *binary)
{
    return binary->next;
}

/* Set the index property */
void setBinaryIndex(my_binary *binary, int newIndex)
{
    binary->index = newIndex;
}

/* Set the code property */
void setBinaryCode(my_binary *binary, char *newCode)
{
    /* Free previous code */
    if (binary->code != NULL)
        free(binary->code);

    if (newCode != NULL)
        binary->code = duplicateString(newCode);
    else
        binary->code = NULL;
}

/* Set the label property */
void setBinaryLabel(my_binary *binary, char *newLabelName)
{
    /* Free previous label */
    if (binary->label != NULL)
        free(binary->label);

    if (newLabelName != NULL)
        binary->label = duplicateString(newLabelName);
    else
        binary->label = NULL;
}

/* Set the next my_binary node */
void setBinaryNext(my_binary *binary, my_binary *newNext)
{
    binary->next = newNext;
}

void addToBinaryList(my_binary **head, my_binary *newBinaryNode)
{
    my_binary *last;
    last = *head;
    if (last != NULL)
    {

        while (getBinaryNext(last) != NULL)
            last = getBinaryNext(last);
        setBinaryNext(last, newBinaryNode);
    }
    else
        *head = newBinaryNode;
}

/* Free the entire my_binary chain from memory allocation */
void freeBinaryChain(my_binary **head)
{
    my_binary *ptr1;
    my_binary *ptr2;
    ptr1 = *head;
    ptr2 = ptr1;

    if (*head != NULL)
    {
        while (getBinaryNext(ptr2) != NULL)
        {
            /* Move the second pointer to the next node */
            ptr2 = getBinaryNext(ptr2);

            /* Free all values of the first pointer */
            if (ptr1->code != NULL)
                free(ptr1->code);
            if (ptr1->label != NULL)
                free(ptr1->label);
            ptr1->code = NULL;
            ptr1->label = NULL;
            free(ptr1);

            /* Move the first pointer to the second pointer */
            ptr1 = ptr2;
        }

        /* Free all values of the first pointer */
        if (ptr1->code != NULL)
            free(ptr1->code);
        if (ptr1->label != NULL)
            free(ptr1->label);
        ptr1->code = NULL;
        ptr1->label = NULL;
        free(ptr1);
        *head = NULL;
    }
}