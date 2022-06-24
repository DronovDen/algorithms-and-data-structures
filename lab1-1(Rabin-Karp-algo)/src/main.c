#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ScanTemplate(FILE *f, unsigned char *template, int *len)
{
    for (int i = 0; i < 17; ++i)
    {
        if (!(template[i] = fgetc(f)))
            return 0;
        if (template[i] == '\n')
        {
            template[i] = '\0';
            break;
        }
        ++*len;
    }
    return 1;
}

int ScanNext(FILE *f, unsigned char *text, int len)
{
    if (!fread(text, 1, len, f))
    {
        return 0;
    }
    if (feof(f))
        return 0;
    return 1;
}

int NextHash(FILE *f, int *text_hash, unsigned char *text, int subsize, int size)
{
    *text_hash /= 3;
    for (int i = 1; i < subsize; ++i)
        text[i - 1] = text[i];
    if (!(text[subsize - 1] = fgetc(f)))
        return 0;
    *text_hash += text[subsize - 1] % 3 * size;
    if (feof(f))
        return 0;
    return 1;
}

int Hash(unsigned char *mas, int subsize, int *p)
{
    int h = 0;
    for (int i = 0; i < subsize; ++i)
    {
        h += (mas[i] % 3) * (*p);
        *p *= 3;
    }
    *p /= 3;
    return h;
}

void Comparison(unsigned char *template, unsigned char *text, int size, int count)
{
    for (int i = 0; i < size; ++i)
        if (template[i] == text[i])
            printf("%d ", count + i + 1);
        else
        {
            printf("%d ", count + i + 1);
            return;
        }
}

void RabinCarp(FILE *f, unsigned char *template, int subsize)
{
    int p = 1;
    int subhash = Hash(template, subsize, &p);
    printf("%d ", subhash);
    unsigned char text[17];
    if (!ScanNext(f, text, subsize))
        return;
    int s = 1;
    int text_hash = Hash(text, subsize, &s);
    int count = 0;
    while (1)
    {
        if (text_hash == subhash)
            Comparison(template, text, subsize, count);
        if (!NextHash(f, &text_hash, text, subsize, p))
        {
            return;
        }
        ++count;
    }
}

int main(void)
{
    FILE *f = fopen("in.txt", "rt");
    if (f == NULL)
        return 0;
    unsigned char template[17];
    int subsize = 0;
    if (!ScanTemplate(f, template, &subsize))
    {
        fclose(f);
        return 0;
    }
    RabinCarp(f, template, subsize);
    fclose(f);
    return EXIT_SUCCESS;
}
