#include <stdio.h>
#include <stdlib.h>

main()
{
    FILE *input=fopen("input.txt","r+");
    if (input==NULL)
        perror("");
    fseek(input,0,SEEK_END);
    long size = ftell(input);
    if (size==0)
        {
            printf("File is empty.");
            exit(1);
        }
    rewind (input);
    char *string=(char*)calloc(size,sizeof(char));
    if (string==NULL)
    {
        printf("Memmory error");
        exit(2);
    }
    fread(string,sizeof(char),size,input);
    fseek(input,0,SEEK_SET);
    FILE *output=fopen("output.txt","w");
    if (output==NULL)
        perror("");
    char c;
    int n=0;
    while(!feof(input))
    {
        if (c!=','&&c!='\0'&&c!='\n')
        {
            c=getc(input);
            n++;
        }
        else
        {
            fseek(input,-n,SEEK_CUR);
            char *word=(char*)calloc(n,sizeof(char));
            fread(word,sizeof(char),n,input);
            if (strstr(word,string)==NULL)
                fwrite(word,sizeof(char),n,output);
            free(word);
            n=0;
        }
    }
    fclose(input);
    fclose(output);
    printf("Done.");
    getc(stdin);
    return 0;
}
