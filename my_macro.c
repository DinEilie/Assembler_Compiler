#include "header.h"

struct node_macro
{
    char *name;
    my_line *lines;
    struct node_macro *next;
};

/* Create a new my_macro structure with the given parameters and return a pointer to it */
my_macro *newMacro(char *name, my_line *lines)
{
    my_macro *myNewMacro;
    myNewMacro = (my_macro *)malloc(sizeof(my_macro));
    if (myNewMacro == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    if (name != NULL)
        myNewMacro->name = duplicateString(name);
    else
        myNewMacro->name = NULL;
    if (lines != NULL)
        myNewMacro->lines = lines;
    else
        myNewMacro->lines = NULL;
    myNewMacro->next = NULL;
    return myNewMacro;
}

/* Return pointer to a copy of the given macro name */
char *getMacroName(my_macro *macro)
{
    if (macro->name != NULL)
        return duplicateString(macro->name);
    return NULL;
}

/* Return a pointer to the head pointer of the macro lines list */
my_line *getMacroLines(my_macro *macro)
{
    return macro->lines;
}

/* Return a pointer to the next macro node */
my_macro *getMacroNext(my_macro *macro)
{
    return macro->next;
}

/* Set the macro lines to the given lines list */
void setMacroLines(my_macro *macro, my_line *line)
{
    macro->lines = line;
}

/* Set the next macro node */
void setMacroNext(my_macro *macro, my_macro *nextMacro)
{
    macro->next = nextMacro;
}

/* Searches for a macro with a specific name and return a pointer to it */
my_macro *searchMacro(my_macro *head, char *otherName)
{
    my_macro *current;
    char *current_name;
    current_name = NULL;
    current = head;
    while (current != NULL && otherName != NULL)
    {
        current_name = getMacroName(current);
        if (strcmp(current_name, otherName) == 0)
        {
            free(current_name);
            return current;
        }
        free(current_name);
        current = getMacroNext(current);
    }
    return NULL;
}

/* Add a new line as the last line of a macro */
void addNewLineToMacro(my_macro *macro, char *str)
{
    my_line *new_line;
    my_line *last;

    /* Create a new node */
    new_line = newLine(str);

    if (getMacroLines(macro) == NULL)
        setMacroLines(macro, new_line);
    else
    {
        last = getMacroLines(macro);
        while (getLineNext(last) != NULL)
            last = getLineNext(last);
        setLineNext(last, new_line);
    }
}

/* Add a new copy of the given macro to the end of macros list */
void addNewMacroToList(my_macro **list, my_macro *macro)
{
    my_macro *last;
    last = *list;

    if (last != NULL)
    {
        while (getMacroNext(last) != NULL)
            last = getMacroNext(last);
        setMacroNext(last, macro);
    }
    else
        *list = macro;
}

/* Remove from list and memory all macros objects and their properties */
void freeMacros(my_macro **head)
{
    my_macro *p1;
    my_macro *p2;
    p1 = *head;
    p2 = p1;
    if (p1 == NULL)
    {
        /* End */
    }
    else
    {
        while (getMacroNext(p1) != NULL)
        {
            p1 = getMacroNext(p1);
            freeFileLines(&p2->lines);
            if (p2->name != NULL)
                free(p2->name);
            p2->name = NULL;
            p2->lines = NULL;
            p2->next = NULL;
            free(p2);
            p2 = p1;
        }
        freeFileLines(&(p1->lines));
        if (p1->name != NULL)
            free(p1->name);
        p1->lines = NULL;
        p1->name = NULL;
        p1->next = NULL;
        free(p1);
        p1 = NULL;
        p2 = NULL;
    }
    *head = NULL;
}