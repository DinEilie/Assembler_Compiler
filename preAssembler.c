/*
 * Title: Maman 14 - Assembler Project
 * Description: preAssembler.c is responsible for the macro reading and spreading. When finnished it will create the ".am" file
 *              and return its name. This algorithm follows the "phases" of the algorithm that has been suggested in the course book (algorithm shildi shel Hoveret course).
 * Author: Din Eilie
 * Student ID: 208679597
 */
#include "header.h"

/* Searches for permutation (print them if found), return 1 if found else return 0 */
char *scanMacros(char source[])
{
    FILE *fpRead;
    FILE *fpWrite;
    my_macro *head_macro;
    my_macro *current_macro;
    my_macro *search_result;
    my_line *head_file_lines;
    char *current_line;
    char *current_ptr;
    char *line_ptr;
    char *new_file_name;
    char *first_phrase;
    char *second_phrase;
    int fileContainsLines = 0;
    int mcr_flag = 0;
    int lineIndex = 1;
    int iCount = 0;
    int isMacroLast = 0;
    new_file_name = generateAmName(source);
    head_macro = NULL;
    current_macro = NULL;
    search_result = NULL;
    head_file_lines = NULL;
    current_line = NULL;
    current_ptr = NULL;
    line_ptr = NULL;
    first_phrase = NULL;
    second_phrase = NULL;
    fpRead = fopen(generateAsName(source), "r");

    if (fpRead == NULL)
    {
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!>                      INTERNAL ERROR                            <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "    Unable to open the file: %s   \n", source);
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
    }
    else
    {
        current_line = (char *)malloc(SIZE + 2);
        if (fgets(current_line, SIZE + 2, fpRead) != NULL)
            fileContainsLines = 1;

        /* Phase 1 */
        while (fileContainsLines)
        {
            /* First word in the current line */
            first_phrase = firstMacroString(current_line);

            /* Phase 3 */
            if (isStringEmpty(current_line) == 0 && strcmp(first_phrase, "mcr") == 0)
            {
                if (strlen(strstr(current_line, "mcr")) - 3 > 0)
                {
                    second_phrase = firstMacroString(strstr(current_line, "mcr") + 3);
                    if (isStringEmpty(strstr(current_line, "mcr") + 3) == 0 && isValidName(second_phrase))
                    {
                        /* Turn on mcr_flag and start recording the next lines for macro */
                        if (isStringEmpty(strstr(current_line, second_phrase) + strlen(second_phrase)))
                        {
                            mcr_flag = 1;
                            current_macro = newMacro(second_phrase, NULL);
                        }
                        else
                        {
                            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                            fprintf(stdout, "                      WARNING AT LINE %d\n", lineIndex);
                            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                            fprintf(stdout, "<!> Only one name is possible for macro definition. Therefore,     <!>\n");
                            fprintf(stdout, "<!> this line will not be considered as a new macro.               <!>\n");
                            fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
                            addNewNonMacroLine(&head_file_lines, current_line);
                        }
                    }
                    else if (isStringEmpty(strstr(current_line, "mcr") + 3) == 0)
                    {
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "                      WARNING AT LINE %d\n", lineIndex);
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "<!> The macro name is a reserved assembler word. Only valid names  <!>\n");
                        fprintf(stdout, "<!> are possible for macro definition. Therefore, this line will   <!>\n");
                        fprintf(stdout, "<!> not be considered as a new macro.                              <!>\n");
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
                        addNewNonMacroLine(&head_file_lines, current_line);
                    }
                    else
                    {
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "                      WARNING AT LINE %d\n", lineIndex);
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                        fprintf(stdout, "<!> No macro name is declared. Therefore, this line will not be    <!>\n");
                        fprintf(stdout, "<!> considered as a new macro.                                     <!>\n");
                        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
                        addNewNonMacroLine(&head_file_lines, current_line);
                    }
                }
                else
                {
                    fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                    fprintf(stdout, "                      WARNING AT LINE %d\n", lineIndex);
                    fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
                    fprintf(stdout, "<!> No macro name is declared. Therefore, this line will not be    <!>\n");
                    fprintf(stdout, "<!> considered as a new macro.                                     <!>\n");
                    fprintf(stdout, "<!>----------------------------------------------------------------<!>\n\n\n");
                    addNewNonMacroLine(&head_file_lines, current_line);
                }
            }
            /* Phase 6 */
            else if (mcr_flag == 1 && isStringEmpty(current_line) == 0 && strcmp(first_phrase, "endmcr") != 0)
            {
                /* Add the current line to the current macro */
                addNewLineToMacro(current_macro, current_line);
            }
            else if (mcr_flag == 1 && isStringEmpty(current_line) == 0 && strcmp(first_phrase, "endmcr") == 0)
            {
                /* Turn off mcr_flag and stop recording the next lines for macro */
                if (isStringEmpty(strstr(current_line, "endmcr") + 6))
                {
                    mcr_flag = 0;
                    addNewMacroToList(&head_macro, current_macro);
                    current_macro = NULL;
                }
                else
                {
                    fprintf(stdout, "<!>---------------- Warning at line %d ---------------<!>\n", lineIndex);
                    fprintf(stdout, "<!> words that come after a macro-end declaration    <!>\n");
                    fprintf(stdout, "<!> will disqualify the ending of a macro.           <!>\n");
                    fprintf(stdout, "<!> Therefore, this line will not be considered as   <!>\n");
                    fprintf(stdout, "<!> an ending for the current macro.                 <!>\n");
                    fprintf(stdout, "<!>---------------------------------------------------<!>\n");
                    addNewLineToMacro(current_macro, current_line);
                }
            }
            /* Search the current line for any macros to spread */
            else
            {
                /* Release first_phrase and reset current_ptr */
                if (first_phrase != NULL)
                    free(first_phrase);
                first_phrase = NULL;
                current_ptr = current_line;

                /* Continue to search for macros in the line */
                while (isStringEmpty(current_ptr) == 0)
                {
                    while (isWhitespace(*(current_ptr + iCount)))
                        iCount++;

                    /* Save the first_phrase */
                    first_phrase = firstMacroString((current_ptr + iCount));

                    /* The first_phrase is a macro  */
                    search_result = searchMacro(head_macro, first_phrase);
                    if (search_result != NULL)
                    {
                        /* Save all the text until the beginning of the macro */
                        line_ptr = (char *)malloc(iCount + 1);
                        memcpy(line_ptr, current_ptr, iCount);
                        *(line_ptr + iCount) = '\0';
                        addNewNonMacroLine(&head_file_lines, line_ptr);

                        /* Add all current macro lines */
                        replaceLineWithMacro(&head_file_lines, search_result);

                        /* Update the current pointer */
                        current_ptr = current_ptr + iCount + strlen(first_phrase);
                        iCount = 0;

                        /* Free irrelevant values from memory */
                        if (first_phrase != NULL)
                            free(first_phrase);
                        if (line_ptr != NULL)
                            free(line_ptr);
                        line_ptr = NULL;
                        first_phrase = NULL;

                        /* This macro is the latest addition to the file */
                        isMacroLast = 1;
                    }
                    else
                    {
                        iCount = iCount + strlen(first_phrase);

                        /* Save all the text until the beginning of the macro */
                        line_ptr = (char *)malloc(iCount + 1);
                        memcpy(line_ptr, current_ptr, iCount);
                        *(line_ptr + iCount) = '\0';
                        addNewNonMacroLine(&head_file_lines, line_ptr);

                        /* Update the current pointer */
                        current_ptr = current_ptr + iCount;
                        iCount = 0;

                        /* Free irrelevant values from memory */
                        if (first_phrase != NULL)
                            free(first_phrase);
                        if (line_ptr != NULL)
                            free(line_ptr);
                        line_ptr = NULL;
                        first_phrase = NULL;

                        /* There is no macro as the latest addition to the file */
                        isMacroLast = 0;
                    }
                }
                if (isMacroLast == 0)
                    addNewNonMacroLine(&head_file_lines, current_ptr);
            }

            if (fgets(current_line, SIZE + 2, fpRead) == NULL)
                fileContainsLines = 0;
            else
                lineIndex++;

            if (first_phrase != NULL)
                free(first_phrase);
            if (second_phrase != NULL)
                free(second_phrase);
            first_phrase = NULL;
            second_phrase = NULL;
        }

        /* Phase 9 */
        /* Save new file */
        fclose(fpRead);
        fpWrite = fopen(new_file_name, "w+");
        printLinesToFile(head_file_lines, fpWrite);
        fclose(fpWrite);

        /* Free memory blocks */
        freeMacros(&head_macro);
        freeFileLines(&head_file_lines);
        free(first_phrase);
        free(second_phrase);
        free(current_line);
        first_phrase = NULL;
        second_phrase = NULL;
        current_line = NULL;
        return new_file_name;
    }
    return NULL;
}