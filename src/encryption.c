/* Implements string encryption and decryption operations. */

#include "encryption.h"

void encryptString(char *str)
{
    int i;

    i = 0;

    while (str[i] != '\0')
    {
        str[i] = (char)(255 - (unsigned char) str[i]);
        i++;
    }
}

void decryptString(char *str)
{
    encryptString(str);
}
