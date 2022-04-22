#include "bitmap.h"
#include "ui.h"
#include "byte.h"

#define EXTENTION_SIZE 6

int calculateFileSize(FILE* f)
{
    fseek(f, 0, SEEK_END);
    int res = ftell(f);

    fseek(f, 0, SEEK_SET);
    return res;
}

int coverPrecheck(BITMAPFILEHEADER containerBitmapFileHeader, FILE* inputFile, int degree)
{
    int iSize = calculateFileSize(inputFile);
    int cSize = containerBitmapFileHeader.bfSize;
    int cOffset = containerBitmapFileHeader.bfOffBits;

    int pSize = (iSize + sizeof(DWORD) + EXTENTION_SIZE) * 8 / degree;

    printf("Input file size: %d\nContainer file size: %d\nAvailable: %d\nPacking input file takes: %d\n",
           iSize, cSize, cSize - cOffset, pSize);

    if (pSize < cSize - cOffset)
    {
        return 0;
    }

    return -1;
}

int modifyPixelData(unsigned char *containerBitmapData, int cSize, FILE* inputFile ,int degree)
{
    int operations = 0;

    unsigned char bitmask = -1;
    unsigned int cBitmapDataCounter = 0;
    union byte dataByte;

    bitmask = bitmask << degree;
    printf("--- Bitmask: %X\n\n", bitmask);
    int fsize = calculateFileSize(inputFile);

    for (size_t i = 0; i < fsize; i++)
    {   
        unsigned char c;
        fread(&c, 1, 1, inputFile);
        bInit(&dataByte, c);

        if (operations < 10)
            printf("--- %X ---\n", c);
        
        int read = 1;
        for (size_t j = 0; j < 8 / degree; j++)
        {
            // Берем очередной байт и пересекаем его с битмаской
            if (operations < 10)
                printf("- %X ", containerBitmapData[cBitmapDataCounter]);
            containerBitmapData[cBitmapDataCounter] = containerBitmapData[cBitmapDataCounter] & bitmask;
            
            if (operations < 10)
                printf("%X ", containerBitmapData[cBitmapDataCounter]);

            if (operations < 10)
                printf("(%d %d)", read, read + degree);
            containerBitmapData[cBitmapDataCounter] += bGetBits(read, read + degree, &dataByte);

            if (operations < 10)
                printf(" + %X", bGetBits(read, read + degree, &dataByte));

            if (operations < 10)
                printf(" = %X\n", containerBitmapData[cBitmapDataCounter]);

            read += degree;
            // printf("> j\n");
            operations++;
            cBitmapDataCounter++;
        }
    }
    return operations;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printWrongUsage();
        exit(EXIT_FAILURE);
    }

    int packingDegree = atoi(argv[3]);

    if (packingDegree > 8 || packingDegree < 1)
    {
        printWrongPackingDegree();
        exit(EXIT_FAILURE);
    }

    FILE *containerFile = fopen(argv[1], "rb");
    FILE *inputFile = fopen(argv[2], "rb");


    if (containerFile == NULL || inputFile == NULL)
    {
        printWrongFiles();
        exit(EXIT_FAILURE);
    }

    BITMAPFILEHEADER containerBitmapFileHeader;
    BITMAPINFOHEADER containerBitmapInfoHeader;

    unsigned char *containerBitmapData;
    unsigned char *inputBitmapData;

    containerBitmapData = LoadBitmapFile(containerFile,
                                         &containerBitmapInfoHeader,
                                         &containerBitmapFileHeader);

    int iHSize = sizeof(containerBitmapInfoHeader);
    int fHSize = sizeof(containerBitmapFileHeader);

    for (size_t i = 0; i < 10; i++)
    {
        printf(">>> %X\n", containerBitmapData[i]);
    }
    

    int pCheck = coverPrecheck(containerBitmapFileHeader, inputFile, packingDegree);
    
    if (pCheck)
    {
        printInsSpace();
        exit(EXIT_FAILURE);
    }

    printf("PreCheck OK!\n\n");

    fclose(containerFile);
    containerFile = fopen(argv[1], "rb");

    FILE* resultFile = fopen("result.bmp", "wb");
    printf("Writing %d bytes before bitmap data...\n", containerBitmapFileHeader.bfOffBits);

    unsigned char byte;
    for (size_t i = 0; i < containerBitmapFileHeader.bfOffBits; i++)
    {
        fread(&byte, 1, 1, containerFile);
        // printf("> %X\n", byte);
        fwrite(&byte, 1, 1, resultFile);
    }
    

    int res = 0;

    res = modifyPixelData(containerBitmapData, containerBitmapFileHeader.bfSize - containerBitmapFileHeader.bfOffBits, inputFile, packingDegree);

    printf("Modifying operations: %d\n", res);

    res = 0;
    for (size_t i = 0; i < containerBitmapFileHeader.bfSize - containerBitmapFileHeader.bfOffBits; i++)
    {
        fwrite(&containerBitmapData[i], 1, 1, resultFile);
        res++;
    }

    // res = fwrite(containerBitmapData, sizeof(containerBitmapData), 1, resultFile);
    // printf("Objects written: %d\nEach %lu bytes\n", res, sizeof(containerBitmapData));

    fclose(resultFile);
    fclose(containerFile);
    fclose(inputFile);

    return 0;
}