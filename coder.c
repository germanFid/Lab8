#include "bitmap.h"
#include "ui.h"
#include "byte.h"

#include <string.h>

#define EXTENTION_SIZE 6

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

void writeByte(char inputByte, unsigned char *containerBitmapData, int *numByte, int *numPosition, int degree)
{
    printf("--- %c %X ---\n", inputByte, inputByte);
    union byte b;
    bInit(&b, inputByte);

    for (size_t i = 8; i >= 1; i--)
    {
        char c = bGetBit(i, &b);
        containerBitmapData[*numByte] += c << *numPosition;
        printf(">> %d\t%d\t%X\n", *numByte, *numPosition, c);
        *numPosition -= 1;

        if (*numPosition < 0)
        {
            *numByte += 1;
            containerBitmapData[*numByte] = containerBitmapData[*numByte] & ((unsigned char)-1 << degree);
            *numPosition = degree - 1;
        }
    }
    
}

void modifyPixelData(unsigned char *containerBitmapData, int cSize, FILE* inputFile , int degree, char* extension)
{
    unsigned char bitmask = (unsigned char)-1 << degree;

    // bitmask = bitmask << degree;
    printf("\n--- Bitmask: %X\n", bitmask);
    int fsize = calculateFileSize(inputFile);

    union byte dataByte;
    int numByte = 0, numPosition = degree - 1;
    containerBitmapData[numByte] = containerBitmapData[numByte] & ((unsigned char)-1 << degree);

    // Пишем содержимое файла
    for (size_t i = 0; i < fsize; i++)
    {   
        char c;
        fread(&c, 1, 1, inputFile);
        bInit(&dataByte, c);
        
        writeByte(c, containerBitmapData, &numByte, &numPosition, degree);
    }

    // Пишем три точки, обозначающие начало расширения
    for (size_t i = 0; i < 3; i++)
    {   
        char c = '.';
        bInit(&dataByte, c);
        
        writeByte(c, containerBitmapData, &numByte, &numPosition, degree);
    }

    int flag = 0;
    // Пишем расширение (то, что после точки в имени файла)
    for (size_t i = 0; i < strlen(extension); i++)
    {   
        char c = extension[i];
        
        if(!flag)
        {
            if(c == '.')
            {
                flag = 1;
            }
        }

        else
        {
            bInit(&dataByte, c);
            writeByte(c, containerBitmapData, &numByte, &numPosition, degree);
        }
    }

    // Пишем три точки, обозначающие конец расширения
    for (size_t i = 0; i < 3; i++)
    {   
        char c = '.';
        bInit(&dataByte, c);
        
        writeByte(c, containerBitmapData, &numByte, &numPosition, degree);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printWrongUsage();
        exit(EXIT_FAILURE);
    }

    int packingDegree = atoi(argv[3]);
    char extenstion[EXTENTION_SIZE];

    char start = -1;
    for (size_t i = 0; i < sizeof(argv[2]); i++)
    {
        if(argv[2][i] == '.')
        {
            start = i;
        }
    }

    if (packingDegree > 8 || packingDegree < 1)
    {
        printWrongPackingDegree();
        exit(EXIT_FAILURE);
    }

    FILE* containerFile = fopen(argv[1], "rb");
    FILE* inputFile = fopen(argv[2], "rb");


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
    

    modifyPixelData(containerBitmapData, containerBitmapFileHeader.bfSize - containerBitmapFileHeader.bfOffBits, inputFile, packingDegree, argv[2]);

    for (size_t i = 0; i < containerBitmapFileHeader.bfSize - containerBitmapFileHeader.bfOffBits; i++)
    {
        fwrite(&containerBitmapData[i], 1, 1, resultFile);
    }

    fclose(resultFile);
    fclose(containerFile);
    fclose(inputFile);

    return 0;
}
