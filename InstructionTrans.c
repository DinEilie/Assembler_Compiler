/*
 * Title: Maman 14 - Assembler Project
 * Description: getInstructionBinary() will return a pointer to the "my_binary" structure with all the information
 *              of an Instruction. This algorithm is responsible for the translation itself to a binary form.
 * Author: Din Eilie
 * Student ID: 208679597
 */

#include "header.h"

my_binary *getInstructionBinary(int index, int *errors, int *ic, char *line, my_label *labels)
{
    my_binary *head;
    my_binary *last;
    int valid;
    int comma_index;
    int isSourceImmediate;
    int isSourceLabel;
    int isSourceRegister;
    int isTargetImmediate;
    int isTargetLabel;
    int isTargetRegister;
    int isFirstParameterImmediate;
    int isFirstParameterLabel;
    int isFirstParameterRegister;
    int isSecondParameterImmediate;
    int isSecondParameterLabel;
    int isSecondParameterRegister;
    char *first_phrase;
    char *second_phrase;
    char *third_phrase;
    char *first_parameter;
    char *second_parameter;
    char *tmpBinary;
    first_phrase = NULL;
    second_phrase = NULL;
    third_phrase = NULL;
    first_parameter = NULL;
    second_parameter = NULL;
    tmpBinary = NULL;

    /* Return null if the line is empty */
    if (isLineEmpty(line) == 1)
    {
        head = NULL;
    }
    else
    {
        first_phrase = firstString(line);
        valid = (isInstruction(first_phrase));
        /* The first phrase is a valid instruction */
        if (valid != -1)
        {
            /* The instruction is one of the following: "mov", "cmp", "add", "sub", "lea" */
            if ((valid >= 0 && valid <= 3) || valid == 6)
            {
                /* Check if the instruction is the only phrase in the line */
                if (strlen(strstr(line, first_phrase)) - 3 > 0)
                {
                    /* Check if the source operand is a valid: immediate, label, register */
                    second_phrase = firstOperand(strstr(line, first_phrase) + 3);
                    isSourceImmediate = isValidImmediate(second_phrase);
                    isSourceLabel = isValidLabel(second_phrase);
                    isSourceRegister = isValidRegister(second_phrase);
                    if (isSourceImmediate != -3000 || isSourceLabel || isSourceRegister != -1)
                    {
                        /* Check if the source operand is the last phrase in the line */
                        if (strlen(strstr(line, second_phrase)) - strlen(second_phrase) > 0)
                        {
                            /* Check there is one comma between the source operand and the target operand */
                            comma_index = isValidComma(strstr(line, second_phrase) + strlen(second_phrase));
                            if (comma_index != -1)
                            {
                                /* Check if the target operand is a valid: immediate, label, register */
                                third_phrase = firstString(strstr(line, second_phrase) + strlen(second_phrase) + comma_index);
                                isTargetImmediate = isValidImmediate(third_phrase);
                                isTargetLabel = isValidLabel(third_phrase);
                                isTargetRegister = isValidRegister(third_phrase);
                                if (isTargetImmediate != -3000 || isTargetLabel || isTargetRegister != -1)
                                {
                                    /* Check that there is no input (except comments) after the target operand  */
                                    if (isLineEmpty(strstr(strstr(line, second_phrase) + strlen(second_phrase) + comma_index, third_phrase) + strlen(third_phrase)))
                                    {
                                        /* The instruction is one of the following: "mov", "add", "sub" */
                                        if (valid != 1 && valid != 6)
                                        {
                                            /* The instruction can not recieve an immediate as a destination */
                                            if (isTargetImmediate == -3000)
                                            {
                                                /* Create the first binary form */
                                                tmpBinary = (char *)malloc(15);
                                                if (tmpBinary == NULL)
                                                {
                                                    /* Error: memory allocation has failed*/
                                                    printError(-1, 0);
                                                    exit(0);
                                                }
                                                *(tmpBinary + 14) = '\0';
                                                *(tmpBinary + 13) = '0';
                                                *(tmpBinary + 12) = '0';
                                                memset(tmpBinary, '0', 8);
                                                if (valid == 2)
                                                    *(tmpBinary + 6) = '1';
                                                if (valid == 3)
                                                {
                                                    *(tmpBinary + 6) = '1';
                                                    *(tmpBinary + 7) = '1';
                                                }

                                                /* Both operands are registers, therefore create a combine binary form */
                                                if ((isSourceRegister != -1) && (isTargetRegister != -1))
                                                {
                                                    /* Finish head binary form */
                                                    *(tmpBinary + 8) = '1';
                                                    *(tmpBinary + 9) = '1';
                                                    *(tmpBinary + 10) = '1';
                                                    *(tmpBinary + 11) = '1';
                                                    head = newBinary(*ic, tmpBinary, NULL);

                                                    free(tmpBinary);

                                                    /* Create a combination binary registers form*/
                                                    last = newBinary(*ic + 1, convertRegisterToBinary(isSourceRegister, isTargetRegister), NULL);
                                                    setBinaryNext(head, last);

                                                    /* Update the ic */
                                                    *ic += 2;
                                                }
                                                /* Source operand is register and target operand is not immediate or register (therefore it is label) */
                                                else if (isSourceRegister != -1)
                                                {
                                                    /* Finish head binary form */
                                                    *(tmpBinary + 8) = '1';
                                                    *(tmpBinary + 9) = '1';
                                                    *(tmpBinary + 10) = '0';
                                                    *(tmpBinary + 11) = '1';
                                                    head = newBinary(*ic, tmpBinary, NULL);

                                                    free(tmpBinary);

                                                    /* Create binary form for register source */
                                                    last = newBinary(*ic + 1, convertRegisterToBinary(isSourceRegister, 0), NULL);
                                                    setBinaryNext(head, last);

                                                    /* Create binary form for label target */
                                                    setBinaryNext(last, newBinary(*ic + 2, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                    /* Update the ic */
                                                    *ic += 3;
                                                }
                                                /* Source operand is not a register and target operand is register */
                                                else if (isTargetRegister != -1)
                                                {
                                                    /* Target operand as register */
                                                    *(tmpBinary + 10) = '1';
                                                    *(tmpBinary + 11) = '1';

                                                    /* Source operand is an immediate */
                                                    if (isSourceImmediate != -3000)
                                                    {
                                                        /* Source operand as immediate */
                                                        *(tmpBinary + 8) = '0';
                                                        *(tmpBinary + 9) = '0';
                                                        head = newBinary(*ic, tmpBinary, NULL);

                                                        free(tmpBinary);

                                                        /* Create binary form for immediate source */
                                                        last = newBinary(*ic + 1, convertImmediateToBinary(isSourceImmediate), NULL);
                                                        setBinaryNext(head, last);

                                                        /* Create binary form for register target */
                                                        setBinaryNext(last, newBinary(*ic + 2, convertRegisterToBinary(0, isTargetRegister), NULL));

                                                        /* Update the ic */
                                                        *ic += 3;
                                                    }
                                                    /* Source operand is a label */
                                                    else
                                                    {
                                                        /* Source operand as label */
                                                        *(tmpBinary + 8) = '0';
                                                        *(tmpBinary + 9) = '1';
                                                        head = newBinary(*ic, tmpBinary, NULL);

                                                        free(tmpBinary);

                                                        /* Create binary form for label source */
                                                        last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                                        setBinaryNext(head, last);

                                                        /* Create binary form for register target */
                                                        setBinaryNext(last, newBinary(*ic + 2, convertRegisterToBinary(0, isTargetRegister), NULL));

                                                        /* Update the ic */
                                                        *ic += 3;
                                                    }
                                                }
                                                /* Source operand is not register and target operand is not immediate or register (therefore it is label) */
                                                else
                                                {
                                                    /* Target operand as label */
                                                    *(tmpBinary + 10) = '0';
                                                    *(tmpBinary + 11) = '1';

                                                    /* Source operand is an immediate */
                                                    if (isSourceImmediate != -3000)
                                                    {
                                                        /* Source operand as immediate */
                                                        *(tmpBinary + 8) = '0';
                                                        *(tmpBinary + 9) = '0';
                                                        head = newBinary(*ic, tmpBinary, NULL);

                                                        free(tmpBinary);

                                                        /* Create binary form for immediate source */
                                                        last = newBinary(*ic + 1, convertImmediateToBinary(isSourceImmediate), NULL);
                                                        setBinaryNext(head, last);

                                                        /* Create binary form for label target */
                                                        setBinaryNext(last, newBinary(*ic + 2, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                        /* Update the ic */
                                                        *ic += 3;
                                                    }
                                                    /* Source operand is a label */
                                                    else
                                                    {
                                                        /* Source operand a label */
                                                        *(tmpBinary + 8) = '0';
                                                        *(tmpBinary + 9) = '1';
                                                        head = newBinary(*ic, tmpBinary, NULL);

                                                        free(tmpBinary);

                                                        /* Create binary form for label source */
                                                        last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                                        setBinaryNext(head, last);

                                                        /* Create binary form for label target */
                                                        setBinaryNext(last, newBinary(*ic + 2, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                        /* Update the ic */
                                                        *ic += 3;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                /* Error: target operand can not be immediate */
                                                printError(8, index);
                                                head = NULL;
                                                *errors += 1;
                                            }
                                        }
                                        /* The instruction is "cmp" */
                                        else if (valid == 1)
                                        {
                                            /* Create the first binary form */
                                            tmpBinary = (char *)malloc(15);
                                            if (tmpBinary == NULL)
                                            {
                                                /* Error: memory allocation has failed*/
                                                printError(-1, 0);
                                                exit(0);
                                            }
                                            *(tmpBinary + 14) = '\0';
                                            *(tmpBinary + 13) = '0';
                                            *(tmpBinary + 12) = '0';
                                            memset(tmpBinary, '0', 8);
                                            *(tmpBinary + 7) = '1';

                                            /* Source operand is immediate */
                                            if (isSourceImmediate != -3000)
                                            {
                                                /* Source operand as immediate */
                                                *(tmpBinary + 8) = '0';
                                                *(tmpBinary + 9) = '0';
                                                head = newBinary(*ic, NULL, NULL);

                                                /* Create binary form for immediate source */
                                                last = newBinary(*ic + 1, convertImmediateToBinary(isSourceImmediate), NULL);
                                                setBinaryNext(head, last);
                                            }
                                            /* Source operand is label */
                                            else if (isSourceLabel)
                                            {
                                                /* Source operand as label */
                                                *(tmpBinary + 8) = '0';
                                                *(tmpBinary + 9) = '1';
                                                head = newBinary(*ic, NULL, NULL);

                                                /* Create binary form for label source */
                                                last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                                setBinaryNext(head, last);
                                            }
                                            /* Source operand is register */
                                            else
                                            {
                                                /* Source operand as register */
                                                *(tmpBinary + 8) = '1';
                                                *(tmpBinary + 9) = '1';
                                                head = newBinary(*ic, NULL, NULL);

                                                /* Create binary form for register source */
                                                last = newBinary(*ic + 1, convertRegisterToBinary(isSourceRegister, 0), NULL);
                                                setBinaryNext(head, last);
                                            }

                                            /* Target operand is immediate */
                                            if (isTargetImmediate != -3000)
                                            {
                                                /* Target operand as immediate */
                                                *(tmpBinary + 10) = '0';
                                                *(tmpBinary + 11) = '0';
                                                setBinaryCode(head, tmpBinary);

                                                free(tmpBinary);

                                                /* Create binary form for immediate target */
                                                setBinaryNext(last, newBinary(*ic + 2, convertImmediateToBinary(isTargetImmediate), NULL));

                                                /* Update the ic */
                                                *ic += 3;
                                            }
                                            /* Target operand is label */
                                            else if (isTargetLabel)
                                            {
                                                /* Target operand as label */
                                                *(tmpBinary + 10) = '0';
                                                *(tmpBinary + 11) = '1';
                                                setBinaryCode(head, tmpBinary);

                                                free(tmpBinary);

                                                /* Create binary form for label target */
                                                setBinaryNext(last, newBinary(*ic + 2, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                /* Update the ic */
                                                *ic += 3;
                                            }
                                            /* Target operand is register */
                                            else
                                            {
                                                /* Target operand as register */
                                                *(tmpBinary + 10) = '1';
                                                *(tmpBinary + 11) = '1';
                                                setBinaryCode(head, tmpBinary);

                                                free(tmpBinary);

                                                /* Both operands are registers, therefore create a combine binary form */
                                                if (isSourceRegister != -1)
                                                {
                                                    setBinaryCode(last, convertRegisterToBinary(isSourceRegister, isTargetRegister));

                                                    /* Update the ic */
                                                    *ic += 2;
                                                }
                                                /* Create binary form for register target */
                                                else
                                                {
                                                    /* Create binary form for register target */
                                                    setBinaryNext(last, newBinary(*ic + 2, convertRegisterToBinary(0, isTargetRegister), NULL));

                                                    /* Update the ic */
                                                    *ic += 3;
                                                }
                                            }
                                        }
                                        /* The instruction is "lea" */
                                        else
                                        {
                                            if (isSourceLabel && isTargetImmediate == -3000)
                                            {
                                                /* Create the first binary form */
                                                head = newBinary(*ic, NULL, NULL);
                                                tmpBinary = (char *)malloc(15);
                                                if (tmpBinary == NULL)
                                                {
                                                    /* Error: memory allocation has failed*/
                                                    printError(-1, 0);
                                                    exit(0);
                                                }

                                                *(tmpBinary + 14) = '\0';
                                                *(tmpBinary + 13) = '0';
                                                *(tmpBinary + 12) = '0';
                                                memset(tmpBinary, '0', 8);
                                                *(tmpBinary + 5) = '1';
                                                *(tmpBinary + 6) = '1';
                                                *(tmpBinary + 8) = '0';
                                                *(tmpBinary + 9) = '1';

                                                /* Create binary form for label source */
                                                last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                                setBinaryNext(head, last);

                                                /* Target operand is label */
                                                if (isTargetLabel)
                                                {
                                                    /* Target operand as label */
                                                    *(tmpBinary + 10) = '0';
                                                    *(tmpBinary + 11) = '1';
                                                    setBinaryCode(head, tmpBinary);

                                                    free(tmpBinary);

                                                    /* Create binary form for label target */
                                                    setBinaryNext(last, newBinary(*ic + 2, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                    /* Update the ic */
                                                    *ic += 3;
                                                }
                                                /* Target operand is register */
                                                else
                                                {
                                                    /* Target operand as register */
                                                    *(tmpBinary + 10) = '1';
                                                    *(tmpBinary + 11) = '1';
                                                    setBinaryCode(head, tmpBinary);

                                                    free(tmpBinary);

                                                    /* Create binary form for register target */
                                                    setBinaryNext(last, newBinary(*ic + 2, convertRegisterToBinary(0, isTargetRegister), NULL));

                                                    /* Update the ic */
                                                    *ic += 3;
                                                }
                                            }
                                            else
                                            {
                                                /* Error: source operand can only be label and target operand can only be label or register */
                                                printError(7, index);
                                                head = NULL;
                                                *errors += 1;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        /* Error: only comments are valid input after target operand */
                                        printError(6, index);
                                        head = NULL;
                                        *errors += 1;
                                    }
                                }
                                else
                                {
                                    /* Error: the target operand is invalid */
                                    printError(5, index);
                                    head = NULL;
                                    *errors += 1;
                                }
                            }
                            else
                            {
                                /* Error: only one comma must be declared between two operands */
                                printError(4, index);
                                head = NULL;
                                *errors += 1;
                            }
                        }
                        else
                        {
                            /* Error: Target operand was not declared */
                            printError(3, index);
                            head = NULL;
                            *errors += 1;
                        }
                    }
                    else
                    {
                        /* Error: the source operand is invalid */
                        printError(2, index);
                        head = NULL;
                        *errors += 1;
                    }
                }
                else
                {
                    /* Error: Not enough operands were declared */
                    printError(1, index);
                    head = NULL;
                    *errors += 1;
                }
            }
            /* The instruction is one of the following: "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr" */
            else if ((valid >= 4 && valid <= 5) || (valid >= 7 && valid <= 13))
            {
                /* Check if the instruction is the only phrase in the line */
                if (strlen(strstr(line, first_phrase)) - 3 > 0)
                {
                    /* Check if the target operand is a valid: immediate, label, register */
                    second_phrase = firstString(strstr(line, first_phrase) + 3);
                    isTargetImmediate = isValidImmediate(second_phrase);
                    isTargetLabel = isValidLabel(second_phrase);
                    isTargetRegister = isValidRegister(second_phrase);
                    if (isTargetImmediate != -3000 || isTargetLabel || isTargetRegister != -1)
                    {
                        /* Check that there is no input (except comments) after the target operand  */
                        if (isLineEmpty(strstr(line, second_phrase) + strlen(second_phrase)))
                        {
                            /* The instruction is one of the following: "not", "clr", "inc", "dec", "jmp", "bne", "red", "jsr" */
                            if (valid != 12)
                            {
                                if (isTargetImmediate == -3000)
                                {
                                    /* Create the first binary form */
                                    tmpBinary = (char *)malloc(15);
                                    if (tmpBinary == NULL)
                                    {
                                        /* Error: memory allocation has failed*/
                                        printError(-1, 0);
                                        exit(0);
                                    }
                                    *(tmpBinary + 14) = '\0';
                                    *(tmpBinary + 13) = '0';
                                    *(tmpBinary + 12) = '0';
                                    memset(tmpBinary, '0', 10);
                                    switch (valid)
                                    {
                                    /* The instruction is "not" */
                                    case 4:
                                        *(tmpBinary + 5) = '1';
                                        break;
                                    /* The instruction is "clr" */
                                    case 5:
                                        *(tmpBinary + 5) = '1';
                                        *(tmpBinary + 7) = '1';
                                        break;
                                    /* The instruction is "inc" */
                                    case 7:
                                        *(tmpBinary + 5) = '1';
                                        *(tmpBinary + 6) = '1';
                                        *(tmpBinary + 7) = '1';
                                        break;
                                    /* The instruction is "dec" */
                                    case 8:
                                        *(tmpBinary + 4) = '1';
                                        break;
                                    /* The instruction is "jmp" */
                                    case 9:
                                        *(tmpBinary + 4) = '1';
                                        *(tmpBinary + 7) = '1';
                                        break;
                                    /* The instruction is "bne" */
                                    case 10:
                                        *(tmpBinary + 4) = '1';
                                        *(tmpBinary + 6) = '1';
                                        break;
                                    /* The instruction is "red" */
                                    case 11:
                                        *(tmpBinary + 4) = '1';
                                        *(tmpBinary + 6) = '1';
                                        *(tmpBinary + 7) = '1';
                                        break;
                                    /* The instruction is "jsr" */
                                    case 13:
                                        *(tmpBinary + 4) = '1';
                                        *(tmpBinary + 5) = '1';
                                        *(tmpBinary + 7) = '1';
                                        break;
                                    }

                                    /* Target operand is label */
                                    if (isTargetLabel)
                                    {
                                        *(tmpBinary + 10) = '0';
                                        *(tmpBinary + 11) = '1';
                                        head = newBinary(*ic, tmpBinary, NULL);

                                        free(tmpBinary);

                                        /* Create binary form for label target */
                                        last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                        setBinaryNext(head, last);

                                        /* Update the ic */
                                        *ic += 2;
                                    }
                                    /* Target operand is register */
                                    else
                                    {
                                        if (valid != 9 && valid != 10 && valid != 13)
                                        {
                                            *(tmpBinary + 10) = '1';
                                            *(tmpBinary + 11) = '1';
                                            head = newBinary(*ic, tmpBinary, NULL);

                                            free(tmpBinary);

                                            /* Create binary form for label target */
                                            last = newBinary(*ic + 1, convertRegisterToBinary(0, isTargetRegister), NULL);
                                            setBinaryNext(head, last);

                                            /* Update the ic */
                                            *ic += 2;
                                        }
                                        else
                                        {
                                            /* Error: the instruction "jmp", "bne" and "jsr" can only recieve label as a traget operand */
                                            printError(9, index);
                                            head = NULL;
                                            *errors += 1;
                                        }
                                    }
                                }
                                else
                                {
                                    /* Error: target operand can not be immediate */
                                    printError(8, index);
                                    head = NULL;
                                    *errors += 1;
                                }
                            }
                            /* The instruction is "prn" */
                            else
                            {
                                /* Create the first binary form */
                                tmpBinary = (char *)malloc(15);
                                if (tmpBinary == NULL)
                                {
                                    /* Error: memory allocation has failed*/
                                    printError(-1, 0);
                                    exit(0);
                                }

                                *(tmpBinary + 14) = '\0';
                                *(tmpBinary + 13) = '0';
                                *(tmpBinary + 12) = '0';
                                memset(tmpBinary, '0', 10);
                                *(tmpBinary + 5) = '1';
                                *(tmpBinary + 4) = '1';

                                /* Target operand is immediate */
                                if (isTargetImmediate != -3000)
                                {
                                    *(tmpBinary + 10) = '0';
                                    *(tmpBinary + 11) = '0';
                                    head = newBinary(*ic, tmpBinary, NULL);

                                    free(tmpBinary);

                                    /* Create binary form for immediate target */
                                    last = newBinary(*ic + 1, convertImmediateToBinary(isTargetImmediate), NULL);
                                    setBinaryNext(head, last);

                                    /* Update the ic */
                                    *ic += 2;
                                }
                                /* Target operand is label */
                                else if (isTargetLabel)
                                {
                                    *(tmpBinary + 10) = '0';
                                    *(tmpBinary + 11) = '1';
                                    head = newBinary(*ic, tmpBinary, NULL);

                                    free(tmpBinary);

                                    /* Create binary form for label target */
                                    last = newBinary(*ic + 1, convertLabelToBinary(labels, second_phrase), second_phrase);
                                    setBinaryNext(head, last);

                                    /* Update the ic */
                                    *ic += 2;
                                }
                                /* Target operand is register */
                                else
                                {
                                    *(tmpBinary + 10) = '1';
                                    *(tmpBinary + 11) = '1';
                                    head = newBinary(*ic, tmpBinary, NULL);

                                    free(tmpBinary);

                                    /* Create binary form for label target */
                                    last = newBinary(*ic + 1, convertRegisterToBinary(0, isTargetRegister), NULL);
                                    setBinaryNext(head, last);

                                    /* Update the ic */
                                    *ic += 2;
                                }
                            }
                        }
                        else
                        {
                            /* Error: only comments are valid input after target operand */
                            printError(6, index);
                            head = NULL;
                            *errors += 1;
                        }
                    }
                    else
                    {
                        /* Check if the target operand contains a valid label until the '(' char */
                        third_phrase = firstLabel(second_phrase);
                        isTargetLabel = isValidLabel(third_phrase);
                        if (strlen(strstr(line, third_phrase)) - strlen(third_phrase) > 0)
                        {
                            /* Target operand contains valid label */
                            if (isTargetLabel)
                            {
                                /* The next char after the valid label is indeed '(' */
                                if (*(strstr(line, third_phrase) + strlen(third_phrase)) == '(')
                                {
                                    /* Check if the first parameter is a valid: immediate, label, register */
                                    first_parameter = firstOperand(strstr(line, third_phrase) + strlen(third_phrase) + 1);
                                    isFirstParameterImmediate = isValidImmediate(first_parameter);
                                    isFirstParameterLabel = isValidLabel(first_parameter);
                                    isFirstParameterRegister = isValidRegister(first_parameter);
                                    if (isFirstParameterImmediate != -3000 || isFirstParameterLabel || isFirstParameterRegister != -1)
                                    {
                                        if (strlen(strstr(line, third_phrase) + strlen(third_phrase) + 1) - strlen(first_parameter) > 0)
                                        {
                                            /* Check there is one comma between the first and second parameters */
                                            comma_index = isValidComma(strstr(strstr(line, third_phrase) + strlen(third_phrase) + 1, first_parameter) + strlen(first_parameter));
                                            if (comma_index != -1)
                                            {
                                                /* Check if the second parameter is a valid: immediate, label, register */
                                                second_parameter = lastParameter(strstr(strstr(line, third_phrase) + strlen(third_phrase) + 1, first_parameter) + strlen(first_parameter) + comma_index);
                                                isSecondParameterImmediate = isValidImmediate(second_parameter);
                                                isSecondParameterLabel = isValidLabel(second_parameter);
                                                isSecondParameterRegister = isValidRegister(second_parameter);
                                                if (isSecondParameterImmediate != -3000 || isSecondParameterLabel || isSecondParameterRegister != -1)
                                                {
                                                    /* Check there is last bracket ')' after the second parameter and noting more */
                                                    if (isValidEndingBracket(strstr(strstr(strstr(line, third_phrase) + strlen(third_phrase) + 1, first_parameter) + strlen(first_parameter) + comma_index, second_parameter) + strlen(second_parameter)) != -1)
                                                    {
                                                        /* The instruction is one of the following: "jmp", "bne", "jsr" */
                                                        if (valid == 9 || valid == 10 || valid == 13)
                                                        {
                                                            /* Create the first binary form */
                                                            tmpBinary = (char *)malloc(15);
                                                            if (tmpBinary == NULL)
                                                            {
                                                                /* Error: memory allocation has failed*/
                                                                printError(-1, 0);
                                                                exit(0);
                                                            }

                                                            /* Finishing the first binary form */
                                                            *(tmpBinary + 14) = '\0';
                                                            *(tmpBinary + 13) = '0';
                                                            *(tmpBinary + 12) = '0';
                                                            *(tmpBinary + 11) = '0';
                                                            *(tmpBinary + 10) = '1';
                                                            memset(tmpBinary, '0', 10);
                                                            switch (valid)
                                                            {
                                                            /* The instruction is "jmp" */
                                                            case 9:
                                                                *(tmpBinary + 4) = '1';
                                                                *(tmpBinary + 7) = '1';
                                                                break;
                                                            /* The instruction is "bne" */
                                                            case 10:
                                                                *(tmpBinary + 4) = '1';
                                                                *(tmpBinary + 6) = '1';
                                                                break;
                                                            /* The instruction is "jsr" */
                                                            case 13:
                                                                *(tmpBinary + 4) = '1';
                                                                *(tmpBinary + 5) = '1';
                                                                *(tmpBinary + 7) = '1';
                                                                break;
                                                            }

                                                            /* Create head node and the target node */
                                                            head = newBinary(*ic, NULL, NULL);
                                                            setBinaryNext(head, newBinary(*ic + 1, convertLabelToBinary(labels, third_phrase), third_phrase));

                                                            /* The first parameter is an immediate number */
                                                            if (isFirstParameterImmediate != -3000)
                                                            {
                                                                /* Create the third binary form */
                                                                last = newBinary(*ic + 2, convertImmediateToBinary(isFirstParameterImmediate), NULL);
                                                            }
                                                            /* The first parameter is label */
                                                            else if (isFirstParameterLabel)
                                                            {
                                                                *(tmpBinary + 1) = '1';

                                                                /* Create the third binary form */
                                                                last = newBinary(*ic + 2, convertLabelToBinary(labels, first_parameter), first_parameter);
                                                            }
                                                            /* The first parameter is register */
                                                            else
                                                            {
                                                                *(tmpBinary) = '1';
                                                                *(tmpBinary + 1) = '1';

                                                                /* Create the third binary form */
                                                                last = newBinary(*ic + 2, convertRegisterToBinary(isFirstParameterRegister, 0), NULL);
                                                            }

                                                            /* The second parameter is an immediate number */
                                                            if (isSecondParameterImmediate != -3000)
                                                            {
                                                                setBinaryCode(head, tmpBinary);
                                                                free(tmpBinary);

                                                                /* Create the fourth binary form */
                                                                setBinaryNext(last, newBinary(*ic + 3, convertImmediateToBinary(isSecondParameterImmediate), NULL));

                                                                /* Update the ic */
                                                                *ic += 4;
                                                            }
                                                            /* The second parameter is label */
                                                            else if (isSecondParameterLabel)
                                                            {
                                                                *(tmpBinary + 3) = '1';
                                                                setBinaryCode(head, tmpBinary);
                                                                free(tmpBinary);

                                                                /* Create the fourth binary form */
                                                                setBinaryNext(last, newBinary(*ic + 3, convertLabelToBinary(labels, second_parameter), second_parameter));

                                                                /* Update the ic */
                                                                *ic += 4;
                                                            }
                                                            /* The second parameter is register */
                                                            else
                                                            {
                                                                *(tmpBinary + 2) = '1';
                                                                *(tmpBinary + 3) = '1';
                                                                setBinaryCode(head, tmpBinary);
                                                                free(tmpBinary);

                                                                if (isFirstParameterRegister != -1)
                                                                {
                                                                    setBinaryCode(last, convertRegisterToBinary(isFirstParameterRegister, isSecondParameterRegister));

                                                                    /* Update the ic */
                                                                    *ic += 3;
                                                                }
                                                                else
                                                                {
                                                                    /* Create the fourth binary form */
                                                                    setBinaryNext(last, newBinary(*ic + 3, convertRegisterToBinary(0, isSecondParameterRegister), NULL));

                                                                    /* Update the ic */
                                                                    *ic += 4;
                                                                }
                                                            }

                                                            /* Linking between the head nodes and the last nodes (Instruction -> Label -> First parameter -> Second parameter) */
                                                            setBinaryNext(getBinaryNext(head), last);
                                                        }
                                                        else
                                                        {
                                                            /* Error: only "jmp", "bne", "jsr" are valid instructions while using parameters */
                                                            printError(16, index);
                                                            head = NULL;
                                                            *errors += 1;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        /* Error: second parameter is invalid */
                                                        printError(15, index);
                                                        head = NULL;
                                                        *errors += 1;
                                                    }
                                                }
                                                else
                                                {
                                                    /* Error: second parameter is invalid */
                                                    printError(14, index);
                                                    head = NULL;
                                                    *errors += 1;
                                                }
                                            }
                                            else
                                            {
                                                /* Error: only one comma must be declared between two operands */
                                                printError(13, index);
                                                head = NULL;
                                                *errors += 1;
                                            }
                                        }
                                        else
                                        {
                                            /* Error: second parameter must be declared */
                                            printError(12, index);
                                            head = NULL;
                                            *errors += 1;
                                        }
                                    }
                                    else
                                    {
                                        /* Error: first parameter is invalid */
                                        printError(11, index);
                                        head = NULL;
                                        *errors += 1;
                                    }
                                }
                                else
                                {
                                    /* Error: target operand is invalid */
                                    printError(5, index);
                                    head = NULL;
                                    *errors += 1;
                                }
                            }
                            else
                            {
                                /* Error: target operand is invalid */
                                printError(5, index);
                                head = NULL;
                                *errors += 1;
                            }
                        }
                        else
                        {
                            /* Target operand contains valid label but the target itself is invalid */
                            if (isTargetLabel)
                            {
                                if (valid != 9 && valid != 10 && valid != 13)
                                {
                                    /* Error: target operand is invalid */
                                    printError(5, index);
                                    head = NULL;
                                    *errors += 1;
                                }
                                else
                                {
                                    /* Error: Not enough parameters were declared */
                                    printError(10, index);
                                    head = NULL;
                                    *errors += 1;
                                }
                            }
                            else
                            {
                                /* Error: target operand is invalid */
                                printError(5, index);
                                head = NULL;
                                *errors += 1;
                            }
                        }
                    }
                }
                else
                {
                    /* Error: Not enough operands were declared */
                    printError(3, index);
                    head = NULL;
                    *errors += 1;
                }
            }
            /* The instruction is one of the following: "rts", "stop" */
            else
            {
                /* There is nothing (except comments) after the instruction */
                if (isLineEmpty(strstr(line, first_phrase) + strlen(first_phrase)))
                {
                    /* Create the only binary form */
                    tmpBinary = (char *)malloc(15);
                    if (tmpBinary == NULL)
                    {
                        /* Error: memory allocation has failed*/
                        printError(-1, 0);
                        exit(0);
                    }

                    *(tmpBinary + 14) = '\0';
                    memset(tmpBinary, '0', 14);
                    *(tmpBinary + 4) = '1';
                    *(tmpBinary + 5) = '1';
                    *(tmpBinary + 6) = '1';
                    if (valid == 15)
                        *(tmpBinary + 7) = '1';

                    /* Finnish the binary form */
                    head = newBinary(*ic, tmpBinary, NULL);
                    free(tmpBinary);

                    /* Update the ic */
                    *ic += 1;
                }
                else
                {
                    /* Error: only comments are allowed after the instructions */
                    printError(17, index);
                    head = NULL;
                    *errors += 1;
                }
            }
        }
        else
        {
            /* Error: unknown instruction */
            printError(0, index);
            head = NULL;
            *errors += 1;
        }
    }

    /* Free irrelevant values from memory */
    if (first_phrase != NULL)
        free(first_phrase);
    if (second_phrase != NULL)
        free(second_phrase);
    if (third_phrase != NULL)
        free(third_phrase);
    if (first_parameter != NULL)
        free(first_parameter);
    if (second_parameter != NULL)
        free(second_parameter);

    /* Return final form */
    return head;
}