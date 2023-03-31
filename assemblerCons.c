/*
 * Title: Maman 14 - Assembler Project
 * Description: assemblerCons.c is responsible for the assembler construction itself.
 *              This algorithm contains the "First Stage" and the "Second Stage", where as
 *              the "First Stage" is responsible for the main translation of the text, error checking and building
 *              the foundations for the next stage. "Second Stage" is responsible for finnishing the
 *              translation. Only, when both stages are complete without errors (warnings are not count), this algorithm will
 *              print the translation into a ".ob",".ent" and ".ext" files.
 * Author: Din Eilie
 * Student ID: 208679597
 */
#include "header.h"

void assemblerConstruction(char *file_name)
{
    my_binary *head_instruction_binary;
    my_binary *head_data_binary;
    my_label *head_labels;
    my_label *head_external_labels;
    my_binary *current_binary;
    my_label *current_label;
    FILE *file_read;
    char *current_line;
    char *current_binary_label;
    char *current_binary_new_code;
    char *current_binary_code;
    char *first_phrase;
    char *second_phrase;
    char *third_phrase;
    char *fourth_phrase;
    char *new_ob;
    char *new_ent;
    char *new_ext;
    int isFileNotEmpty;
    int isSecondPhraseLabel;
    int isFourthPhraseLabel;
    int line_index;
    int ic;
    int dc;
    int total_errors;
    head_instruction_binary = NULL;
    head_data_binary = NULL;
    head_labels = NULL;
    head_external_labels = NULL;
    current_binary = NULL;
    current_binary_label = NULL;
    current_binary_new_code = NULL;
    current_binary_code = NULL;
    current_line = NULL;
    first_phrase = NULL;
    second_phrase = NULL;
    third_phrase = NULL;
    fourth_phrase = NULL;
    new_ent = NULL;
    new_ext = NULL;
    new_ob = NULL;
    line_index = 1;
    ic = 0;
    dc = 0;
    total_errors = 0;
    file_read = fopen(file_name, "r");

    /* Assembler stage 1 */
    /* Unable to read the file */
    if (file_read == NULL)
    {
        /* Error: Unable to read the file */
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!>                      INTERNAL ERROR                            <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "    Unable to open the file: %s   \n", file_name);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
        total_errors += 1;
    }
    else
    {
        fprintf(stdout, "<$>----------------------------------------------------------------<$>\n");
        fprintf(stdout, "                         STARTING STAGE 1\n");
        fprintf(stdout, "<$>----------------------------------------------------------------<$>\n\n\n");
        current_line = (char *)malloc(SIZE + 2);
        if (fgets(current_line, SIZE + 2, file_read) != NULL)
            isFileNotEmpty = 1;
        while (isFileNotEmpty)
        {
            /* The current line is not entirely empty and is not a comment line */
            if (isLineEmpty(current_line) != 1 && *current_line != ';')
            {
                /* Check if the label is defined correctly */
                first_phrase = firstString(current_line);
                second_phrase = firstLabelDefinition(strstr(current_line, first_phrase));
                isSecondPhraseLabel = isValidLabel(second_phrase);
                if (isSecondPhraseLabel == 1 && *(strstr(current_line, second_phrase) + strlen(second_phrase)) == ':')
                {
                    if (isLineEmpty(strstr(current_line, second_phrase) + strlen(second_phrase) + 1) != 1)
                    {
                        third_phrase = firstString(strstr(current_line, second_phrase) + strlen(second_phrase) + 1);
                        /* The command is ".data" or ".string" */
                        if (strcmp(third_phrase, ".data") == 0 || strcmp(third_phrase, ".string") == 0)
                        {
                            /* The label already exist */
                            current_label = searchLabel(head_labels, second_phrase);
                            if (current_label != NULL)
                            {
                                /* Error: The label already exist */
                                printError(31, line_index);
                                total_errors += 1;
                            }
                            else
                            {
                                /* Create new label for label's list */
                                addToLabelList(&head_labels, newLabel(dc + 100, second_phrase, 0));

                                /* Create new binary form for the command and add it to the list */
                                addToBinaryList(&head_data_binary, getDataBinary(line_index, &total_errors, &dc, strstr(current_line, second_phrase) + strlen(second_phrase) + 1, head_labels));
                            }
                        }
                        /* The command is ".extern" or ".entry" */
                        else if (strcmp(third_phrase, ".extern") == 0 || strcmp(third_phrase, ".entry") == 0)
                        {
                            if (isLineEmpty(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase)) != 1)
                            {
                                /* Check if the label is valid */
                                fourth_phrase = firstString(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase));
                                isFourthPhraseLabel = isValidLabel(fourth_phrase);
                                if (isFourthPhraseLabel == 1)
                                {
                                    /* Check there is no more inputs */
                                    if (isLineEmpty(strstr(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase), fourth_phrase) + strlen(fourth_phrase)) == 1)
                                    {
                                        /* Warning: defining a label at the beginnig of ".extern" or ".entry" is irrelavent */
                                        printError(30, line_index);

                                        /* the parameter is a valid label that already has been added to the label list */
                                        current_label = searchLabel(head_labels, fourth_phrase);
                                        if (current_label != NULL)
                                        {
                                            if (strcmp(third_phrase, ".extern") == 0)
                                            {
                                                /* the current label type is not external */
                                                if (getLabelType(current_label) != 1)
                                                {
                                                    /* Error: can not redefined an internal label as external */
                                                    printError(28, line_index);
                                                    total_errors += 1;
                                                }
                                                else
                                                {
                                                    /* Warning: the current label is already defined as external */
                                                    printError(29, line_index);
                                                }
                                            }
                                            else
                                            {
                                                /* the current label type is external */
                                                if (getLabelType(current_label) == 1)
                                                {
                                                    /* Error: can not redefined an external label as internal */
                                                    printError(28, line_index);
                                                    total_errors += 1;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if (strcmp(third_phrase, ".extern") == 0)
                                            {
                                                /* Create new label for label's list with the type "extern" */
                                                addToLabelList(&head_labels, newLabel(0, second_phrase, 1));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        /* Error: no inputs are available after the last parameter */
                                        printError(27, line_index);
                                        total_errors += 1;
                                    }
                                }
                                else
                                {
                                    /* Error: label is invalid */
                                    printError(25, line_index);
                                    total_errors += 1;
                                }
                            }
                            else
                            {
                                /* Error: a single label is required */
                                printError(26, line_index);
                                total_errors += 1;
                            }
                        }
                        /* The command is instruction or unknown (if unknown then errors will continue to show up) */
                        else
                        {
                            /* The label already exist */
                            current_label = searchLabel(head_labels, second_phrase);
                            if (current_label != NULL)
                            {
                                /* Error: The label already exist */
                                printError(31, line_index);
                                total_errors += 1;
                            }
                            else
                            {
                                /* Create new label for label's list */
                                addToLabelList(&head_labels, newLabel(ic + 100, second_phrase, 3));

                                /* Create new binary form for the command and add it to the list */
                                addToBinaryList(&head_instruction_binary, getInstructionBinary(line_index, &total_errors, &ic, strstr(current_line, second_phrase) + strlen(second_phrase) + 1, head_labels));
                            }
                        }
                    }
                    else
                    {
                        /* Error: after label definition a command must appeared */
                        printError(32, line_index);
                        total_errors += 1;
                    }
                }
                else if (isSecondPhraseLabel == 1)
                {
                    /* Error: label definition must conclude a-':' at the end */
                    printError(24, line_index);
                    total_errors += 1;
                }
                else if (*(strstr(current_line, second_phrase) + strlen(second_phrase)) == ':')
                {
                    /* Error: label is invalid */
                    printError(25, line_index);
                    total_errors += 1;
                }
                /* There is no valid label definition at the beginning of the line */
                else
                {
                    /* The command is ".data" or ".string" */
                    if (strcmp(first_phrase, ".data") == 0 || strcmp(first_phrase, ".string") == 0)
                    {
                        /* Add the data binary form to the Data list */
                        addToBinaryList(&head_data_binary, getDataBinary(line_index, &total_errors, &dc, current_line, head_labels));
                    }
                    /* The command is ".extern" or ".entry" */
                    else if (strcmp(first_phrase, ".extern") == 0 || strcmp(first_phrase, ".entry") == 0)
                    {
                        if (isLineEmpty(strstr(current_line, first_phrase) + strlen(first_phrase)) != 1)
                        {
                            /* Check if the label is valid */
                            second_phrase = firstString(strstr(current_line, first_phrase) + strlen(first_phrase));
                            isSecondPhraseLabel = isValidLabel(second_phrase);
                            if (isSecondPhraseLabel == 1)
                            {
                                /* Check there is no more inputs */
                                if (isLineEmpty(strstr(strstr(current_line, first_phrase) + strlen(first_phrase), second_phrase) + strlen(second_phrase)) == 1)
                                {
                                    /* the parameter is a valid label that already has been added to the label list */
                                    current_label = searchLabel(head_labels, second_phrase);
                                    if (current_label != NULL)
                                    {
                                        if (strcmp(first_phrase, ".extern") == 0)
                                        {
                                            /* the current label type is not external */
                                            if (getLabelType(current_label) != 1)
                                            {
                                                /* Error: can not redefined an internal label as external */
                                                printError(28, line_index);
                                                total_errors += 1;
                                            }
                                            else
                                            {
                                                /* Warning: the current label is already defined as external */
                                                printError(29, line_index);
                                            }
                                        }
                                        else
                                        {
                                            /* the current label type is external */
                                            if (getLabelType(current_label) == 1)
                                            {
                                                /* Error: can not redefined an external label as internal */
                                                printError(28, line_index);
                                                total_errors += 1;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (strcmp(first_phrase, ".extern") == 0)
                                        {
                                            /* Create new label for label's list */
                                            addToLabelList(&head_labels, newLabel(0, second_phrase, 1));
                                        }
                                    }
                                }
                                else
                                {
                                    /* Error: no inputs are available after the last parameter */
                                    printError(27, line_index);
                                    total_errors += 1;
                                }
                            }
                            else
                            {
                                /* Error: label is invalid */
                                printError(25, line_index);
                                total_errors += 1;
                            }
                        }
                        else
                        {
                            /* Error: a single label is required */
                            printError(26, line_index);
                            total_errors += 1;
                        }
                    }
                    /* The command is instruction or unknown */
                    else
                    {
                        addToBinaryList(&head_instruction_binary, getInstructionBinary(line_index, &total_errors, &ic, current_line, head_labels));
                    }
                }
            }

            /* Free irrelevant values from memory */
            if (first_phrase != NULL)
                free(first_phrase);
            if (second_phrase != NULL)
                free(second_phrase);
            if (third_phrase != NULL)
                free(third_phrase);
            if (fourth_phrase != NULL)
                free(fourth_phrase);
            first_phrase = NULL;
            second_phrase = NULL;
            third_phrase = NULL;
            fourth_phrase = NULL;

            /* Move to the next line */
            line_index++;
            if (fgets(current_line, SIZE + 2, file_read) != NULL)
                isFileNotEmpty = 1;
            else
                isFileNotEmpty = 0;
        }

        /* Update the index for all the data labels with the total ic */
        updateDataLabels(head_labels, ic);

        /* Assembler stage 2 */
        fclose(file_read);
        line_index = 1;
        file_read = fopen(file_name, "r");
        fprintf(stdout, "<$>----------------------------------------------------------------<$>\n");
        fprintf(stdout, "                         STARTING STAGE 2\n");
        fprintf(stdout, "<$>----------------------------------------------------------------<$>\n\n\n");

        /* Unable to read the file */
        if (file_read == NULL)
        {
            /* Error: Unable to read the file */
            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
            fprintf(stdout, "<!>                      INTERNAL ERROR                            <!>\n");
            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
            fprintf(stdout, "    Unable to open the file: %s   \n", file_name);
            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
            total_errors += 1;
        }
        else
        {
            if (fgets(current_line, SIZE + 2, file_read) != NULL)
                isFileNotEmpty = 1;
            while (isFileNotEmpty)
            {
                /* The current line is not entirely empty and is not a comment line */
                if (isLineEmpty(current_line) != 1 && *current_line != ';')
                {
                    /* Check if the label is defined correctly */
                    first_phrase = firstString(current_line);
                    second_phrase = firstLabelDefinition(strstr(current_line, first_phrase));
                    isSecondPhraseLabel = isValidLabel(second_phrase);
                    if (isSecondPhraseLabel == 1 && *(strstr(current_line, second_phrase) + strlen(second_phrase)) == ':')
                    {
                        if (isLineEmpty(strstr(current_line, second_phrase) + strlen(second_phrase) + 1) != 1)
                        {
                            /* Check for ".entry" command in order to update the label's list */
                            third_phrase = firstString(strstr(current_line, second_phrase) + strlen(second_phrase) + 1);
                            if (strcmp(third_phrase, ".entry") == 0)
                            {
                                if (isLineEmpty(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase)) != 1)
                                {
                                    fourth_phrase = firstString(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase));
                                    if (isLineEmpty(strstr(strstr(strstr(current_line, second_phrase) + strlen(second_phrase) + 1, third_phrase) + strlen(third_phrase), fourth_phrase) + strlen(fourth_phrase)) == 1)
                                    {

                                        current_label = searchLabel(head_labels, fourth_phrase);
                                        if (current_label != NULL)
                                        {
                                            /* The label has not been defined as external, therefore it is valid */
                                            if (getLabelType(current_label) != 1)
                                                setLabelType(current_label, 2);
                                            else
                                            {
                                                /* Error: can not redefined an external label as internal */
                                                printError(28, line_index);
                                                total_errors += 1;
                                            }
                                        }
                                        else
                                        {
                                            /* Error: the label has not been defined at all */
                                            printError(33, line_index);
                                            total_errors += 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        /* Check for ".entry" command in order to update the label's list */
                        if (strcmp(first_phrase, ".entry") == 0)
                        {
                            if (isLineEmpty(strstr(current_line, ".entry") + 6) != 1)
                            {
                                third_phrase = firstString(strstr(current_line, ".entry") + 6);
                                if (isLineEmpty(strstr(strstr(current_line, ".entry") + 6, third_phrase) + strlen(third_phrase)) == 1)
                                {
                                    current_label = searchLabel(head_labels, third_phrase);
                                    if (current_label != NULL)
                                    {
                                        /* The label has not been defined as external */
                                        if (getLabelType(current_label) != 1)
                                            setLabelType(current_label, 2);
                                        else
                                        {
                                            /* Error: can not redefined an external label as internal */
                                            printError(28, line_index);
                                            total_errors += 1;
                                        }
                                    }
                                    else
                                    {
                                        /* Error: the label has not been defined */
                                        printError(33, line_index);
                                        total_errors += 1;
                                    }
                                }
                            }
                        }
                    }
                }

                /* Free irrelevant values from memory */
                if (first_phrase != NULL)
                    free(first_phrase);
                if (second_phrase != NULL)
                    free(second_phrase);
                if (third_phrase != NULL)
                    free(third_phrase);
                if (fourth_phrase != NULL)
                    free(fourth_phrase);
                first_phrase = NULL;
                second_phrase = NULL;
                third_phrase = NULL;
                fourth_phrase = NULL;

                /* Move to the next line */
                line_index++;
                if (fgets(current_line, SIZE + 2, file_read) != NULL)
                    isFileNotEmpty = 1;
                else
                    isFileNotEmpty = 0;
            }
            fclose(file_read);
        }

        /* Update missing binary forms for labels, for example: ?????????????? will be update to 00000000000010 */
        current_binary = head_instruction_binary;
        while (current_binary != NULL)
        {
            current_binary_label = getBinaryLabel(current_binary);
            current_binary_code = getBinaryCode(current_binary);
            if (current_binary_label != NULL)
            {
                if (*current_binary_code == '?')
                {
                    current_binary_new_code = convertLabel(head_labels, current_binary_label);
                    if (current_binary_new_code != NULL)
                    {
                        /* Update the binary code from ? to a proper binary form */
                        setBinaryCode(current_binary, current_binary_new_code);

                        /* The new binary code is external */
                        if (*(current_binary_new_code + 13) == '1')
                        {
                            /* Save the label with the index of current_binary for the ext file */
                            addToLabelList(&head_external_labels, newLabel(getBinaryIndex(current_binary) + 100, current_binary_label, 1));
                        }
                    }
                    else
                    {
                        /* Error: the label has not been defined */
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "<!>                      INTERNAL ERROR                            <!>\n");
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "           Failed to translate the label: %s\n", current_binary_label);
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        printError(34, line_index);
                        total_errors += 1;
                    }
                }
                else
                {
                    /* The new binary code is external */
                    if (*(current_binary_code + 13) == '1')
                    {
                        /* Save the label with the index of current_binary for the ext file */
                        addToLabelList(&head_external_labels, newLabel(getBinaryIndex(current_binary) + 100, current_binary_label, 1));
                    }
                }
            }

            /* Free irrelevant values from memory */
            if (current_binary_label != NULL)
                free(current_binary_label);
            if (current_binary_code != NULL)
                free(current_binary_code);
            if (current_binary_new_code != NULL)
                free(current_binary_new_code);
            current_binary_label = NULL;
            current_binary_code = NULL;
            current_binary_new_code = NULL;

            /* Move to the next node */
            current_binary = getBinaryNext(current_binary);
        }

        /* Print the results to the new files */
        if (total_errors == 0 && ic + dc <= 156)
        {
            new_ob = generateObName(file_name);
            new_ent = generateEntName(file_name);
            new_ext = generateExtName(file_name);
            printInstructions(head_instruction_binary, head_data_binary, new_ob, ic, dc);
            printEntries(head_labels, new_ent);
            printExternals(head_external_labels, new_ext);
            fprintf(stdout, "<$>****************************************************************<$>\n");
            fprintf(stdout, "                    SUCCESSFULLY CREATED FILES\n");
            fprintf(stdout, "<$>****************************************************************<$>\n\n\n");
        }
        else
        {
            if (ic + dc <= 156)
            {
                fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                fprintf(stdout, "      Total errors encountered: %d\n", total_errors);
                fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
            }
            else
            {
                fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                fprintf(stdout, "      Total errors encountered: %d\n", total_errors + 1);
                fprintf(stdout, "      Over 156 words were used, there are no more memory blocks.\n");
                fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
            }
        }

        /* Free all values form memory */
        freeBinaryChain(&head_instruction_binary);
        freeBinaryChain(&head_data_binary);
        freeBinaryChain(&current_binary);
        freeLabelChain(&head_external_labels);
        freeLabelChain(&head_labels);
        if (current_line != NULL)
            free(current_line);
        if (current_binary_code != NULL)
            free(current_binary_code);
        if (current_binary_label != NULL)
            free(current_binary_label);
        if (current_binary_new_code != NULL)
            free(current_binary_new_code);
        if (first_phrase != NULL)
            free(first_phrase);
        if (second_phrase != NULL)
            free(second_phrase);
        if (third_phrase != NULL)
            free(third_phrase);
        if (fourth_phrase != NULL)
            free(fourth_phrase);
        if (new_ob != NULL)
            free(new_ob);
        if (new_ent != NULL)
            free(new_ent);
        if (new_ext != NULL)
            free(new_ext);
    }
}