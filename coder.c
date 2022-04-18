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

    unsigned int containerSize;

    if(containerFile && inputFile)
    {
        if(checkBMPFile(containerFile))
        {
            printWrongFiles();
            return 0;
        }
        
        containerSize = calculateFileSize(containerFile);
    }

    else
    {
        printWrongFiles();
        exit(EXIT_FAILURE);
    }

    return 0;
}