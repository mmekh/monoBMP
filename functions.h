#ifndef FUNC
#define FUNC

#include "structures.h"

int infoSize,i,a=255*3/2;//25 отвечает за баланс белого-черного
unsigned int s,p[4];

void PutPixel(int m,int n,FILE *bmp)
{
    while(n>0)
    {
        fputc(m,bmp);
        n--;
    }
}
void ScanFILEHEADER(FILE *bmp)
{

    //Если начальный файл не BMP, то не создаем новый файл
    fread(&fileHeader,sizeof(fileHeader),1,bmp);
    if (fileHeader.bfType != 0x4d42)
    {
        printf("File is not BMP");
        getc(stdin);
        exit(1);
    }
}
void ScanFILEINFO(FILE *bmp)
{

    //Начинаем считывать BITMAPINFO
    fread(&infoSize,sizeof(fileInfo.biSize),1,bmp);//Размер структуры
    fseek(bmp,14,SEEK_SET);
    fread(&fileInfo,infoSize,1,bmp);
}

void CreateHeader(FILE *bmp_new)
{
    fwrite(&fileHeader,sizeof(fileHeader),1,bmp_new);
    fwrite(&fileInfo,infoSize,1,bmp_new);
}

void CreateBitMap(FILE *bmp,FILE *bmp_new)
{
    if (fileInfo.biBitCount<=8)
    {

        for (i=0;i<pow(2,fileInfo.biBitCount)*4;i++)
        {
            p[i%4]=fgetc(bmp);
            s+=p[i%4];
            if (i%4==3)
            {
                if (s>a)
                {
                    PutPixel(255,4,bmp_new);
                }
                else
                {
                    PutPixel(0,4,bmp_new);
                }
                s=0;
            }
        }
        for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
        {
            putc(fgetc(bmp),bmp_new);
        }
    }
    //still not working
    if (fileInfo.biBitCount==16)
    {
        DWORD pixel,b,g,r;
        if (fileInfo.biCompression==3&&fileInfo.biSize==40)
        {
            fread(&mask16,sizeof(char),12,bmp);
            fwrite(&mask16,sizeof(char),12,bmp_new);
            for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
            {
                fread(&pixel,sizeof(pixel),1,bmp);
                b=mask16.rgbBlue&pixel;
                g=mask16.rgbGreen&pixel;
                r=mask16.rgbRed&pixel;
                s=(mask16.rgbBlue+mask16.rgbGreen/(mask16.rgbBlue+1)+mask16.rgbRed/(mask16.rgbBlue|mask16.rgbGreen+1)+3)/2;
                if(b+g+r>s)
                {
                    pixel=0xFFFF;
                    fwrite(&pixel,sizeof(char),2,bmp_new);
                }
                else
                {
                   pixel=0x0000;
                   fwrite(&pixel,sizeof(char),2,bmp_new);
                }
            }
        }
        else if (fileInfo.biCompression==6&&fileInfo.biSize==40)
        {
            fread(&mask16,sizeof(char),16,bmp);
            fwrite(&mask16,sizeof(char),16,bmp_new);
            for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
            {
                fread(&mask16,sizeof(char),12,bmp);
                fwrite(&mask16,sizeof(char),12,bmp_new);
                printf("Masks:\n%x-BLUE",mask16.rgbBlue);
                printf("\n%x-GREEN",mask16.rgbGreen);
                printf("\n%x-RED\n",mask16.rgbRed);
            }
        }
        else
        {
            for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
            {

            }
        }
    }
    //
    if (fileInfo.biBitCount==24)
    {
        RGB24 *palette,*pixel;
        for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
        {
            fread(&palette,sizeof(RGB24),1,bmp);
            pixel=&palette;
            s=pixel->rgbBlue+pixel->rgbGreen+pixel->rgbRed;
            if (s > a)
            {
                PutPixel(255,3,bmp_new);
            }
            else
            {
                PutPixel(0,3,bmp_new);
            }
        }
    }
    if (fileInfo.biBitCount==32)
    {
        RGB32 *palette,*pixel;
        for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
        {
            fread(&palette,sizeof(RGB32),1,bmp);
            pixel=&palette;
            s=pixel->rgbBlue+pixel->rgbGreen+pixel->rgbRed;
            if (s > a)
            {
                PutPixel(255,3,bmp_new);
                fputc(pixel->rgbReserved,bmp_new);
            }
            else
            {
                PutPixel(0,3,bmp_new);
                fputc(pixel->rgbReserved,bmp_new);
            }
        }
    }
}

#endif // FUNC
