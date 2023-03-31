#include "header.h"

struct node_label
{
    int index;
    int type;
    char *name;
    struct node_label *next;
};

/* Create a new my_label structure with the given parameters and return a pointer to it */
my_label *newLabel(int index, char *name, int type)
{
    my_label *myNewLabel = (my_label *)malloc(sizeof(my_label));
    if (myNewLabel == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    if (name != NULL)
        myNewLabel->name = duplicateString(name);
    else
        myNewLabel->name = NULL;
    myNewLabel->index = index;
    myNewLabel->type = type;
    myNewLabel->next = NULL;
    return myNewLabel;
}

/* Return the index property of the given label */
int getLabelIndex(my_label *label)
{
    return label->index;
}

/* Return a pointer to a copy of the given label's name  */
char *getLabelName(my_label *label)
{
    if (label->name != NULL)
        return duplicateString(label->name);
    return NULL;
}

/* Return the type property of the given label: 0 = data    1 = extern    2 = entry    3 = code */
int getLabelType(my_label *label)
{
    return label->type;
}

/* Return a pointer to the next node property of the given label */
my_label *getLabelNext(my_label *label)
{
    return label->next;
}

/* Set a new given index for the label */
void setLabelIndex(my_label *label, int newIndex)
{
    label->index = newIndex;
}

/* Set a new given name for the label */
void setLabelName(my_label *label, char *newName)
{
    /* Free the previous name */
    if (label->name != NULL)
        free(label->name);

    if (newName == NULL)
        label->name = NULL;
    else
        label->name = duplicateString(newName);
}

/* Set a new given type for the label */
void setLabelType(my_label *label, int newType)
{
    label->type = newType;
}

/* Set a new next node for the label */
void setLabelNext(my_label *label, my_label *newNext)
{
    label->next = newNext;
}

/* Add a new next node to the end of the label chain */
void addToLabelList(my_label **head, my_label *newLabelNode)
{
    my_label *last;
    last = *head;
    if (last != NULL)
    {
        while (getLabelNext(last) != NULL)
            last = getLabelNext(last);
        setLabelNext(last, newLabelNode);
    }
    else
        *head = newLabelNode;
}

/* Return a pointer to label with the give name from a label list */
my_label *searchLabel(my_label *head, char *label)
{
    my_label *current;
    char *current_name;
    current_name = NULL;
    current = head;
    while (current != NULL && label != NULL)
    {
        current_name = getLabelName(current);
        if (current_name != NULL && strcmp(current_name, label) == 0)
        {
            free(current_name);
            return current;
        }
        free(current_name);
        current = getLabelNext(current);
    }
    return NULL;
}

/* Free the entire label list from memory */
void freeLabelChain(my_label **head)
{
    my_label *ptr1;
    my_label *ptr2;
    ptr1 = *head;
    ptr2 = ptr1;

    if (*head != NULL)
    {
        while (getLabelNext(ptr2) != NULL)
        {
            /* Move the second pointer to the next node */
            ptr2 = getLabelNext(ptr2);

            /* Free all values of the first pointer */
            if (ptr1->name != NULL)
                free(ptr1->name);
            ptr1->name = NULL;
            free(ptr1);

            /* Move the first pointer to the second pointer */
            ptr1 = ptr2;
        }

        /* Free all values of the first pointer */
        if (ptr1->name != NULL)
            free(ptr1->name);
        ptr1->name = NULL;
        free(ptr1);
        *head = NULL;
    }
}