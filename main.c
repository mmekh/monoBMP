#include "functions.h"

main(int argc,char * argv[])
{
    if (argv[1]==NULL || argv[2]==NULL)
    {
        printf("Not enough arguments");
        exit(1);
    }
    FILE *bmp,*bmp_new;
    bmp=BMPname(argv[1],argv[2],"input");
    ScanFILEHEADER(bmp);
    bmp_new=BMPname(argv[1],argv[2],"output");
    ScanFILEINFO(bmp);
    CreateHeader(bmp_new);
    CreateBitMap(bmp,bmp_new);
    fclose(bmp);
    fclose(bmp_new);
    printf("Done.");
    getc(stdin);
    return 0;
}
