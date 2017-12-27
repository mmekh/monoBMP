#include "functions.h"

main(int argc,char * argv[])
{
    char name[100];
    //Читаем и создаем новое имя файла
	if (argv[1]==NULL)
    {
       	printf("Enter file name: ");
       	gets(name);
    }
    else
        strcpy(name,argv[1]);
    FILE *bmp = fopen(name,"rb");
    name[strlen(name)-4]='\0';
    strcat(name,"_mono.bmp");
    ScanFILEHEADER(bmp);
    ScanFILEINFO(bmp);
    printf("Bit count: %d\n",fileInfo.biBitCount);
    printf("Compression:%d\n",fileInfo.biClrUsed);
    printf("Size: %d\n",fileInfo.biSize);
    printf("Palette/mask field size: %d\n",(fileHeader.bfOffBits-fileInfo.biSize-14));
    FILE *bmp_new = fopen(name,"wb");
    if (bmp_new==NULL)
        perror("");
    CreateHeader(bmp_new);
    CreateBitMap(bmp,bmp_new);
    printf("Done.");
    getc(stdin);
    return 0;
}
