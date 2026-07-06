/* Declares the main library operations for managing magic books. */
#ifndef LIBRARY_H
#define LIBRARY_H

#include "structs.h"

void initializeLibrary(MagicBook **library);

void freeLibrary(MagicBook **library);

void registerBook(MagicBook **library);

void deleteBookById(MagicBook **library, int id);

void displayBookById(MagicBook **library, int id);

void updateBookById(MagicBook **library, int id);

void listBookTitles(MagicBook **library);

int findFreeLibrarySlot(MagicBook **library);

int findBookIndexById(MagicBook **library, int id);

int bookIdExists(MagicBook **library, int id);

#endif
