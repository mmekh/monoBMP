#ifndef STRUCT
#define STRUCT

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#pragma pack(push,1)

typedef uint8_t BYTE;//8 бит без знака
typedef uint16_t WORD;//16 бит без знака
typedef uint32_t DWORD;//32 бита без знака
typedef int32_t LONG;//32 бита со знаком
typedef int FXPT2DOT30;

typedef struct {
	FXPT2DOT30 ciexyzX;
	FXPT2DOT30 ciexyzY;
	FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
	CIEXYZ  ciexyzRed;
	CIEXYZ  ciexyzGreen;
	CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct
{
    unsigned char rgbRed;
    unsigned char rgbGreen;
    unsigned char rgbBlue;
    unsigned char rgbReserved;
}RGB32;

typedef struct
{
    unsigned char rgbRed;
    unsigned char rgbGreen;
    unsigned char rgbBlue;
}RGB24;

struct mask
{
    DWORD rgbRed;
    DWORD rgbGreen;
    DWORD rgbBlue;
    DWORD rgbAlpha;
}mask16;

struct BITMAPFILEHEADER
{
    WORD bfType;//Отметка для отличия формата от других 4D42/424D
    DWORD bfSize;//Размер файла в байтах
    WORD bfReserved1;//Зарезервированны и содержат 0
    WORD bfReserved2;//Зарезервированны и содержат 0
    DWORD bfOffBits;//Положение пиксельных данных относительно начала данной структуры (в байтах)
}fileHeader;

struct BITMAPINFO
{
    // CORE
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    //Добавлено в версии 3
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    //Добавлено в версии 4
    DWORD biRedMask;
    DWORD biGreenMask;
    DWORD biBlueMask;
    DWORD biAlphaMask;
    DWORD biCSType;
    CIEXYZTRIPLE biEndpoints;
    DWORD biGammaRed;
    DWORD biGammaGreen;
    DWORD biGammaBlue;
    //Добавлены в версии 5
    DWORD biIntent;
    DWORD biProfileData;
    DWORD biProfileSize;
    DWORD biReserved;
}fileInfo;

#endif //STRUCT
