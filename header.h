#ifndef HEADER_H
#define HEADER_H

//инклюды

#include <stdint.h>

//структуры

typedef struct PIXEL //32 разрядный rgb  пиксель с прозрачностью
{
    uint8_t r,g,b,a;
}pixels_s;

typedef struct s_pixelmap_s//структура пикселей в изображении
{
    size_t width,height;//размеры изображения
    pixels_s *data;//одномерный массив пикселей
}pixelmap_s;

//прототипы

pixelmap_s *createPixelmap(const size_t,const size_t);
void save(pixelmap_s *, const char *);
pixelmap_s *load(const char *);

//функции

//Создание новой структуры пикселей размером width*height
pixelmap_s *createPixelmap (const size_t width, const size_t height)
{
    //выделение памяти на структуру
	pixelmap_s * newPixelmap = (pixelmap_s *) malloc (sizeof(pixelmap_s));
	if (newPixelmap == NULL)
		return NULL;
    //выделение памяти на пиксели
	newPixelmap -> data = (pixels_s *) calloc (width * height, sizeof(pixels_s));
	if (newPixelmap -> data == NULL)
    {
		free (newPixelmap);
		return NULL;
	}
    newPixelmap ->width = width;
    newPixelmap ->height = height;
    return newPixelmap;
}

//Сохранение BMP файла
void save(pixelmap_s *pixelmap_1,const char *name)
{
    FILE *bmp = fopen(name,"wb");
    if (bmp==NULL)
        return;
    uint16_t tmpUint16=0;
    uint32_t tmpUint32=0;
    int32_t tmpInt32=0;
    //BITMAPFILEHEADER 14 байт
    fputc('B', bmp);//0
    fputc('M', bmp);//1
    tmpUint32 = 14 + 40 + pixelmap_1 -> width * pixelmap_1 -> height * 32;
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//2-5 байт-размер файла
	fputc (0, bmp);//6
	fputc (0, bmp);//7
	fputc (0, bmp);//8
	fputc (0, bmp);//9-зарезервированно
	tmpUint32 = 14 + 40;
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//10-13 положение пиксельных данных относительно начала
	//BITMAPFILEINFO 40 байт
    tmpUint32 = 40; //версия 3.0
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//14-17 размер структуры (версия формата)
	tmpInt32 = (int32_t) pixelmap_1 -> width;
	fwrite (&tmpInt32, sizeof(int32_t), 1, bmp);//18-21 ширина изображения
	tmpInt32 = (int32_t) pixelmap_1 -> height;
	fwrite (&tmpInt32, sizeof(int32_t), 1, bmp);//22-25 Высота изображения
	tmpUint16 = 1;
	fwrite (&tmpUint16, sizeof(uint16_t), 1, bmp);//26-27 Значение формата
	tmpUint16 = 32;
	fwrite (&tmpUint16, sizeof(uint16_t), 1, bmp);//28-29 Бит на пиксель
	tmpUint32 = 0;//Без сжатия
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//30-33 Метод компрессии
	tmpUint32 = 0; //Если нет сжатия то 0
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//34-37 Размер пиксельных данных в байтах
	tmpInt32 = 3780;//96 PPI
	fwrite (&tmpInt32, sizeof(int32_t), 1, bmp);//38-41 PPM по горизонтали
	tmpInt32 = 3780;//96 PPI
	fwrite (&tmpInt32, sizeof(int32_t), 1, bmp);//42-45 PPM по вертикали
	tmpUint32 = 0;
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//46-49 Размер таблицы цветов в ячейках
	tmpUint32 = 0;
	fwrite (&tmpUint32, sizeof(uint32_t), 1, bmp);//50-53 Количество ячеек от начала таблицы цветов до последней используемой (включая её саму)
	size_t i = 0;
	for (; i < pixelmap_1 -> width * pixelmap_1 -> height; ++i) {
		fwrite (&(pixelmap_1 -> data[i].b), sizeof(uint8_t), 1, bmp);
		fwrite (&(pixelmap_1 -> data[i].g), sizeof(uint8_t), 1, bmp);
		fwrite (&(pixelmap_1 -> data[i].r), sizeof(uint8_t), 1, bmp);
		fwrite (&(pixelmap_1 -> data[i].a), sizeof(uint8_t), 1, bmp);
	}

}
//Считывание pixelmap из .bmp
pixelmap_s * load (const char * filename) {

	/*Открываем файл;*/
	FILE * bmp = fopen (filename, "rb");
	if (bmp == NULL)
		return NULL;

	uint32_t pixelDataOffset = 0;
	int32_t imgWidth = 0, imgHeight = 0;
	uint16_t bitsPerPixel = 0;

	//Получаем смещение пиксельных данных
	fseek (bmp, 10, SEEK_SET);
	fread (&pixelDataOffset, sizeof(uint32_t), 1, bmp);

	//Получаем размеры изображения
	fseek (bmp, 18, SEEK_SET);
	fread (&imgWidth, sizeof(int32_t), 1, bmp);
	fread (&imgHeight, sizeof(int32_t), 1, bmp);

	//Количество бит на пиксель
	fseek (bmp, 28, SEEK_SET);
	fread (&bitsPerPixel, sizeof(uint16_t), 1, bmp);
	//Получаем pixelmap
	pixelmap_s * pixelmap_1 = createPixelmap (imgWidth, imgHeight);
	if (pixelmap_1 == NULL) {
		fclose (bmp);
		return NULL;
	}
	fseek (bmp, pixelDataOffset, SEEK_SET);
	size_t i = 0;
	for (; i < pixelmap_1 -> width * pixelmap_1 -> height; ++i) {
		fread (&(pixelmap_1 -> data[i].b), sizeof(uint8_t), 1, bmp);
		fread (&(pixelmap_1 -> data[i].g), sizeof(uint8_t), 1, bmp);
		fread (&(pixelmap_1 -> data[i].r), sizeof(uint8_t), 1, bmp);
		if (bitsPerPixel > 24)
			fread (&(pixelmap_1 -> data[i].a), sizeof(uint8_t), 1, bmp);
	}
	fclose (bmp);
	return pixelmap_1;
}
#endif // HEADER_H
