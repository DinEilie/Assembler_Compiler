/*
 * Title: Maman 14 - Assembler Project
 * Description: utilites.c is holding most of the functions that are neccessary for the program at all stages.
 * Author: Din Eilie
 * Student ID: 208679597
 */
#include "header.h"

/* Return 1 if the given char is considered whitespace */
int isWhitespace(char ch)
{
    if (ch == ' ' || ch == '\t')
        return 1;
    return 0;
}

/* Return a pointer to the first consecutive word from the given string */
char *firstString(char *p)
{
    char *pos;
    char *str;
    int j = 0;
    str = (char *)malloc(1);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *str = '\0';
    pos = p;
    /* Skip whitespaces */
    while (isWhitespace(*pos))
    {
        pos += 1;
    }
    if (*pos != '\n' && *pos != '\0' && *pos != '\r')
    {
        /* Count every char that does not resemble the end of the word, or the line */
        while (isWhitespace(*(pos + j)) != 1 && *(pos + j) != '\n' && *(pos + j) != '\0' && *(pos + j) != '\r')
            j++;
        free(str);
        str = (char *)malloc(j + 1);
        if (str == NULL)
        {
            /* Error: memory allocation has failed*/
            printError(-1, 0);
            exit(0);
        }
        memcpy(str, pos, j);
        *(str + j) = '\0';
    }
    return str;
}

/* Return a pointer to the first consecutive word from the given string (may stop at first occurence of ',') */
char *firstOperand(char *p)
{
    char *pos;
    char *str;
    int j = 0;
    str = (char *)malloc(1);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *str = '\0';
    pos = p;
    /* Skip whitespaces */
    while (isWhitespace(*pos))
    {
        pos += 1;
    }
    if (*pos != '\n' && *pos != '\0' && *pos != ',' && *pos != '\r')
    {
        /* Count every char that does not resemble the end of the word, or the line */
        while (isWhitespace(*(pos + j)) != 1 && *(pos + j) != '\n' && *(pos + j) != '\0' && *(pos + j) != ',' && *(pos + j) != '\r')
            j++;
        free(str);
        str = (char *)malloc(j + 1);
        if (str == NULL)
        {
            /* Error: memory allocation has failed*/
            printError(-1, 0);
            exit(0);
        }
        memcpy(str, pos, j);
        *(str + j) = '\0';
    }
    return str;
}

/* Return a pointer to the first consecutive word from the given string (may stop at first occurence of ')') */
char *lastParameter(char *p)
{
    char *pos;
    char *str;
    int j = 0;
    str = (char *)malloc(1);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *str = '\0';
    pos = p;
    while (isWhitespace(*pos))
    {
        pos += 1;
    }
    if (*pos != '\n' && *pos != '\0' && *pos != ')' && *pos != '\r')
    {
        /* Count every char that does not resemble the end of the word, or the line */
        while (isWhitespace(*(pos + j)) != 1 && *(pos + j) != '\n' && *(pos + j) != '\0' && *(pos + j) != ')' && *(pos + j) != '\r')
            j++;
        free(str);
        str = (char *)malloc(j + 1);
        if (str == NULL)
        {
            /* Error: memory allocation has failed*/
            printError(-1, 0);
            exit(0);
        }
        memcpy(str, pos, j);
        *(str + j) = '\0';
    }
    return str;
}

/* Return a pointer to the first consecutive word from the given string (may stop at first occurence of '('  ) */
char *firstLabel(char *p)
{
    char *str;
    int j = 0;
    while (isWhitespace(*(p + j)) != 1 && *(p + j) != '\n' && *(p + j) != '\0' && *(p + j) != '(' && *(p + j) != '\r')
        j++;
    str = (char *)malloc(j + 1);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    memcpy(str, p, j);
    *(str + j) = '\0';
    return str;
}

/* Return a pointer to the first consecutive word from the given string (may stop at first occurence of ':'  ) */
char *firstLabelDefinition(char *p)
{
    char *str;
    int j = 0;
    while (isWhitespace(*(p + j)) != 1 && *(p + j) != '\n' && *(p + j) != '\0' && *(p + j) != ':' && *(p + j) != '\r')
        j++;
    str = (char *)malloc(j + 1);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    memcpy(str, p, j);
    *(str + j) = '\0';
    return str;
}

