#ifndef FUNC
#define FUNC

#include "structures.h"
#include "PutPixel.c"

int infoSize,i,a=255*3/2;
unsigned int s,p[4];


void Rename(char name[],char q)
{
    char* eqSign;
    eqSign=strchr(name,q);

    memmove(name,eqSign+1,strlen(name) - (eqSign - name) + 1);
}
FILE* BMPname(char name1[],char name2[],char io[])
{
    FILE *temp;
    if (strstr(name1,io)!=0)
    {
        Rename(name1,'=');
        FILE *temp = fopen(name1,"rb");
        if (temp==NULL)
            perror("");
        return temp;
    }
    else
    {
        Rename(name2,'=');
        FILE *temp = fopen(name2,"wb");
        if (temp==NULL)
            perror("");
        return temp;
    }
}
void ScanFILEHEADER(FILE *bmp)
{
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
    fread(&infoSize,sizeof(fileInfo.biSize),1,bmp);
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
    if (fileInfo.biBitCount==1)
    {
        for (i=0;i<2;i++)
        {
            p[1]=fgetc(bmp);
            p[2]=fgetc(bmp);
            p[3]=fgetc(bmp);
            p[4]=fgetc(bmp);
            s=p[1]+p[2]+p[3];
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
        for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
        {
            putc(fgetc(bmp),bmp_new);
        }
    }

    if (fileInfo.biBitCount==4)
    {
        for (i=0;i<64;i++)
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

    if (fileInfo.biBitCount==8)
    {
        for (i=0;i<256;i++)
        {
            p[1]=fgetc(bmp);
            p[2]=fgetc(bmp);
            p[3]=fgetc(bmp);
            p[4]=fgetc(bmp);
            s=p[1]+p[2]+p[3];
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
        for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
        {
            putc(fgetc(bmp),bmp_new);
        }
    }

    if (fileInfo.biBitCount==16)
    {
        WORD pixel,palette,b,g,r,a;
        if (fileInfo.biCompression==3)
        {
            mask16.rgbBlue=0x001F;
            mask16.rgbGreen=0x07E0;
            mask16.rgbRed=0xF800;
            if(fileInfo.biSize==40)
            {
                fread(&mask16,sizeof(char),12,bmp);
                fwrite(&mask16,sizeof(char),12,bmp_new);
            }

            for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
            {
                fread(&pixel,sizeof(pixel),1,bmp);
                b=mask16.rgbBlue&pixel;
                g=(mask16.rgbGreen&pixel)/(mask16.rgbBlue+1);
                r=(mask16.rgbRed&pixel)/(mask16.rgbBlue|mask16.rgbGreen+1);
                s=(b+g+r)/2;
                if(b+g+r>s)
                {
                    PutPixel(255,2,bmp_new);
                }
                else
                {
                    PutPixel(0,2,bmp_new);
                }
            }
        }
        else
        {
            mask16.rgbAlpha=0x8000;
            mask16.rgbBlue=0x001F;
            mask16.rgbGreen=0x03E0;
            mask16.rgbRed=0x7C00;
            if(fileInfo.biSize==40&&fileInfo.biCompression==6)
            {
                fread(&mask16,sizeof(char),16,bmp);
                fwrite(&mask16,sizeof(char),16,bmp_new);
            }
            for (i=0;i<fileInfo.biWidth*fileInfo.biHeight;i++)
            {
                fread(&pixel,sizeof(pixel),1,bmp);
                b=mask16.rgbBlue&pixel;
                g=(mask16.rgbGreen&pixel)/(mask16.rgbBlue+1);
                r=(mask16.rgbRed&pixel)/(mask16.rgbBlue|mask16.rgbGreen+1);
                s=(b+g+r)/2;
                a=mask16.rgbAlpha&pixel/(mask16.rgbBlue|mask16.rgbGreen|mask16.rgbRed+1);
                if(b+g+r>s)
                {
                    pixel=0x7FFF|a;
                    fwrite(&pixel,sizeof(pixel),1,bmp_new);
                }
                else
                {
                    PutPixel(0,2,bmp_new);
                }
            }
        }
    }

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
