/* Implements file persistence operations for saving and loading the library. */

#include <stdio.h>
#include <stdlib.h>
#include "files.h"
#include "encryption.h"
#include "structs.h"

static void copyText(char destination[], const char source[]);
static void removeLineBreak(char str[]);
static void discardRestOfLine(FILE *file);
static int findFreeSlot(MagicBook **library);

void saveLibraryToFile(MagicBook **library, const char *fileName)
{
    FILE *file;
    int i;
    char encryptedTitle[TEXT_SIZE];
    MagicBook *book;

    file = fopen(fileName, "w");

    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL)
        {
            book = library[i];

            copyText(encryptedTitle, book->title);
            encryptString(encryptedTitle);

            fprintf(file, "%d\n", book->id);
            fprintf(file, "%s\n", encryptedTitle);
            fprintf(file, "%s\n", book->author.name);

            fprintf(file, "%d %d %d\n",
                    book->author.birthDate.day,
                    book->author.birthDate.month,
                    book->author.birthDate.year);

            fprintf(file, "%d %d %d\n",
                    book->writingDate.day,
                    book->writingDate.month,
                    book->writingDate.year);
        }
    }

    fclose(file);

    printf("Library saved successfully.\n");
}

void loadLibraryFromFile(MagicBook **library, const char *fileName)
{
    FILE *file;
    MagicBook *newBook;
    int slot;
    int result;

    file = fopen(fileName, "r");

    if (file == NULL)
    {
        return;
    }

    while (1)
    {
        slot = findFreeSlot(library);

        if (slot == -1)
        {
            break;
        }

        newBook = (MagicBook *) malloc(sizeof(MagicBook));

        if (newBook == NULL)
        {
            printf("Memory allocation failed while loading file.\n");
            break;
        }

        result = fscanf(file, "%d", &newBook->id);

        if (result != 1)
        {
            free(newBook);
            break;
        }

        discardRestOfLine(file);

        if (fgets(newBook->title, TEXT_SIZE, file) == NULL)
        {
            free(newBook);
            break;
        }

        removeLineBreak(newBook->title);
        decryptString(newBook->title);

        if (fgets(newBook->author.name, TEXT_SIZE, file) == NULL)
        {
            free(newBook);
            break;
        }

        removeLineBreak(newBook->author.name);

        result = fscanf(file, "%d %d %d",
                        &newBook->author.birthDate.day,
                        &newBook->author.birthDate.month,
                        &newBook->author.birthDate.year);

        if (result != 3)
        {
            free(newBook);
            break;
        }

        discardRestOfLine(file);

        result = fscanf(file, "%d %d %d",
                        &newBook->writingDate.day,
                        &newBook->writingDate.month,
                        &newBook->writingDate.year);

        if (result != 3)
        {
            free(newBook);
            break;
        }

        discardRestOfLine(file);

        library[slot] = newBook;
    }

    fclose(file);
}

static void copyText(char destination[], const char source[])
{
    int i;

    i = 0;

    while (source[i] != '\0' && i < TEXT_SIZE - 1)
    {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0';
}

static void removeLineBreak(char str[])
{
    int i;

    i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            return;
        }

        i++;
    }
}

static void discardRestOfLine(FILE *file)
{
    int character;

    do
    {
        character = fgetc(file);
    } while (character != '\n' && character != EOF);
}

static int findFreeSlot(MagicBook **library)
{
    int i;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}