/* Return a pointer to a string that describes the binary code for an int between -8192 to 8191 using 14-Bits  */
char *convertNumberToBinary(int num)
{
    char *str;
    int i;
    unsigned int abs;
    abs = num;

    if (num < 0)
        abs = num * -1;

    /* Convert the absolute number to binary with 12Bits */
    str = (char *)malloc(15);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *(str + 14) = '\0';
    for (i = 13; i >= 0; i--)
    {
        if ((abs & 1U) == 1)
            *(str + i) = '1';
        else
            *(str + i) = '0';
        abs = abs >> 1;
    }

    if (num < 0)
    {
        /* After the first appearance of a bit '1' transform every bit its opposite  */
        i = 13;
        while (*(str + i) == '0' && i >= 0)
            i--;
        if (i > 0)
        {
            i--;
            while (i >= 0)
            {
                if (*(str + i) == '0')
                    *(str + i) = '1';
                else
                    *(str + i) = '0';
                i--;
            }
        }
    }

    return str;
}

/* Return a pointer to a string that describes the binary code for an int between -2048 to 2047 using the last 12-Bits (the first 2-Bit always zero)  */
char *convertImmediateToBinary(int num)
{
    char *str;
    int i;
    unsigned int abs;
    abs = num;

    if (num < 0)
        abs = num * -1;

    /* Convert the absolute number to binary with 12Bits */
    str = (char *)malloc(15);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *(str + 14) = '\0';
    *(str + 13) = '0';
    *(str + 12) = '0';
    for (i = 11; i >= 0; i--)
    {
        if ((abs & 1U) == 1)
            *(str + i) = '1';
        else
            *(str + i) = '0';
        abs = abs >> 1;
    }

    if (num < 0)
    {
        /* After the first appearance of a bit '1' transform every bit its opposite  */
        i = 11;
        while (*(str + i) == '0' && i >= 0)
            i--;
        if (i > 0)
        {
            i--;
            while (i >= 0)
            {
                if (*(str + i) == '0')
                    *(str + i) = '1';
                else
                    *(str + i) = '0';
                i--;
            }
        }
    }

    return str;
}

