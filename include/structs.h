/* Defines the core data structures used by the Magic Library system. */
#ifndef STRUCTS_H
#define STRUCTS_H

#define LIBRARY_SIZE 100
#define TEXT_SIZE 100
#define ATTRIBUTE_MIN 1
#define ATTRIBUTE_MAX 20

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    char name[TEXT_SIZE];
    Date birthDate;
} Author;

typedef struct
{
    int hasStrength;
    int strength;

    int hasDexterity;
    int dexterity;

    int hasConstitution;
    int constitution;

    int hasIntelligence;
    int intelligence;

    int hasWisdom;
    int wisdom;

    int hasCharisma;
    int charisma;

    int hasMagic;
    int magic;
} BookAttributes;

typedef struct
{
    int id;
    char title[TEXT_SIZE];
    Author author;
    Date writingDate;
    BookAttributes attributes;
} MagicBook;

#endif
