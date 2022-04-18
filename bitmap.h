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

unsigned int calculateFileSize(FILE* f)
{
    uint32_t resultSize;
    uint8_t bfSize[4];

    fseek(f, 0x02, SEEK_SET); // go to file size in .bmp structure
    fread(&bfSize, 4, 1, f);

    for (size_t i = 0; i < 4; i++)
    {
        printf("%X ", bfSize[i]);
    }

    // printf("%d", atoi(&bfSize));
    

    return resultSize;
}