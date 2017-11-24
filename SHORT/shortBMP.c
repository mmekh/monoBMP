#include <stdio.h>
 main(int argc,char* argv[])
 {
     unsigned char name[100],data[54],pixel[4],s=0;
     int j,i;
     FILE *bmp = fopen(argv[1],"rb");
     strcpy(name,argv[1]);
     name[strlen(name)-4]='\0';
     strcat(name,"_grey.bmp");
     FILE *bmp_new = fopen(name,"wb");
     fread(&data,sizeof(char),54,bmp);
	 fwrite(&data,sizeof(char),54,bmp_new);
     do{
        pixel[i%4]=fgetc(bmp);
        if (i%4==3)
            {
            s=(pixel[0]+pixel[1]+pixel[2])/3;
            for (j=0;j<4;j++)
                fputc(s,bmp_new);
            }
        i++;
     }while(feof(bmp)==0);
}
