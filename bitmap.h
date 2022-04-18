#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int calculateBMPSize(FILE* f)
{
    for (size_t i = 0; i < 2; i++)
        getc(f);

    int resultSize = 0;
    unsigned char sizeRaw[4];
    
    char counter = 3;
    for (size_t i = 0; i < 4; i++)
    {
        sizeRaw[counter] = getc(f);
        printf("> %u\n", sizeRaw[counter]);
        counter--;
    }
    memcpy(&resultSize, &sizeRaw, 4);
    
    return resultSize;
}

// int calculateBMPOffset()
// {
    
// }