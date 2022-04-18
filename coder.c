#include "bitmap.h"
#include "ui.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printWrongUsage();
        exit(EXIT_FAILURE);
    }

    FILE* containerFile = fopen(argv[1], "rw");
    FILE* inputFile = fopen(argv[2], "r");

    unsigned int fSize;

    if(containerFile && inputFile)
    {
        fSize = calculateBMPSize(containerFile);
        printf("Size: %u", fSize);
    }

    else
    {
        printWrongFiles();
        exit(EXIT_FAILURE);
    }

    return 0;
}