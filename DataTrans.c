/*
 * Title: Maman 14 - Assembler Project
 * Description: getDataBinary() will return a pointer to the "my_binary" structure with all the information
 *              of a Data command. This algorithm is responsible for the translation itself to a binary form.
 * Author: Din Eilie
 * Student ID: 208679597
 */
#include "header.h"

my_binary *getDataBinary(int index, int *errors, int *dc, char *line, my_label *labels)
{
    my_binary *head;
    my_binary *last;
    int comma_count;
    int current_parameter_len;
    int count;
    int stop;
    int i;
    char *first_phrase;
    char *current_parameter;
    char *current_parameter_pos;
    stop = 0;
    count = 0;
    first_phrase = NULL;
    current_parameter = NULL;

    /* Return null if the line is empty */
    if (isLineEmpty(line) == 1)
    {
        head = NULL;
    }
    else
    {
        first_phrase = firstString(line);
        /* The data type is .data */
        if (strcmp(first_phrase, ".data") == 0)
        {
            if (isLineEmpty(strstr(line, ".data") + 5) != 1)
            {
                /* Get the first parameter value and position */
                current_parameter = firstOperand(strstr(line, ".data") + 5);
                current_parameter_len = strlen(current_parameter);
                current_parameter_pos = strstr(strstr(line, ".data") + 5, current_parameter);

                /* Check if the current parameter is valid */
                while (isValidInt(current_parameter) != -9000 && stop != 1)
                {
                    count++;
                    comma_count = isValidComma(current_parameter_pos + current_parameter_len);
                    if (comma_count != -1)
                    {
                        /* There is a (valid or invalid) parameter after the comma */
                        if (*(current_parameter_pos + current_parameter_len + comma_count) != '\n' && *(current_parameter_pos + current_parameter_len + comma_count) != '\0' && *(current_parameter_pos + current_parameter_len + comma_count) != ';' && *(current_parameter_pos + current_parameter_len + comma_count) != '\r')
                        {
                            free(current_parameter);
                            current_parameter = firstOperand(current_parameter_pos + current_parameter_len + comma_count);
                            current_parameter_pos = strstr(current_parameter_pos + current_parameter_len + comma_count, current_parameter);
                            current_parameter_len = strlen(current_parameter);
                        }
                        else
                        {
                            /* Error: .data structure must end without comma or with comments */
                            printError(19, index);
                            head = NULL;
                            stop = 1;
                            count = -1;
                            *errors += 1;
                        }
                    }
                    /* current_parameter is the last valid parameter */
                    else if (isLineEmpty(current_parameter_pos + current_parameter_len) == 1)
                    {
                        /* Stop the loop and start translate into binary */
                        stop = 1;
                    }
                    else
                    {
                        /* Error: in .data structure two parameters must be seprated by one comma */
                        printError(20, index);
                        head = NULL;
                        stop = 1;
                        count = -1;
                        *errors += 1;
                    }
                }

                if (isValidInt(current_parameter) == -9000)
                {
                    /* Error: in .data structure a parameter must be an integer between -8192 to 8191 */
                    printError(21, index);
                    head = NULL;
                    *errors += 1;
                }
                else if (count != -1)
                {
                    /* Get the first parameter value and position */
                    free(current_parameter);
                    current_parameter = firstOperand(strstr(line, ".data") + 5);
                    current_parameter_len = strlen(current_parameter);
                    current_parameter_pos = strstr(strstr(line, ".data") + 5, current_parameter);

                    /* Set the first binary form */
                    head = newBinary(*dc, convertNumberToBinary(isValidInt(current_parameter)), NULL);
                    last = head;
                    for (i = 1; i < count; i++)
                    {
                        /* Get the next parameter value and position */
                        comma_count = isValidComma(current_parameter_pos + current_parameter_len);
                        free(current_parameter);
                        current_parameter = firstOperand(current_parameter_pos + current_parameter_len + comma_count);
                        current_parameter_pos = strstr(current_parameter_pos + current_parameter_len + comma_count, current_parameter);
                        current_parameter_len = strlen(current_parameter);

                        /* Add the next binary form */
                        setBinaryNext(last, newBinary(*dc + i, convertNumberToBinary(isValidInt(current_parameter)), NULL));
                        last = getBinaryNext(last);
                    }

                    /* Update the dc */
                    *dc += count;
                }
            }
            else
            {
                /* Error: .data structure must contains at least one integer */
                printError(18, index);
                head = NULL;
                *errors += 1;
            }
        }
        /* The data type is .string */
        else if (strcmp(first_phrase, ".string") == 0)
        {
            /* The data is initialized */
            if (isLineEmpty(strstr(line, ".string") + 7) != 1)
            {
                first_phrase = firstString(strstr(line, ".string") + 7);
                if (*first_phrase == '"')
                {
                    /* The data has a string starting with-" and ending with-" */
                    count = lastCharPosition(strstr(strstr(line, ".string") + 7, first_phrase) + 1, '"');
                    if (count > 0)
                    {
                        /* There is only comments after the end of the string */
                        if (isLineEmpty(strstr(strstr(line, ".string") + 7, first_phrase) + 2 + count) == 1)
                        {
                            /* Create the first binary form */
                            head = newBinary(*dc, convertNumberToBinary(*(strstr(strstr(line, ".string") + 7, first_phrase) + 1)), NULL);
                            last = head;

                            /* Create the rest of the binary form */
                            for (i = 1; i < count; i++)
                            {
                                setBinaryNext(last, newBinary(*dc + i, convertNumberToBinary(*(strstr(strstr(line, ".string") + 7, first_phrase) + 1 + i)), NULL));
                                last = getBinaryNext(last);
                            }

                            /* Create the last binary form and update the DC */
                            setBinaryNext(last, newBinary(*dc + count, convertNumberToBinary(0), NULL));
                            last = getBinaryNext(last);

                            /* Update the dc */
                            *dc += count + 1;
                        }
                        else
                        {
                            /* Error: .string structure must contains a single string with the length of 1 and above */
                            printError(19, index);
                            head = NULL;
                            *errors += 1;
                        }
                    }
                    else if (count == 0)
                    {
                        /* Error: .string structure must contains a single string with the length of 1 and above */
                        printError(23, index);
                        head = NULL;
                        *errors += 1;
                    }
                    else
                    {
                        /* Error: .string structure must contains at least one string using " for example: .string "YourStringHere" */
                        printError(22, index);
                        head = NULL;
                        *errors += 1;
                    }
                }
                else
                {
                    /* Error: .string structure must contains at least one string using " for example: .string "YourStringHere" */
                    printError(22, index);
                    head = NULL;
                    *errors += 1;
                }
            }
            else
            {
                /* Error: .string structure must contains at least one string using " for example: .string "YourStringHere" */
                printError(22, index);
                head = NULL;
                *errors += 1;
            }
        }
        else
        {
            head = NULL;
        }
    }

    /* Free irrelevant values from memory */
    if (first_phrase != NULL)
        free(first_phrase);
    if (current_parameter != NULL)
        free(current_parameter);

    /* Return result */
    return head;
}