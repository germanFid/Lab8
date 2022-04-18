#include <stdlib.h>
#include <stdio.h>
#include <string.h>

short checkBMPFile(FILE* f) // check if file is a bmp one
{
    uint8_t header[2]; // .bmp file header (aka 'BM')
    int result = fread(&header, 2, 1, f);

    fseek(f, 0, SEEK_SET);

    if (header[0] == 66 && header[1] == 77)
    {
        return 0;
    }

    return -1;
}

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
        counter--;
    }
    memcpy(&resultSize, &sizeRaw, 4);
    fseek(f, 0, SEEK_SET);

    return resultSize;
}