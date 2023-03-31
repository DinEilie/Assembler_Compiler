#include "header.h"

struct node_line
{
    char *code;
    struct node_line *next;
};

/* Create a new my_line structure with the given parameters and return a pointer to it */
my_line *newLine(char *str)
{
    my_line *myNewLine;
    myNewLine = (my_line *)malloc(sizeof(my_line));
    if (myNewLine == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    if (str != NULL)
        myNewLine->code = duplicateString(str);
    else
        myNewLine->code = NULL;
    myNewLine->next = NULL;
    return myNewLine;
}

/* Return a pointer to a new copy of the line property */
char *getLineCode(my_line *line)
{
    if (line->code != NULL)
        return duplicateString(line->code);
    return NULL;
}

/* Return a pointer to the next my_line node */
my_line *getLineNext(my_line *line)
{
    return line->next;
}

/* Set the code property to the given string */
void setLineCode(my_line *line, char *str)
{
    if (line->code != NULL)
        free(line->code);

    if (str != NULL)
        line->code = duplicateString(str);
    else
        line->code = NULL;
}

/* Set the next property to the given node */
void setLineNext(my_line *line, my_line *nextLine)
{
    line->next = nextLine;
}

/* Add a new line to the final output */
void addNewNonMacroLine(my_line **head, char *str)
{
    my_line *newFileLine;
    my_line *lastFileLine;

    /* Create a new node */
    newFileLine = newLine(str);

    /* If head is empty, it is an empty list */
    if (*head == NULL)
        *head = newFileLine;
    else
    {
        lastFileLine = *head;
        while (getLineNext(lastFileLine) != NULL)
            lastFileLine = getLineNext(lastFileLine);
        setLineNext(lastFileLine, newFileLine);
    }
}

/* Remove from list and memory all lines objects and their properties */
void freeFileLines(my_line **head)
{
    my_line *p1;
    my_line *p2;
    p1 = *head;
    p2 = p1;
    if (p1 == NULL)
    {
        /* End */
    }
    else
    {
        while (getLineNext(p1) != NULL)
        {
            p1 = getLineNext(p1);
            if (p2->code != NULL)
                free(p2->code);
            p2->code = NULL;
            p2->next = NULL;
            free(p2);
            p2 = p1;
        }
        if (p2->code != NULL)
            free(p2->code);
        p2->code = NULL;
        p2->next = NULL;
        free(p2);
        p1 = NULL;
        p2 = NULL;
    }
    *head = NULL;
}

/* Print to the given file the lines list */
void printLinesToFile(my_line *head, FILE *fp)
{
    my_line *current;
    current = head;
    while (current != NULL)
    {
        fprintf(fp, "%s", getLineCode(current));
        current = getLineNext(current);
    }
}