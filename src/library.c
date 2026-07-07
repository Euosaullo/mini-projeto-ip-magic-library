/* Implements the main library operations for managing magic books. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"
#include "utils.h"

static int readRequiredText(const char *message, char str[], int size);
static int readValidDate(const char *message, Date *date);
static int readBookAttributes(BookAttributes *attributes);
static void initializeBookAttributes(BookAttributes *attributes);
static int readOptionalAttribute(const char *question, const char *valueMessage, int *hasAttribute, int *attributeValue);
static int editBookAttributes(BookAttributes *attributes, int *changed);
static int editOptionalAttribute(const char *editQuestion,
                                 const char *presenceQuestion,
                                 const char *valueMessage,
                                 int *hasAttribute,
                                 int *attributeValue,
                                 int *changed);
static int askYesOrNo(const char *message, int *answer);
static int readAttributeValue(const char *message, int *value);
static void displayBookAttributes(const BookAttributes *attributes);
static void displayBookPowerProfile(const BookAttributes *attributes);
static void includeAttributeInPowerProfile(int hasAttribute,
                                           int value,
                                           const char *attributeName,
                                           const char *archetype,
                                           int *activeCount,
                                           int *totalValue,
                                           int *dominantValue,
                                           const char **dominantAttribute,
                                           const char **bookArchetype);
static int calculatePowerLevel(double average);
static const char *getPowerRank(int powerLevel);
static int hasAnyBookAttribute(const BookAttributes *attributes);
static void printSectionHeader(const char *title);

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

    printSectionHeader("REGISTER BOOK");

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
        !readValidDate("Writing date (DD MM YYYY): ", &newBook->writingDate) ||
        !readBookAttributes(&newBook->attributes))
    {
        free(newBook);
        printf("Book registration canceled.\n");
        return;
    }

    library[slot] = newBook;

    printf("\nBook registered successfully.\n");
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

    printf("\nBook deleted successfully.\n");
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

    printSectionHeader("BOOK DETAILS");
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

    displayBookAttributes(&book->attributes);
    displayBookPowerProfile(&book->attributes);
}

void updateBookById(MagicBook **library, int id)
{
    int index;
    int answer;
    int changed;
    MagicBook *book;
    char newTitle[TEXT_SIZE];
    char newAuthorName[TEXT_SIZE];
    Date newAuthorBirthDate;
    Date newWritingDate;
    BookAttributes newAttributes;

    index = findBookIndexById(library, id);

    if (index == -1)
    {
        printf("Book with ID %d was not found.\n", id);
        return;
    }

    book = library[index];

    printf("\nSelected ID: %d\n\n", id);

    changed = 0;
    copyText(newTitle, book->title, TEXT_SIZE);
    copyText(newAuthorName, book->author.name, TEXT_SIZE);
    newAuthorBirthDate = book->author.birthDate;
    newWritingDate = book->writingDate;
    newAttributes = book->attributes;

    if (!askYesOrNo("Update book title? (1 yes / 0 no): ", &answer))
    {
        printf("Book update canceled.\n");
        return;
    }

    if (answer)
    {
        if (!readRequiredText("New book title: ", newTitle, TEXT_SIZE))
        {
            printf("Book update canceled.\n");
            return;
        }

        changed = 1;
    }

    if (!askYesOrNo("Update author name? (1 yes / 0 no): ", &answer))
    {
        printf("Book update canceled.\n");
        return;
    }

    if (answer)
    {
        if (!readRequiredText("New author name: ", newAuthorName, TEXT_SIZE))
        {
            printf("Book update canceled.\n");
            return;
        }

        changed = 1;
    }

    if (!askYesOrNo("Update author birth date? (1 yes / 0 no): ", &answer))
    {
        printf("Book update canceled.\n");
        return;
    }

    if (answer)
    {
        if (!readValidDate("New author birth date (DD MM YYYY): ", &newAuthorBirthDate))
        {
            printf("Book update canceled.\n");
            return;
        }

        changed = 1;
    }

    if (!askYesOrNo("Update writing date? (1 yes / 0 no): ", &answer))
    {
        printf("Book update canceled.\n");
        return;
    }

    if (answer)
    {
        if (!readValidDate("New writing date (DD MM YYYY): ", &newWritingDate))
        {
            printf("Book update canceled.\n");
            return;
        }

        changed = 1;
    }

    if (!editBookAttributes(&newAttributes, &changed))
    {
        printf("Book update canceled.\n");
        return;
    }

    if (!changed)
    {
        printf("No changes made.\n");
        return;
    }

    copyText(book->title, newTitle, TEXT_SIZE);
    copyText(book->author.name, newAuthorName, TEXT_SIZE);
    book->author.birthDate = newAuthorBirthDate;
    book->writingDate = newWritingDate;
    book->attributes = newAttributes;

    printf("\nBook updated successfully.\n");
}

void listBookTitles(MagicBook **library)
{
    int i;
    int count;

    count = 0;

    printSectionHeader("REGISTERED TITLES");

    for (i = 0; i < LIBRARY_SIZE; i++)
    {
        if (library[i] != NULL)
        {
            printf("[ID %03d] %s\n", library[i]->id, library[i]->title);
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

static int readBookAttributes(BookAttributes *attributes)
{
    initializeBookAttributes(attributes);

    printSectionHeader("OPTIONAL RPG ATTRIBUTES");

    if (!readOptionalAttribute("Does this book provide FOR / Strength? (1 yes / 0 no): ",
                               "FOR value (1-20): ",
                               &attributes->hasStrength,
                               &attributes->strength))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide DES / Dexterity? (1 yes / 0 no): ",
                               "DES value (1-20): ",
                               &attributes->hasDexterity,
                               &attributes->dexterity))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide CON / Constitution? (1 yes / 0 no): ",
                               "CON value (1-20): ",
                               &attributes->hasConstitution,
                               &attributes->constitution))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide INT / Intelligence? (1 yes / 0 no): ",
                               "INT value (1-20): ",
                               &attributes->hasIntelligence,
                               &attributes->intelligence))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide SAB / Wisdom? (1 yes / 0 no): ",
                               "SAB value (1-20): ",
                               &attributes->hasWisdom,
                               &attributes->wisdom))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide CAR / Charisma? (1 yes / 0 no): ",
                               "CAR value (1-20): ",
                               &attributes->hasCharisma,
                               &attributes->charisma))
    {
        return 0;
    }

    if (!readOptionalAttribute("Does this book provide MAG / Magic? (1 yes / 0 no): ",
                               "MAG value (1-20): ",
                               &attributes->hasMagic,
                               &attributes->magic))
    {
        return 0;
    }

    return 1;
}

static void initializeBookAttributes(BookAttributes *attributes)
{
    attributes->hasStrength = 0;
    attributes->strength = 0;

    attributes->hasDexterity = 0;
    attributes->dexterity = 0;

    attributes->hasConstitution = 0;
    attributes->constitution = 0;

    attributes->hasIntelligence = 0;
    attributes->intelligence = 0;

    attributes->hasWisdom = 0;
    attributes->wisdom = 0;

    attributes->hasCharisma = 0;
    attributes->charisma = 0;

    attributes->hasMagic = 0;
    attributes->magic = 0;
}

static int readOptionalAttribute(const char *question,
                                 const char *valueMessage,
                                 int *hasAttribute,
                                 int *attributeValue)
{
    int answer;

    if (!askYesOrNo(question, &answer))
    {
        return 0;
    }

    *hasAttribute = answer;

    if (!answer)
    {
        *attributeValue = 0;
        return 1;
    }

    return readAttributeValue(valueMessage, attributeValue);
}

static int editBookAttributes(BookAttributes *attributes, int *changed)
{
    printSectionHeader("UPDATE RPG ATTRIBUTES");

    if (!editOptionalAttribute("Update FOR / Strength? (1 yes / 0 no): ",
                               "Does this book provide FOR / Strength? (1 yes / 0 no): ",
                               "FOR value (1-20): ",
                               &attributes->hasStrength,
                               &attributes->strength,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update DES / Dexterity? (1 yes / 0 no): ",
                               "Does this book provide DES / Dexterity? (1 yes / 0 no): ",
                               "DES value (1-20): ",
                               &attributes->hasDexterity,
                               &attributes->dexterity,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update CON / Constitution? (1 yes / 0 no): ",
                               "Does this book provide CON / Constitution? (1 yes / 0 no): ",
                               "CON value (1-20): ",
                               &attributes->hasConstitution,
                               &attributes->constitution,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update INT / Intelligence? (1 yes / 0 no): ",
                               "Does this book provide INT / Intelligence? (1 yes / 0 no): ",
                               "INT value (1-20): ",
                               &attributes->hasIntelligence,
                               &attributes->intelligence,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update SAB / Wisdom? (1 yes / 0 no): ",
                               "Does this book provide SAB / Wisdom? (1 yes / 0 no): ",
                               "SAB value (1-20): ",
                               &attributes->hasWisdom,
                               &attributes->wisdom,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update CAR / Charisma? (1 yes / 0 no): ",
                               "Does this book provide CAR / Charisma? (1 yes / 0 no): ",
                               "CAR value (1-20): ",
                               &attributes->hasCharisma,
                               &attributes->charisma,
                               changed))
    {
        return 0;
    }

    if (!editOptionalAttribute("Update MAG / Magic? (1 yes / 0 no): ",
                               "Does this book provide MAG / Magic? (1 yes / 0 no): ",
                               "MAG value (1-20): ",
                               &attributes->hasMagic,
                               &attributes->magic,
                               changed))
    {
        return 0;
    }

    return 1;
}

static int editOptionalAttribute(const char *editQuestion,
                                 const char *presenceQuestion,
                                 const char *valueMessage,
                                 int *hasAttribute,
                                 int *attributeValue,
                                 int *changed)
{
    int answer;

    if (!askYesOrNo(editQuestion, &answer))
    {
        return 0;
    }

    if (!answer)
    {
        return 1;
    }

    *changed = 1;

    return readOptionalAttribute(presenceQuestion,
                                 valueMessage,
                                 hasAttribute,
                                 attributeValue);
}

static int askYesOrNo(const char *message, int *answer)
{
    int inputResult;
    int value;

    while (1)
    {
        printf("%s", message);

        inputResult = scanf("%d", &value);

        if (inputResult == EOF)
        {
            clearInputBuffer();
            return 0;
        }

        clearInputBuffer();

        if (inputResult != 1 || (value != 0 && value != 1))
        {
            printf("Type 1 for yes or 0 for no.\n");
            continue;
        }

        *answer = value;
        return 1;
    }
}

static int readAttributeValue(const char *message, int *value)
{
    int inputResult;
    int typedValue;

    while (1)
    {
        printf("%s", message);

        inputResult = scanf("%d", &typedValue);

        if (inputResult == EOF)
        {
            clearInputBuffer();
            return 0;
        }

        clearInputBuffer();

        if (inputResult != 1)
        {
            printf("Invalid value. Try again.\n");
            continue;
        }

        if (typedValue < ATTRIBUTE_MIN || typedValue > ATTRIBUTE_MAX)
        {
            printf("Value must be between %d and %d.\n", ATTRIBUTE_MIN, ATTRIBUTE_MAX);
            continue;
        }

        *value = typedValue;
        return 1;
    }
}

static void displayBookAttributes(const BookAttributes *attributes)
{
    printSectionHeader("RPG ATTRIBUTES");

    if (!hasAnyBookAttribute(attributes))
    {
        printf("No RPG attributes registered for this book.\n");
        return;
    }

    if (attributes->hasStrength)
    {
        printf("FOR / Strength: %d\n", attributes->strength);
    }

    if (attributes->hasDexterity)
    {
        printf("DES / Dexterity: %d\n", attributes->dexterity);
    }

    if (attributes->hasConstitution)
    {
        printf("CON / Constitution: %d\n", attributes->constitution);
    }

    if (attributes->hasIntelligence)
    {
        printf("INT / Intelligence: %d\n", attributes->intelligence);
    }

    if (attributes->hasWisdom)
    {
        printf("SAB / Wisdom: %d\n", attributes->wisdom);
    }

    if (attributes->hasCharisma)
    {
        printf("CAR / Charisma: %d\n", attributes->charisma);
    }

    if (attributes->hasMagic)
    {
        printf("MAG / Magic: %d\n", attributes->magic);
    }
}

static void displayBookPowerProfile(const BookAttributes *attributes)
{
    int activeCount;
    int totalValue;
    int dominantValue;
    int powerLevel;
    double average;
    const char *dominantAttribute;
    const char *bookArchetype;

    activeCount = 0;
    totalValue = 0;
    dominantValue = -1;
    dominantAttribute = "None";
    bookArchetype = "None";

    includeAttributeInPowerProfile(attributes->hasStrength,
                                   attributes->strength,
                                   "FOR / Strength",
                                   "Warrior Tome",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasDexterity,
                                   attributes->dexterity,
                                   "DES / Dexterity",
                                   "Rogue Manual",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasConstitution,
                                   attributes->constitution,
                                   "CON / Constitution",
                                   "Guardian Codex",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasIntelligence,
                                   attributes->intelligence,
                                   "INT / Intelligence",
                                   "Scholar Grimoire",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasWisdom,
                                   attributes->wisdom,
                                   "SAB / Wisdom",
                                   "Oracle Scroll",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasCharisma,
                                   attributes->charisma,
                                   "CAR / Charisma",
                                   "Royal Manuscript",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    includeAttributeInPowerProfile(attributes->hasMagic,
                                   attributes->magic,
                                   "MAG / Magic",
                                   "Arcane Spellbook",
                                   &activeCount,
                                   &totalValue,
                                   &dominantValue,
                                   &dominantAttribute,
                                   &bookArchetype);

    average = 0.0;

    if (activeCount > 0)
    {
        average = (double) totalValue / activeCount;
    }

    powerLevel = calculatePowerLevel(average);

    printSectionHeader("POWER PROFILE");
    printf("Active attributes: %d/7\n", activeCount);
    printf("Average attribute value: %.2f\n", average);
    printf("Power level: %d/5\n", powerLevel);
    printf("Rank: %s\n", getPowerRank(powerLevel));
    printf("Dominant attribute: %s\n", dominantAttribute);
    printf("Book archetype: %s\n", bookArchetype);
}

static void includeAttributeInPowerProfile(int hasAttribute,
                                           int value,
                                           const char *attributeName,
                                           const char *archetype,
                                           int *activeCount,
                                           int *totalValue,
                                           int *dominantValue,
                                           const char **dominantAttribute,
                                           const char **bookArchetype)
{
    if (!hasAttribute)
    {
        return;
    }

    (*activeCount)++;
    *totalValue += value;

    if (value > *dominantValue)
    {
        *dominantValue = value;
        *dominantAttribute = attributeName;
        *bookArchetype = archetype;
    }
}

static int calculatePowerLevel(double average)
{
    if (average <= 0.0)
    {
        return 0;
    }

    if (average <= 4.0)
    {
        return 1;
    }

    if (average <= 8.0)
    {
        return 2;
    }

    if (average <= 12.0)
    {
        return 3;
    }

    if (average <= 16.0)
    {
        return 4;
    }

    return 5;
}

static const char *getPowerRank(int powerLevel)
{
    switch (powerLevel)
    {
        case 1:
            return "Weak";

        case 2:
            return "Apprentice";

        case 3:
            return "Adept";

        case 4:
            return "Arcane";

        case 5:
            return "Legendary";

        default:
            return "No power level";
    }
}

static int hasAnyBookAttribute(const BookAttributes *attributes)
{
    return attributes->hasStrength ||
           attributes->hasDexterity ||
           attributes->hasConstitution ||
           attributes->hasIntelligence ||
           attributes->hasWisdom ||
           attributes->hasCharisma ||
           attributes->hasMagic;
}

static void printSectionHeader(const char *title)
{
    int i;
    int insideWidth;
    int titleLength;
    int labelWidth;
    int leftPadding;
    int rightPadding;

    insideWidth = 52;
    titleLength = (int) strlen(title);
    labelWidth = titleLength + 2;

    if (labelWidth > insideWidth)
    {
        printf("\n+ %s +\n\n", title);
        return;
    }

    leftPadding = (insideWidth - labelWidth) / 2;
    rightPadding = insideWidth - labelWidth - leftPadding;

    printf("\n+");

    for (i = 0; i < leftPadding; i++)
    {
        printf("-");
    }

    printf(" %s ", title);

    for (i = 0; i < rightPadding; i++)
    {
        printf("-");
    }

    printf("+\n\n");
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
