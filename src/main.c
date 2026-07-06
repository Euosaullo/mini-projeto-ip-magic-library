/* Controls the main execution flow and menu of the Magic Library system. */

#include <stdio.h>
#include "structs.h"
#include "library.h"
#include "files.h"
#include "utils.h"

static int readMenuOption(MagicBook **library);
static int readBookId(const char *message, int *id);
static int countRegisteredBooks(MagicBook **library);
static void printMenuLine(const char *text);
static void printMenuInfoLine(const char *label, const char *value);
static void printMenuBorder(char character);

int main(int argc, char *argv[])
{
    MagicBook *library[LIBRARY_SIZE];
    int option;
    int id;

    if (argc < 2)
    {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }

    initializeLibrary(library);
    loadLibraryFromFile(library, argv[1]);

    do
    {
        option = readMenuOption(library);

        switch (option)
        {
            case 1:
                registerBook(library);
                break;

            case 2:
                printf("\n+------------------- DELETE BOOK --------------------+\n\n");

                if (readBookId("Enter the ID of the book to delete: ", &id))
                {
                    deleteBookById(library, id);
                }
                else
                {
                    printf("Invalid ID.\n");
                }

                break;

            case 3:
                printf("\n+------------------- DISPLAY BOOK -------------------+\n\n");

                if (readBookId("Enter the ID of the book to display: ", &id))
                {
                    displayBookById(library, id);
                }
                else
                {
                    printf("Invalid ID.\n");
                }

                break;

            case 4:
                printf("\n+------------------- UPDATE BOOK --------------------+\n\n");

                if (readBookId("Enter the ID of the book to update: ", &id))
                {
                    updateBookById(library, id);
                }
                else
                {
                    printf("Invalid ID.\n");
                }

                break;

            case 5:
                listBookTitles(library);
                break;

            case 6:
                printf("\n+--------------------- EXITING ----------------------+\n\n");
                saveLibraryToFile(library, argv[1]);
                freeLibrary(library);
                printf("Exiting Magic Library...\n");
                break;

            default:
                printf("Invalid option.\n");
        }

    } while (option != 6);

    return 0;
}

static int readMenuOption(MagicBook **library)
{
    int option;
    int result;
    char countText[32];

    snprintf(countText, sizeof(countText), "%03d/%03d", countRegisteredBooks(library), LIBRARY_SIZE);

    printf("\n");
    printMenuBorder('=');
    printMenuLine("MAGIC LIBRARY");
    printMenuLine("Enchanted Book Inventory");
    printMenuBorder('=');
    printMenuInfoLine("Registered books", countText);
    printMenuBorder('-');
    printMenuLine("[1] Register book");
    printMenuLine("[2] Delete book");
    printMenuLine("[3] Display book");
    printMenuLine("[4] Update book");
    printMenuLine("[5] List book titles");
    printMenuLine("[6] Save and exit");
    printMenuBorder('=');
    printf("Choose an option: ");

    result = scanf("%d", &option);

    if (result == EOF)
    {
        printf("\nInput ended.\n");
        return 6;
    }

    clearInputBuffer();

    if (result != 1)
    {
        return -1;
    }

    return option;
}

static int readBookId(const char *message, int *id)
{
    int result;

    printf("%s", message);

    result = scanf("%d", id);
    clearInputBuffer();

    return result == 1;
}

static int countRegisteredBooks(MagicBook **library)
{
    int i;
    int count;

    count = 0;

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL)
        {
            count++;
        }
    }

    return count;
}

static void printMenuLine(const char *text)
{
    printf("| %-50.50s |\n", text);
}

static void printMenuInfoLine(const char *label, const char *value)
{
    char line[128];

    snprintf(line, sizeof(line), "%s: %s", label, value);
    printMenuLine(line);
}

static void printMenuBorder(char character)
{
    int i;

    printf("+");

    for (i = 0; i < 52; i++)
    {
        printf("%c", character);
    }

    printf("+\n");
}
