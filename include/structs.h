/* Defines the core data structures used by the Magic Library system. */
#ifndef STRUCTS_H
#define STRUCTS_H

#define LIBRARY_SIZE 100
#define TEXT_SIZE 100

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
    int id;
    char title[TEXT_SIZE];
    Author author;
    Date writingDate;
} MagicBook;

#endif
