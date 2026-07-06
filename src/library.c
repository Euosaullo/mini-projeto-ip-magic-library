/* Implements the main library operations for managing magic books. */

#include <stdio.h>
#include <stdlib.h>
#include "library.h"

static void clearInputBuffer(void);
static void removeLineBreak(char *str);
static void readText(const char *message, char str[], int size);
static void readValidDate(const char *message, Date *date);
static int isValidDate(int day, int month, int year);
static int findFreeSlot(MagicBook **library);
static int findBookIndexById(MagicBook **library, int id);
static int idAlreadyExists(MagicBook **library, int id);

void initializeLibrary(MagicBook **library)
{
    int i;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        library[i] = NULL;
    }
}

void registerBook(MagicBook **library)
{
    int slot;
    int id;
    int validInput;
    MagicBook *newBook;

    slot = findFreeSlot(library);

    if (slot == -1)
    {
        printf("The library is full.\n");
        return;
    }

    newBook = (MagicBook *) malloc(sizeof(MagicBook));

    if (newBook == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("\n--- Register New Book ---\n");

    do
    {
        printf("Book ID: ");
        validInput = scanf("%d", &id);
        clearInputBuffer();

        if (validInput != 1)
        {
            printf("Invalid ID. Try again.\n");
        }
        else if (idAlreadyExists(library, id))
        {
            printf("This ID already exists. Try another one.\n");
        }

    } while (validInput != 1 || idAlreadyExists(library, id));

    newBook->id = id;

    readText("Book title: ", newBook->title, TEXT_SIZE);
    readText("Author name: ", newBook->author.name, TEXT_SIZE);

    readValidDate("Author birth date (DD MM YYYY): ", &newBook->author.birthDate);
    readValidDate("Writing date (DD MM YYYY): ", &newBook->writingDate);

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

    index = findBookIndexById(library, id);

    if (index == -1)
    {
        printf("Book with ID %d was not found.\n", id);
        return;
    }

    book = library[index];

    printf("\n--- Update Book ID %d ---\n", id);

    readText("New book title: ", book->title, TEXT_SIZE);
    readText("New author name: ", book->author.name, TEXT_SIZE);

    readValidDate("New author birth date (DD MM YYYY): ", &book->author.birthDate);
    readValidDate("New writing date (DD MM YYYY): ", &book->writingDate);

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

static void clearInputBuffer(void)
{
    int character;

    do
    {
        character = getchar();
    } while (character != '\n' && character != EOF);
}

static void removeLineBreak(char *str)
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

static void readText(const char *message, char str[], int size)
{
    printf("%s", message);

    if (fgets(str, size, stdin) != NULL)
    {
        removeLineBreak(str);
    }
    else
    {
        str[0] = '\0';
    }
}

static void readValidDate(const char *message, Date *date)
{
    int validInput;

    do
    {
        printf("%s", message);

        validInput = scanf("%d %d %d", &date->day, &date->month, &date->year);
        clearInputBuffer();

        if (validInput != 3 || !isValidDate(date->day, date->month, date->year))
        {
            printf("Invalid date. Try again.\n");
        }

    } while (validInput != 3 || !isValidDate(date->day, date->month, date->year));
}

static int isValidDate(int day, int month, int year)
{
    int daysInMonth[13];

    daysInMonth[0] = 0;
    daysInMonth[1] = 31;
    daysInMonth[2] = 28;
    daysInMonth[3] = 31;
    daysInMonth[4] = 30;
    daysInMonth[5] = 31;
    daysInMonth[6] = 30;
    daysInMonth[7] = 31;
    daysInMonth[8] = 31;
    daysInMonth[9] = 30;
    daysInMonth[10] = 31;
    daysInMonth[11] = 30;
    daysInMonth[12] = 31;

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

static int findBookIndexById(MagicBook **library, int id)
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

static int idAlreadyExists(MagicBook **library, int id)
{
    if (findBookIndexById(library, id) != -1)
    {
        return 1;
    }

    return 0;
}
