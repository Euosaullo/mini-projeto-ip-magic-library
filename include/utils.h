/* Declares shared utility functions used by the Magic Library system. */
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void clearInputBuffer(void);

int removeLineBreak(char str[]);

void discardRestOfLine(FILE *file);

int readLine(const char *message, char str[], int size);

void copyText(char destination[], const char source[], int size);

int isValidDate(int day, int month, int year);

#endif
