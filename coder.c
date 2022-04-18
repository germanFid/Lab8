#include "bitmap.h"
#include "ui.h"

int main(int argc, char *argv[])
{
    // if (argc != 4)
    // {
    //     printWrongUsage();
    //     exit(EXIT_FAILURE);
    // }

    // FILE *containerFile = fopen(argv[1], "rb");
    // FILE *inputFile = fopen(argv[2], "rb");

    FILE *containerFile = fopen("stormrooper.bmp", "rb");
    FILE *inputFile = fopen("image.bmp", "rb");


    if (containerFile == NULL || inputFile == NULL)
    {
        printWrongFiles();
        return 0;
    }

    BITMAPFILEHEADER containerBitmapFileHeader;
    BITMAPFILEHEADER inputBitmapFileHeader;

    BITMAPINFOHEADER containerBitmapInfoHeader;
    BITMAPINFOHEADER inputBitmapInfoHeader;

    unsigned char *containerBitmapData;
    unsigned char *inputBitmapData;

    containerBitmapData = LoadBitmapFile(containerFile,
                                         &containerBitmapInfoHeader,
                                         &containerBitmapFileHeader);

    inputBitmapData = LoadBitmapFile(inputFile,
                                     &inputBitmapInfoHeader,
                                     &inputBitmapFileHeader);

    printf("input file size: %d\ncontainer file size: %d\n",
           inputBitmapFileHeader.bfSize,
           containerBitmapFileHeader.bfSize);

    return 0;
}