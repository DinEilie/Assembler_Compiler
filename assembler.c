/*
 * Title: Maman 14 - Assembler Project
 * Description: This program will compute and compile the given files into their unique binary form.
 * Author: Din Eilie
 * Student ID: 208679597
 */
#include "header.h"

int main(int argc, char const *argv[])
{
    char *file_name;
    char *file_name_am;
    char *file_name_as;
    int i;
    int count;
    count = argc - 1;
    file_name = NULL;
    file_name_am = NULL;
    file_name_as = NULL;

    if (count > 0)
    {
        for (i = 0; i < count; i++)
        {
            /* Get the full file name with ".as" at the end and scan for macros*/
            file_name = (char *)malloc(strlen(*(argv + 1 + i)) + 1);
            memcpy(file_name, *(argv + 1 + i), strlen(*(argv + 1 + i)));
            *(file_name + strlen(*(argv + 1 + i))) = '\0';
            file_name_as = generateAsName(file_name);
            if(file_name == NULL)
            {
        			/* Error: memory allocation has failed*/
        			printError(-1, 0);
        			exit(0);
   			}

            fprintf(stdout, "<$>****************************************************************<$>\n");
            fprintf(stdout, "               STARTING PRE-ASSEMBLER FOR: %s\n", file_name_as);
            fprintf(stdout, "<$>****************************************************************<$>\n");
            file_name_am = scanMacros(file_name);

            /* Activate the assembler construction on the new file */
            if (file_name_am != NULL)
            {
                fprintf(stdout, "<$>****************************************************************<$>\n");
                fprintf(stdout, "               STARTING ASSEMBLER FOR: %s\n", file_name_am);
                fprintf(stdout, "<$>****************************************************************<$>\n");
                assemblerConstruction(file_name_am);
            }

            /* Free irrelevant values from memory */
            if (file_name != NULL)
                free(file_name);
            if (file_name_am != NULL)
                free(file_name_am);
            if (file_name_as != NULL)
                free(file_name_as);
            file_name_as = NULL;
            file_name_am = NULL;
            file_name = NULL;
        }
    }
    else
    {
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
        fprintf(stdout, "<!>                      NO FILES WERE MENTIONED                   <!>\n");
        fprintf(stdout, "<!>----------------------------------------------------------------<!>\n");
    }
    return 0;
}
