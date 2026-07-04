/* Declares the main library operations for managing magic books. */
#ifndef LIBRARY_H
#define LIBRARY_H

#include "structs.h"

void initializeLibrary (MagicBook **library);
void registerBook (MagicBook **library);
void deleteBookById (MagicBook **library, int id);
void displayBookById (MagicBook **library, int id);
void updateBookById (MagicBook **library, int id);
void listBookTitles (MagicBook **library);

#endif
