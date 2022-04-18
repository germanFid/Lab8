#include <stdlib.h>
#include <stdio.h>
#include <string.h>

short checkBMPFile(FILE* f) // check if file is a bmp one
{
    uint8_t header[2]; // .bmp file header (aka 'BM')
    int result;

    result = fread(&header, 2, 1, f);
    fseek(f, 0, SEEK_SET);

    if (header[0] == 66 && header[1] == 77)
    {
        return 0;
    }

    return -1;
}

unsigned int calculateBMPSize(FILE* f)
{
    int resultSize;
    return resultSize;
}