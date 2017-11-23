#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

int main (int argc, char* argv[]) {
	char name[100];
	if (argv[1]==NULL)
    {
       	printf("Enter file name: ");
       	gets(name);
    }
    else
        strcpy(name,argv[1]);
	printf ("Loading: %s\n",name);
	//Загружаем bmp в pixelmap
	pixelmap_s * pixelmap = load (name);
	size_t i = 0;
	for (; i < pixelmap -> width * pixelmap -> height; ++i)
    {
        uint8_t S;
        S=(pixelmap ->data[i].r*30+pixelmap -> data[i].g*59+pixelmap -> data[i].b*11)/100;
        pixelmap -> data[i].r = S;
        pixelmap -> data[i].g = S;
        pixelmap -> data[i].b = S;
    }
    name[strlen(name) - 4] = '\0';
    strcat(name,"_new.bmp");
	save(pixelmap, name);
	printf ("Done");
	return 0;
}