/* Return a pointer to a string that describes the binary code for registers */
char *convertRegisterToBinary(int source, int dest)
{
    char *str;
    int i;
    i = 11;
    str = (char *)malloc(15);
    if (str == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    *(str + 14) = '\0';
    memset(str, '0', 14);
    /* Translate for target operand */
    while (i > 5)
    {
        if ((dest & 1U) == 1)
            *(str + i) = '1';
        else
            *(str + i) = '0';
        i--;
        dest = dest >> 1;
    }
    /* Translate for source operand */
    while (i >= 0)
    {
        if ((source & 1U) == 1)
            *(str + i) = '1';
        else
            *(str + i) = '0';
        i--;
        source = source >> 1;
    }
    return str;
}

/* Return a pointer to a string that describes the binary code for labels */
/* if the label has not been found return a pointer to the string: "??????????????" */
char *convertLabelToBinary(my_label *head, char *label)
{
    my_label *p;
    char *str;
    /* Search for the given label */
    p = searchLabel(head, label);
    if (p != NULL)
    {
        /* The label type is not "data", therefore the index is not going to update after "First Stage" */
        if (getLabelType(p) != 0)
        {
            str = convertImmediateToBinary(getLabelIndex(p));

            /* The label type is "code" */
            if (getLabelType(p) != 1)
                *(str + 12) = '1';
            /* The label type is "extern" */
            else
                *(str + 13) = '1';
        }
        else
        {
            str = (char *)malloc(15);
            *(str + 14) = '\0';
            memset(str, '?', 14);
        }
    }
    else
    {
        str = (char *)malloc(15);
        *(str + 14) = '\0';
        memset(str, '?', 14);
    }
    return str;
}

/* Return a pointer to a string that describes the binary code for labels */
/* if the label has not been found return NULL */
char *convertLabel(my_label *head, char *label)
{
    my_label *p;
    char *str;
    /* Search for the given label */
    p = searchLabel(head, label);
    if (p != NULL)
    {
        str = convertImmediateToBinary(getLabelIndex(p));

        /* The label type is not "extern" */
        if (getLabelType(p) != 1)
            *(str + 12) = '1';
        /* The label type is "extern" */
        else
            *(str + 13) = '1';
        return str;
    }
    return NULL;
}

/* Return a pointer to a new copy of the given string */
char *duplicateString(char *arr)
{
    int length;
    char *arr_cpy;
    if (arr != NULL)
        length = strlen(arr);
    else
        length = 0;
    arr_cpy = (char *)malloc(length + 1);
    if (arr_cpy == NULL)
    {
        /* Error: memory allocation has failed*/
        printError(-1, 0);
        exit(0);
    }
    memcpy(arr_cpy, arr, length);
    *(arr_cpy + length) = '\0';
    return arr_cpy;
}

/* Return a pointer to a new copy of the given file name and change its ending with .ob */
char *generateObName(char *fullName)
{
    char *newName;
    int i;
    i = strlen(fullName);
    newName = duplicateString(fullName);
    *(newName + (i - 2)) = 'o';
    *(newName + (i - 1)) = 'b';
    return newName;
}

/* Return a pointer to a new copy of the given file name and change its ending with .ent */
char *generateEntName(char *fullName)
{
    char *newName;
    int i;
    i = strlen(fullName);
    if (i < 256)
    {
        newName = (char *)malloc(i + 2);
        memcpy(newName, fullName, i);
        *(newName + (i - 2)) = 'e';
        *(newName + (i - 1)) = 'n';
        *(newName + i) = 't';
        *(newName + i + 1) = '\0';
    }
    else
    {
        newName = duplicateString(fullName);
        *(newName + (i - 3)) = 'e';
        *(newName + (i - 2)) = 'n';
        *(newName + (i - 1)) = 't';
    }
    return newName;
}

/* Return a pointer to a new copy of the given file name and change its ending with .ext */
char *generateExtName(char *fullName)
{
    char *newName;
    int i;
    i = strlen(fullName);
    if (i < 256)
    {
        newName = (char *)malloc(i + 2);
        memcpy(newName, fullName, i);
        *(newName + (i - 2)) = 'e';
        *(newName + (i - 1)) = 'x';
        *(newName + i) = 't';
        *(newName + i + 1) = '\0';
    }
    else
    {
        newName = duplicateString(fullName);
        *(newName + (i - 3)) = 'e';
        *(newName + (i - 2)) = 'x';
        *(newName + (i - 1)) = 't';
    }
    return newName;
}

/* Return the immediate number if valid, otherwise return -3000 */
int isValidImmediate(char *str)
{
    int i;
    int result;

    if (strlen(str) == 2)
    {
        if (*str == '#' && isdigit(*(str + 1)))
        {
            result = atoi((str + 1));
        }
        else
        {
            result = -3000;
        }
    }
    else if (strlen(str) > 2)
    {
        if (*str == '#' && (*(str + 1) == '+' || *(str + 1) == '-' || isdigit(*(str + 1))))
        {
            result = atoi((str + 1));
            for (i = 2; i < strlen(str); i++)
            {
                if (isdigit(*(str + i)) == 0)
                {
                    /* Error: an invalid immediate */
                    result = -3000;
                }
            }
            if (result < -2048 || result > 2047)
            {
                /* Error: immediate can only be a number between -2048 to 2047 */
                result = -3000;
            }
        }
        else
        {
            /* Error: an invalid immediate */
            result = -3000;
        }
    }
    else
    {
        /* Error: an invalid immediate */
        result = -3000;
    }
    return result;
}

/* Return the integer number if valid, otherwise return -9000 */
int isValidInt(char *str)
{
    int result;
    int i;
    if (strlen(str) == 1)
    {
        if (isdigit(*str))
            result = atoi(str);
        else
            /* Error: an invalid integer */
            result = -9000;
    }
    else if (strlen(str) >= 2)
    {
        if (*str == '+' || *str == '-' || isdigit(*str))
        {
            result = atoi(str);
            for (i = 1; i < strlen(str); i++)
            {
                if (isdigit(*(str + i)) == 0)
                    /* Error: an invalid integer */
                    result = -9000;
            }
            /* Error: integer with 14-Bits can only be a number between -8192 to 8191 */
            if (result < -8192 || result > 8191)
                result = -9000;
        }
        else
        {
            /* Error: an invalid integer */
            result = -9000;
        }
    }
    else
    {
        /* Error: an invalid integer */
        result = -9000;
    }
    return result;
}

/* Return 1 if the string is a valid (not neccesary exists) label */
int isValidLabel(char *str)
{
    int i;
    int result = 1;
    if (strlen(str) < 1 || strlen(str) > 30)
        result = 0;
    else if (isalpha(*str) == 0)
        result = 0;
    else if (strlen(str) > 1)
    {
        for (i = 1; i < strlen(str); i++)
        {
            if (isalpha(*(str + i)) == 0 && isdigit(*(str + i)) == 0)
                result = 0;
        }
        if (isValidName(str) == 0)
            result = 0;
    }
    return result;
}

/* Return 1 if the given string is not a saved-assembler phrase  */
int isValidName(char str[])
{
    int result = 1;
    char *forbiddenPhrases[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", ".data", ".string", ".entry", ".extern"};
    int i;

    for (i = 0; i < 24; i++)
    {
        if (strcmp(str, *(forbiddenPhrases + i)) == 0)
            result = 0;
    }
    return result;
}

/* Return 1 if the given string is not a register phrase  */
int isValidRegister(char *str)
{
    int i;
    int result = -1;
    char *phrases[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    for (i = 0; i < 8; i++)
    {
        if (strcmp(str, *(phrases + i)) == 0)
            result = i;
    }
    return result;
}

/* Return the index of the next char after a comma (as long as one comma only was detected), else return -1 */
int isValidComma(char *str)
{
    int count = 0;
    int i = 0;
    while (isWhitespace(*(str + i)) || *(str + i) == ',')
    {
        if (*(str + i) == ',')
            count++;
        i++;
    }
    if (count != 1)
        return -1;
    return i;
}

/* Return the index of the ending of a line, as long as one ending bracket only was detected, else return -1 */
int isValidEndingBracket(char *str)
{
    int count = 0;
    int i = 0;
    while (isWhitespace(*(str + i)) || *(str + i) == ')')
    {
        if (*(str + i) == ')')
            count++;
        i++;
    }
    if (*(str + i) != '\n' && *(str + i) != '\0' && *(str + i) != '\r')
        return -1;
    if (count != 1)
        return -1;
    return i;
}

/* Return 1 if the given string contains nothing but whitespaces, line ending or comments */
int isLineEmpty(char *p)
{
    int result = 1;
    int i = 0;
    while (isWhitespace(*(p + i)))
        i++;
    if (*(p + i) != '\n' && *(p + i) != '\0' && *(p + i) != '\r')
        result = 0;
    return result;
}

/* Return the instruction opcode of the given string, otherwise return -1 */
int isInstruction(char str[])
{
    int result = -1;
    char *phrases[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    int i;

    for (i = 0; i < 16; i++)
    {
        if (strcmp(str, *(phrases + i)) == 0)
            result = i;
    }
    return result;
}

/* Return the index of the last occurance of the given char in the given string */
int lastCharPosition(char *str, char ch)
{
    int result;
    int i;
    int length;
    result = -1;
    length = strlen(str);
    for (i = 0; i < length; i++)
    {
        if (*(str + i) == ch)
            result = i;
    }
    return result;
}

/* Update the given string to the unique binary form, '1' -> '/' and '0' -> '.' */
void convertToUniqueBinary(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (*(str + i) == '0')
            *(str + i) = '.';
        else
            *(str + i) = '/';
    }
}

/* Print the given my_label list into the given file name all the label with ".entry" type */
void printEntries(my_label *p, char *file_name)
{
    FILE *ent_file;
    my_label *current;
    char *current_name;
    int isEmpty;
    isEmpty = 1;
    current_name = NULL;
    current = p;

    /* Verify that the list contains labels with ".entry" type */
    while (current != NULL)
    {
        /* The label type is ".entry" */
        if (getLabelType(current) == 2)
            isEmpty = 0;
        current = getLabelNext(current);
    }

    current = p;

    if (isEmpty == 0)
    {
        ent_file = fopen(file_name, "w+");
        while (current != NULL)
        {
            current_name = getLabelName(current);
            /* The label type is ".entry" */
            if (getLabelType(current) == 2)
                fprintf(ent_file, "%s\t\t%d\n", current_name, getLabelIndex(current));
            if (current_name != NULL)
                free(current_name);
            current = getLabelNext(current);
        }
        fclose(ent_file);
    }
}

/* Print the given my_label list into the given file name all the label with ".external" type */
void printExternals(my_label *p, char *file_name)
{
    FILE *ext_file;
    my_label *current;
    char *current_name;
    current_name = NULL;
    current = p;

    /* Verify that the list contains labels with ".entry" type */
    if (p != NULL)
    {
        ext_file = fopen(file_name, "w+");
        while (current != NULL)
        {
            current_name = getLabelName(current);
            /* The label type is ".extern" */
            if (getLabelType(current) == 1)
                fprintf(ext_file, "%s\t\t%d\n", current_name, getLabelIndex(current));
            if (current_name != NULL)
                free(current_name);
            current = getLabelNext(current);
        }
        fclose(ext_file);
    }
}

/* Create a new file with the given file name and print the instructions  and data to it */
void printInstructions(my_binary *head_instruction, my_binary *head_data, char *file_name, int ic, int dc)
{
    FILE *ob_file;
    my_binary *current;
    char *current_code;
    current_code = NULL;
    current = head_instruction;
    ob_file = fopen(file_name, "w+");

    /* Print the ic and dc */
    fprintf(ob_file, "\t%d %d\n", ic, dc);

    /* Print all instructions nodes */
    while (current != NULL)
    {
        current_code = getBinaryCode(current);
        convertToUniqueBinary(current_code);
        fprintf(ob_file, "0%d\t\t%s\n", getBinaryIndex(current) + 100, current_code);
        if (current_code != NULL)
            free(current_code);
        current = getBinaryNext(current);
    }

    current = head_data;

    /* Print all data nodes */
    while (current != NULL)
    {
        current_code = getBinaryCode(current);
        convertToUniqueBinary(current_code);
        fprintf(ob_file, "0%d\t\t%s\n", getBinaryIndex(current) + ic + 100, current_code);
        if (current_code != NULL)
            free(current_code);
        current = getBinaryNext(current);
    }

    fclose(ob_file);
}

/* Update all labels with the IC */
void updateDataLabels(my_label *head, int ic)
{
    my_label *current;
    current = head;
    while (current != NULL)
    {
        /* The the type of the current label is data */
        if (getLabelType(current) == 0)
            setLabelIndex(current, getLabelIndex(current) + ic);
        current = getLabelNext(current);
    }
}

/* Print the given error with the current line */
void printError(int id, int lineIndex)
{
    switch (id)
    {
    case -1:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                      FATAL ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> A memory allocation function such as: calloc(), malloc() and   <!>\n");
        fprintf(stdout, "<!> realloc() has failed. Make sure enough memory is available!    <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!>                        EXITING PROGRAM                         <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 0:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> An unknown command was detected.                               <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 1:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> A source operand must be declared.                             <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 2:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The source operand is not valid. A source operand must be one  <!>\n");
        fprintf(stdout, "<!> of the following: immediate, label or register.                <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 3:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> A target operand must be declared.                             <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 4:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> One comma must be declared between two operands.               <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 5:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The target operand is not valid. A target operand must be one  <!>\n");
        fprintf(stdout, "<!> of the following: immediate, label or register.                <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 6:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> No valid inputs are allowed after target operand.              <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 7:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> While using the \"lea\" instruction, the source operand must     <!>\n");
        fprintf(stdout, "<!> be a label, and the target operand must be a label or a        <!>\n");
        fprintf(stdout, "<!> register.                                                      <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 8:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> While using the \"mov\", \"add\", \"sub\", \"not\", \"clr\"              <!>\n");
        fprintf(stdout, "<!> \"inc\", \"dec\", \"jmp\", \"bne\", \"red\" and \"jsr\"                    <!>\n");
        fprintf(stdout, "<!> instructions, the target operand can not be an immediate       <!>\n");
        fprintf(stdout, "<!> number.                                                        <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 9:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> While using the \"jmp\", \"bne\" and \"jsr\" instructions, the       <!>\n");
        fprintf(stdout, "<!> target operand can only be a valid label.                      <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 10:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> Parameters must be declared using parenthesis. for example:    <!>\n");
        fprintf(stdout, "<!> \"(par1,par2)\"                                                  <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 11:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The first parameter is not valid. A parameter must be one of   <!>\n");
        fprintf(stdout, "<!> the following: immediate, label or register.                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 12:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> A second parameter must be declared.                           <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 13:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> One comma must be declared between two parameters.             <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 14:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The second parameter is not valid. A parameter must be one of  <!>\n");
        fprintf(stdout, "<!> the following: immediate, label or register.                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 15:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> After the second parameter, a closing parenthensis must be     <!>\n");
        fprintf(stdout, "<!> declared and nothing more.                                     <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 16:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \"jmp\", \"bne\" and \"jsr\" are the only instructions that          <!>\n");
        fprintf(stdout, "<!> can preform a jump sequence with parameters.                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 17:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> No valid inputs are allowed after the instructions \"rts\"       <!>\n");
        fprintf(stdout, "<!> and \"stop\".                                                    <!>\n");
        fprintf(stdout, "<!>------------------------------------------------------------------<!>\n\n\n");
        break;
    case 18:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".data\" structures must contain at least one integer.          <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 19:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> No valid inputs are allowed after the last parameter in        <!>\n");
        fprintf(stdout, "<!> \".data\" and \".string\" structures.                              <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 20:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".data\" structures must contain one comma between two          <!>\n");
        fprintf(stdout, "<!> integers.                                                      <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 21:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".data\" structures must contain only integers between -8192    <!>\n");
        fprintf(stdout, "<!>  to 8191.                                                      <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 22:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".string\" structures must be initialized using two \"           <!>\n");
        fprintf(stdout, "<!> symbols, for example:                                          <!>\n");
        fprintf(stdout, "<!> .string    \"abcd\"                                              <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 23:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".string\" structures must contain a single string with a       <!>\n");
        fprintf(stdout, "<!> minimum length of at least 1.                                  <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 24:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> Unknown command was detected. In order to define a label at    <!>\n");
        fprintf(stdout, "<!> the beginning of a line, The ':' sign must appear immediately  <!>\n");
        fprintf(stdout, "<!> after the label.                                               <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 25:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The label is not valid. A label must answer all of             <!>\n");
        fprintf(stdout, "<!> the following:                                                 <!>\n");
        fprintf(stdout, "<!>    - The length must be between 1 to 30.                       <!>\n");
        fprintf(stdout, "<!>    - The first character must be alphabetic.                   <!>\n");
        fprintf(stdout, "<!>    - The rest of the characters must be alphabetic             <!>\n");
        fprintf(stdout, "<!>      or digits.                                                <!>\n");
        fprintf(stdout, "<!>    - A label can not be a saved-assembler phrase.              <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 26:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> \".extern\" and \".entry\" structures can only recieve one         <!>\n");
        fprintf(stdout, "<!>  valid label as a parameter.                                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 27:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> No valid inputs are allowed after the last parameter in        <!>\n");
        fprintf(stdout, "<!> \".extern\" and \".entry\" structures.                             <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 28:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The label is already defined, therefore, it is not possible to <!>\n");
        fprintf(stdout, "<!> change the type from \"external\" to \"internal\" and              <!>\n");
        fprintf(stdout, "<!> vice versa.                                                    <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 29:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        WARNING AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The label is already defined as \"external\". Redefining the     <!>\n");
        fprintf(stdout, "<!> label as \"external\" will not consider as an error, but it      <!>\n");
        fprintf(stdout, "<!> is unecessary.                                                 <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 30:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        WARNING AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> Label definition at the beginning of \".extern\" or \".entry\"     <!>\n");
        fprintf(stdout, "<!> command will not be applied.                                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 31:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The label is already defined, therefore, it is not possible    <!>\n");
        fprintf(stdout, "<!> to define it again.                                            <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 32:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> After label definition a command must apeared.                 <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 33:
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "                        ERROR AT LINE %d\n", lineIndex);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!> The label has not been defined at the current file. Therefore, <!>\n");
        fprintf(stdout, "<!> it is not possible to define it as \".entry\".                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    case 34:
        fprintf(stdout, "<!> The label has not been defined at the current file. Therefore, <!>\n");
        fprintf(stdout, "<!> it is not possible to convert the label into a binary form.    <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        break;
    default:
        break;
    }
}

/* For preAssembler */
/* Replace current line with macro lines */
void replaceLineWithMacro(my_line **head, my_macro *macro)
{
    my_line *newFileLine;
    my_line *lastFileLine;
    my_line *currentMacroLine;
    char *currentMacroLineCode;
    currentMacroLineCode = NULL;
    lastFileLine = *head;
    currentMacroLine = getMacroLines(macro);
    currentMacroLineCode = NULL;

    /* The head is not empty */
    if (lastFileLine != NULL)
    {
        /* Move to the last line in the head */
        while (getLineNext(lastFileLine) != NULL)
            lastFileLine = getLineNext(lastFileLine);

        /* Duplicate each line from macro the last position in head */
        while (currentMacroLine != NULL)
        {
            currentMacroLineCode = getLineCode(currentMacroLine);
            newFileLine = newLine(currentMacroLineCode);
            setLineNext(lastFileLine, newFileLine);
            lastFileLine = getLineNext(lastFileLine);
            currentMacroLine = getLineNext(currentMacroLine);
            if (currentMacroLineCode != NULL)
                free(currentMacroLineCode);
            currentMacroLineCode = NULL;
        }
    }
    else
    {
        /* Define a new head lines */
        currentMacroLineCode = getLineCode(currentMacroLine);
        newFileLine = newLine(currentMacroLineCode);
        *head = newFileLine;
        lastFileLine = *head;
        currentMacroLine = getLineNext(currentMacroLine);
        if (currentMacroLineCode != NULL)
            free(currentMacroLineCode);
        currentMacroLineCode = NULL;

        /* Duplicate each line from macro the last position in head */
        while (currentMacroLine != NULL)
        {
            currentMacroLineCode = getLineCode(currentMacroLine);
            newFileLine = newLine(currentMacroLineCode);
            setLineNext(lastFileLine, newFileLine);
            lastFileLine = getLineNext(lastFileLine);
            currentMacroLine = getLineNext(currentMacroLine);
            if (currentMacroLineCode != NULL)
                free(currentMacroLineCode);
            currentMacroLineCode = NULL;
        }
    }
}

/* Generat a new string with ".as" in the end */
char *generateAsName(char fullName[])
{
    char *newName;
    int i;
    i = strlen(fullName);
    if (i <= 253)
    {
        newName = (char *)malloc(i + 4);
        memcpy(newName, fullName, i);
        *(newName + i) = '.';
        *(newName + i + 1) = 'a';
        *(newName + i + 2) = 's';
        *(newName + i + 3) = '\0';
    }
    else
    {
        newName = (char *)malloc(257);
        memcpy(newName, fullName, 256);
        *(newName + 253) = '.';
        *(newName + 254) = 'a';
        *(newName + 255) = 's';
        *(newName + 256) = '\0';
    }

    return newName;
}

/* Generat a new string with ".am" in the end */
char *generateAmName(char fullName[])
{
    char *newName;
    int i;
    i = strlen(fullName);
    if (i <= 253)
    {
        newName = (char *)malloc(i + 4);
        memcpy(newName, fullName, i);
        *(newName + i) = '.';
        *(newName + i + 1) = 'a';
        *(newName + i + 2) = 'm';
        *(newName + i + 3) = '\0';
    }
    else
    {
        newName = (char *)malloc(257);
        memcpy(newName, fullName, 256);
        *(newName + 253) = '.';
        *(newName + 254) = 'a';
        *(newName + 255) = 'm';
        *(newName + 256) = '\0';
    }

    return newName;
}

/* Return 1 if the current string only contains nothing more than whitespaces, else return 0 */
int isStringEmpty(char *p)
{
    int result = 1;
    int i = 0;
    while (isWhitespace(*(p + i)))
        i++;
    if (*(p + i) != '\n' && *(p + i) != '\0' && *(p + i) != '\r')
        result = 0;
    return result;
}

/* Returns a pointer to the first none-whitespace string in a line */
char *firstMacroString(char *p)
{
    char *pos;
    char *str;
    int j = 0;
    str = (char *)malloc(1);
    *str = '\r';
    pos = p;
    while (isWhitespace(*pos))
    {
        pos += 1;
    }
    if (*pos != '\n' && *pos != '\0' && *pos != '\r')
    {
        while (isWhitespace(*(pos + j)) != 1 && *(pos + j) != '\n' && *(pos + j) != '\0' && *(pos + j) != '\r')
            j++;
        free(str);
        str = (char *)malloc(j + 1);
        memcpy(str, pos, j);
        *(str + j) = '\0';
    }
    return str;
}