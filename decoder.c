#include <stdio.h>

#include "ui.h"
#include "bitmap.h"
#include "byte.h"

void writeBufferByte(char inputByte, unsigned char* bufferData, int* bufByte, int* bufPosition, int degree)
{
    union byte b; // представление inputByte
    
    bInit(&b, inputByte);
    int startBit;

    // if (*bufPosition != 8 - degree)
    // {
    //     startBit = 1;
    //     printf("Mode 1\n");
    // }

    // else
    // {
    //     startBit = 8 - degree;
    //     printf("Mode 2\n");
    // }

    startBit = 9 - degree;
    // printf("Mode 2\n");

    // printf("> %d\n", startBit);
    for (size_t i = startBit; i <= 8; i++) // Проход по битам b
    {
        char c = bGetBit(9 - i, &b); // Получаем нужный бит

        bufferData[*bufByte] += c << (7 - *bufPosition);
        // printf(">> %d\t%d\t%X\n", *bufByte, *bufPosition, c);
        // printf("%X\n", bufferData[*bufByte]);
        *bufPosition += 1;

        if(*bufPosition > 7)
        {
            // printf("---+>\n");
            *bufPosition = 0;
            *bufByte += 1;
        }
    }
}

void readBuffer(unsigned char *containerBitmapData, int cSize, unsigned char *bufferData, int bSize, int degree)
{
    char bitmask = 0;
    int bufByte = 0; // Номер байта в буфере
    int bufPosition = 0; // Номер позиции в байте

    // Генерируем битмаску
    for (size_t i = 0; i < degree; i++)
    {
        bitmask = bitmask << 1;
        bitmask += 1;
    }

    // Начинаем считывание скрытых байтов
    for (size_t i = 0; i < cSize; i++)
    {
        char c;

        c = containerBitmapData[i] & bitmask;
        writeBufferByte(c, bufferData, &bufByte, &bufPosition, degree);
    }
    
}

int main(int argc, char* argv[]) // <input filename> <packing degree> <output filename>
{
    if (argc != 4)
    {
        printWrongUsage();
        exit(EXIT_FAILURE);
    }

    FILE *inputFile = fopen(argv[1], "rb");
    // FILE *bufferFile = fopen(".buffer", "wb");

    if(inputFile == NULL)
    {
        printWrongFiles();
        exit(EXIT_FAILURE);
    }

    int packingDegree = atoi(argv[2]);

    BITMAPFILEHEADER containerBitmapFileHeader;
    BITMAPINFOHEADER containerBitmapInfoHeader;

    unsigned char* containerBitmapData, *bufferData;
    int buffSize;

    // Считываем контейнер
    containerBitmapData = LoadBitmapFile(inputFile,
                                         &containerBitmapInfoHeader,
                                         &containerBitmapFileHeader);

    fclose(inputFile);
    inputFile = fopen(argv[1], "rb");

    buffSize = calculateFileSize(inputFile);
    printf("Bufsize: %d\n", buffSize);
    bufferData = (unsigned char*) malloc(buffSize);
    readBuffer(containerBitmapData, containerBitmapFileHeader.bfSize, bufferData, buffSize, packingDegree);

    for (int i = 0; i < 100; i++)
    {
        // printf("> %d %X %c\n", i, bufferData[i], bufferData[i]);
        printf("%X ", bufferData[i]);
    }
    printf("\n");

    fclose(inputFile);
}