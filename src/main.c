/* Controls the main execution flow and menu of the Magic Library system. */

#include <stdio.h>
#include "structs.h"
#include "library.h"
#include "files.h"
#include "utils.h"

static int readMenuOption(void);

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
        option = readMenuOption();

        switch (option)
        {
            case 1:
                registerBook(library);
                break;

            case 2:
                printf("Enter the ID of the book to delete: ");

                if (scanf("%d", &id) == 1)
                {
                    clearInputBuffer();
                    deleteBookById(library, id);
                }
                else
                {
                    clearInputBuffer();
                    printf("Invalid ID.\n");
                }

                break;

            case 3:
                printf("Enter the ID of the book to display: ");

                if (scanf("%d", &id) == 1)
                {
                    clearInputBuffer();
                    displayBookById(library, id);
                }
                else
                {
                    clearInputBuffer();
                    printf("Invalid ID.\n");
                }

                break;

            case 4:
                printf("Enter the ID of the book to update: ");

                if (scanf("%d", &id) == 1)
                {
                    clearInputBuffer();
                    updateBookById(library, id);
                }
                else
                {
                    clearInputBuffer();
                    printf("Invalid ID.\n");
                }

                break;

            case 5:
                listBookTitles(library);
                break;

            case 6:
                saveLibraryToFile(library, argv[1]);
                freeLibrary(library);
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid option.\n");
        }

    } while (option != 6);

    return 0;
}

static int readMenuOption(void)
{
    int option;
    int result;

    printf("\n--- Magic Library Menu ---\n");
    printf("1. Register book\n");
    printf("2. Delete book\n");
    printf("3. Display book\n");
    printf("4. Update book\n");
    printf("5. List book titles\n");
    printf("6. Exit\n");
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

