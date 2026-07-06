/* Implements the main library operations for managing magic books. */

#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "utils.h"

static int readRequiredText(const char *message, char str[], int size);
static int readValidDate(const char *message, Date *date);

void initializeLibrary(MagicBook **library)
{
    int i;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        library[i] = NULL;
    }
}

void freeLibrary(MagicBook **library)
{
    int i;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL)
        {
            free(library[i]);
            library[i] = NULL;
        }
    }
}

void registerBook(MagicBook **library)
{
    int slot;
    int id;
    int inputResult;
    int validId;
    MagicBook *newBook;

    slot = findFreeLibrarySlot(library);

    if (slot == -1)
    {
        printf("The library is full.\n");
        return;
    }

    newBook = malloc(sizeof *newBook);

    if (newBook == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n--- Register New Book ---\n");

    validId = 0;

    while (!validId)
    {
        printf("Book ID: ");

        inputResult = scanf("%d", &id);

        if (inputResult == EOF)
        {
            clearInputBuffer();
            free(newBook);
            printf("Book registration canceled.\n");
            return;
        }

        if (inputResult != 1)
        {
            printf("Invalid ID. Try again.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (id <= 0)
        {
            printf("The ID must be a positive number.\n");
        }
        else if (bookIdExists(library, id))
        {
            printf("This ID already exists. Try another one.\n");
        }
        else
        {
            validId = 1;
        }

    }

    newBook->id = id;

    if (!readRequiredText("Book title: ", newBook->title, TEXT_SIZE) ||
        !readRequiredText("Author name: ", newBook->author.name, TEXT_SIZE) ||
        !readValidDate("Author birth date (DD MM YYYY): ", &newBook->author.birthDate) ||
        !readValidDate("Writing date (DD MM YYYY): ", &newBook->writingDate))
    {
        free(newBook);
        printf("Book registration canceled.\n");
        return;
    }

    library[slot] = newBook;

    printf("Book registered successfully.\n");
}

void deleteBookById(MagicBook **library, int id)
{
    int index;

    index = findBookIndexById(library, id);

    if (index == -1)
    {
        printf("Book with ID %d was not found.\n", id);
        return;
    }

    free(library[index]);
    library[index] = NULL;

    printf("Book deleted successfully.\n");
}

void displayBookById(MagicBook **library, int id)
{
    int index;
    MagicBook *book;

    index = findBookIndexById(library, id);

    if (index == -1)
    {
        printf("Book with ID %d was not found.\n", id);
        return;
    }

    book = library[index];

    printf("\n--- Book Details ---\n");
    printf("ID: %d\n", book->id);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author.name);

    printf("Author birth date: %02d/%02d/%04d\n",
           book->author.birthDate.day,
           book->author.birthDate.month,
           book->author.birthDate.year);

    printf("Writing date: %02d/%02d/%04d\n",
           book->writingDate.day,
           book->writingDate.month,
           book->writingDate.year);
}

void updateBookById(MagicBook **library, int id)
{
    int index;
    MagicBook *book;
    char newTitle[TEXT_SIZE];
    char newAuthorName[TEXT_SIZE];
    Date newAuthorBirthDate;
    Date newWritingDate;

    index = findBookIndexById(library, id);

    if (index == -1)
    {
        printf("Book with ID %d was not found.\n", id);
        return;
    }

    book = library[index];

    printf("\n--- Update Book ID %d ---\n", id);

    if (!readRequiredText("New book title: ", newTitle, TEXT_SIZE) ||
        !readRequiredText("New author name: ", newAuthorName, TEXT_SIZE) ||
        !readValidDate("New author birth date (DD MM YYYY): ", &newAuthorBirthDate) ||
        !readValidDate("New writing date (DD MM YYYY): ", &newWritingDate))
    {
        printf("Book update canceled.\n");
        return;
    }

    copyText(book->title, newTitle, TEXT_SIZE);
    copyText(book->author.name, newAuthorName, TEXT_SIZE);
    book->author.birthDate = newAuthorBirthDate;
    book->writingDate = newWritingDate;

    printf("Book updated successfully.\n");
}

void listBookTitles(MagicBook **library)
{
    int i;
    int count;

    count = 0;

    printf("\n--- Book Titles ---\n");

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL)
        {
            printf("ID: %d | Title: %s\n", library[i]->id, library[i]->title);
            count++;
        }
    }

    if (count == 0)
    {
        printf("The library is empty.\n");
    }
}

static int readRequiredText(const char *message, char str[], int size)
{
    do
    {
        if (!readLine(message, str, size))
        {
            return 0;
        }

        if (str[0] == '\0')
        {
            printf("This field cannot be empty.\n");
        }

    } while (str[0] == '\0');

    return 1;
}

static int readValidDate(const char *message, Date *date)
{
    int validInput;

    do
    {
        printf("%s", message);

        validInput = scanf("%d %d %d", &date->day, &date->month, &date->year);

        if (validInput == EOF)
        {
            clearInputBuffer();
            return 0;
        }

        clearInputBuffer();

        if (validInput != 3 || !isValidDate(date->day, date->month, date->year))
        {
            printf("Invalid date. Try again.\n");
        }

    } while (validInput != 3 || !isValidDate(date->day, date->month, date->year));

    return 1;
}

int findFreeLibrarySlot(MagicBook **library)
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

int findBookIndexById(MagicBook **library, int id)
{
    int i;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL && library[i]->id == id)
        {
            return i;
        }
    }

    return -1;
}

int bookIdExists(MagicBook **library, int id)
{
    if (findBookIndexById(library, id) != -1)
    {
        return 1;
    }

    return 0;
}
