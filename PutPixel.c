void PutPixel(int m,int n,FILE *bmp)
{
    while(n>0)
    {
        fputc(m,bmp);
        n--;
    }
}
