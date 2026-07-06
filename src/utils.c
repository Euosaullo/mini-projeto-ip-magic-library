/* Implements shared utility functions used by multiple modules. */

#include <stdio.h>
#include "utils.h"

void clearInputBuffer(void)
{
    int character;

    do
    {
        character = getchar();
    } while (character != '\n' && character != EOF);
}

int removeLineBreak(char str[])
{
    int i;

    i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            return 1;
        }

        i++;
    }

    return 0;
}

void discardRestOfLine(FILE *file)
{
    int character;

    do
    {
        character = fgetc(file);
    } while (character != '\n' && character != EOF);
}

int readLine(const char *message, char str[], int size)
{
    if (message != NULL)
    {
        printf("%s", message);
    }

    if (fgets(str, size, stdin) == NULL)
    {
        str[0] = '\0';
        return 0;
    }

    if (!removeLineBreak(str))
    {
        clearInputBuffer();
    }

    return 1;
}

void copyText(char destination[], const char source[], int size)
{
    int i;

    i = 0;

    while (source[i] != '\0' && i < size - 1)
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';
}

int isValidDate(int day, int month, int year)
{
    int daysInMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (year < 1 || month < 1 || month > 12 || day < 1)
    {
        return 0;
    }

    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month])
    {
        return 0;
    }

    return 1;
}
