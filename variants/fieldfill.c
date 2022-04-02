#include <stdio.h>
#include <stdlib.h>
void testFieldFill(char field[], size_t size)
{
    for (int i = 0; i < size; i++)
    {
        field[i] = i;
    }
}

void testFieldFillAsky(char field[])
{
    char *arr = "⠀⠀⠀⠀⠀⠀⠀⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠤⠤⠤⠤⠤⠤⠤⠤⠤⣤⠀⠀⠀⠀⠀⠀⠀"
                "⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀"
                "⠀⠀⠀⠀⠀⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⣤⣤⣤⣤⣤⣤⣤⣤⣿⣤⠀⠀⠀⠀⠀"
                "⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀"
                "⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀"
                "⠀⠀⣤⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣿⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⣤⠀⠀"
                "⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀"
                "⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⣤"
                "⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛⠛";
    for (int i = 0; i < sizeof(arr) / sizeof(char); i++)
        field[i] = arr[i];
}

void testFieldFillFromFile(char field[], char *fileName)
{
    FILE *fp;
    wchar_t *buffer;

    if ((buffer = malloc(sizeof(wchar_t))) == NULL)
    {
        printf("Error: No enough memory\n");
        getchar();
        return;
    }
    else
    {
        fp = fopen(fileName, "r");

        if (fp == NULL)
        {
            printf("Error: File cannot be opened\n");
            getchar();
            return;
        }
        else
        {
            int i = 0;
            while (fread(buffer, sizeof(wchar_t), 1, fp) > 0)
            {
                field[i++] = *buffer;
                printf("%s", buffer);
            }
        }
        fclose(fp);
    }
}