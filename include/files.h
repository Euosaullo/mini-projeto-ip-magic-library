/* Declares file persistence operations for saving and loading the library. */
#ifndef FILES_H
#define FILES_H

#include "structs.h"

void saveLibraryToFile(MagicBook **library, const char *fileName);

void loadLibraryFromFile(MagicBook **library, const char *fileName);

#endif
