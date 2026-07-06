/* Implements file persistence operations for saving and loading the library. */

#include <stdio.h>
#include <stdlib.h>
#include "files.h"
#include "encryption.h"
#include "library.h"
#include "structs.h"
#include "utils.h"

#define ENCRYPTED_TITLE_LINE_SIZE (TEXT_SIZE * 2 + 2)

static void writeEncryptedTitle(FILE *file, const char title[]);
static int readEncryptedTitle(FILE *file, char title[]);
static int decodeEncryptedTitle(const char source[], char destination[]);
static int hexDigitValue(char character);
static void writeBookAttributes(FILE *file, const BookAttributes *attributes);
static int readBookAttributesFromFile(FILE *file, BookAttributes *attributes);
static int isLoadedBookValid(MagicBook **library, const MagicBook *book);
static int isBookAttributesValid(const BookAttributes *attributes);
static int isOptionalAttributeValid(int hasAttribute, int value);

void saveLibraryToFile(MagicBook **library, const char *fileName)
{
    FILE *file;
    int i;
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

            fprintf(file, "%d\n", book->id);
            writeEncryptedTitle(file, book->title);
            fprintf(file, "%s\n", book->author.name);

            fprintf(file, "%d %d %d\n",
                    book->author.birthDate.day,
                    book->author.birthDate.month,
                    book->author.birthDate.year);

            fprintf(file, "%d %d %d\n",
                    book->writingDate.day,
                    book->writingDate.month,
                    book->writingDate.year);

            writeBookAttributes(file, &book->attributes);
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
        slot = findFreeLibrarySlot(library);

        if (slot == -1)
        {
            break;
        }

        newBook = malloc(sizeof *newBook);

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

        if (!readEncryptedTitle(file, newBook->title))
        {
            free(newBook);
            break;
        }

        if (fgets(newBook->author.name, TEXT_SIZE, file) == NULL)
        {
            free(newBook);
            break;
        }

        if (!removeLineBreak(newBook->author.name))
        {
            discardRestOfLine(file);
        }

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

        if (!readBookAttributesFromFile(file, &newBook->attributes))
        {
            free(newBook);
            break;
        }

        if (!isLoadedBookValid(library, newBook))
        {
            printf("Skipping invalid or duplicated book record with ID %d.\n", newBook->id);
            free(newBook);
            continue;
        }

        library[slot] = newBook;
    }

    fclose(file);
}

static void writeEncryptedTitle(FILE *file, const char title[])
{
    char encryptedTitle[TEXT_SIZE];
    int i;

    copyText(encryptedTitle, title, TEXT_SIZE);
    encryptString(encryptedTitle);

    for (i = 0; encryptedTitle[i] != '\0'; i++)
    {
        fprintf(file, "%02X", (unsigned char) encryptedTitle[i]);
    }

    fprintf(file, "\n");
}

static int readEncryptedTitle(FILE *file, char title[])
{
    char encryptedTitleLine[ENCRYPTED_TITLE_LINE_SIZE];

    if (fgets(encryptedTitleLine, ENCRYPTED_TITLE_LINE_SIZE, file) == NULL)
    {
        return 0;
    }

    if (!removeLineBreak(encryptedTitleLine))
    {
        discardRestOfLine(file);
        return 0;
    }

    return decodeEncryptedTitle(encryptedTitleLine, title);
}

static int decodeEncryptedTitle(const char source[], char destination[])
{
    int i;
    int outputIndex;
    int high;
    int low;

    i = 0;
    outputIndex = 0;

    while (source[i] != '\0')
    {
        if (source[i + 1] == '\0' || outputIndex >= TEXT_SIZE - 1)
        {
            return 0;
        }

        high = hexDigitValue(source[i]);
        low = hexDigitValue(source[i + 1]);

        if (high == -1 || low == -1)
        {
            return 0;
        }

        destination[outputIndex] = (char) ((high * 16) + low);
        outputIndex++;
        i += 2;
    }

    destination[outputIndex] = '\0';
    decryptString(destination);

    return 1;
}

static int hexDigitValue(char character)
{
    if (character >= '0' && character <= '9')
    {
        return character - '0';
    }

    if (character >= 'A' && character <= 'F')
    {
        return character - 'A' + 10;
    }

    if (character >= 'a' && character <= 'f')
    {
        return character - 'a' + 10;
    }

    return -1;
}

static void writeBookAttributes(FILE *file, const BookAttributes *attributes)
{
    fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
            attributes->hasStrength,
            attributes->strength,
            attributes->hasDexterity,
            attributes->dexterity,
            attributes->hasConstitution,
            attributes->constitution,
            attributes->hasIntelligence,
            attributes->intelligence,
            attributes->hasWisdom,
            attributes->wisdom,
            attributes->hasCharisma,
            attributes->charisma,
            attributes->hasMagic,
            attributes->magic);
}

static int readBookAttributesFromFile(FILE *file, BookAttributes *attributes)
{
    int result;

    result = fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                    &attributes->hasStrength,
                    &attributes->strength,
                    &attributes->hasDexterity,
                    &attributes->dexterity,
                    &attributes->hasConstitution,
                    &attributes->constitution,
                    &attributes->hasIntelligence,
                    &attributes->intelligence,
                    &attributes->hasWisdom,
                    &attributes->wisdom,
                    &attributes->hasCharisma,
                    &attributes->charisma,
                    &attributes->hasMagic,
                    &attributes->magic);

    if (result != 14)
    {
        return 0;
    }

    discardRestOfLine(file);

    return 1;
}

static int isLoadedBookValid(MagicBook **library, const MagicBook *book)
{
    if (book->id <= 0 || bookIdExists(library, book->id))
    {
        return 0;
    }

    if (book->title[0] == '\0' || book->author.name[0] == '\0')
    {
        return 0;
    }

    if (!isValidDate(book->author.birthDate.day,
                     book->author.birthDate.month,
                     book->author.birthDate.year))
    {
        return 0;
    }

    if (!isValidDate(book->writingDate.day,
                     book->writingDate.month,
                     book->writingDate.year))
    {
        return 0;
    }

    if (!isBookAttributesValid(&book->attributes))
    {
        return 0;
    }

    return 1;
}

static int isBookAttributesValid(const BookAttributes *attributes)
{
    if (!isOptionalAttributeValid(attributes->hasStrength, attributes->strength))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasDexterity, attributes->dexterity))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasConstitution, attributes->constitution))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasIntelligence, attributes->intelligence))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasWisdom, attributes->wisdom))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasCharisma, attributes->charisma))
    {
        return 0;
    }

    if (!isOptionalAttributeValid(attributes->hasMagic, attributes->magic))
    {
        return 0;
    }

    return 1;
}

static int isOptionalAttributeValid(int hasAttribute, int value)
{
    if (hasAttribute != 0 && hasAttribute != 1)
    {
        return 0;
    }

    if (!hasAttribute)
    {
        return value == 0;
    }

    return value >= ATTRIBUTE_MIN && value <= ATTRIBUTE_MAX;
}
