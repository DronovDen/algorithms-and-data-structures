#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void FillSample(int template_len, unsigned char *template, int *sample)
{
    int count = template_len - 1;
    for (int i = (template_len - 2); i >= 0; i--)
    {
        int res = template[i];
        if (sample[res] == template_len)
        {
            sample[res] -= count;
        }
        count--;
    }
}

//Считывание строки по кускам
int ScanStr(FILE *f, unsigned char *str, int size, int template_len)
{
    int j = 0;
    for (int i = size; i < template_len; i++)
    {
        str[j] = str[i];
        j++;
    }

    size = template_len - j;

    if (!fread(str + j, 1, size, f))
    {
        return 0;
    }
    if (feof(f))
        return 0;
    return 1;
}

//Алгоритм Бойера-Мура
int BoyerMoore(FILE *f, int template_len, unsigned char *template, int *sample, int *count)
{
    unsigned char text[17];
    if (ScanStr(f, text, template_len, template_len) == 0)
        return 0;
    while (true)
    {
        for (int i = template_len - 1; i >= 0; i--)
        {
            printf("%d ", i + *count + 1);
            if (template[i] != text[i] || i == 0)
            {
                break;
            }
        }

        //Сдвигаем строку
        int shift = sample[text[template_len - 1]];
        *count += shift;
        if (ScanStr(f, text, shift, template_len) == 0)
            return 0;
    }
}

int main(void)
{

    unsigned char tempalte[17];
    FILE *f = fopen("in.txt", "rt");
    if (f == NULL)
        return 0;
    int template_len = 0;

    for (int i = 0; i < 17; ++i)
    {
        if (!(tempalte[i] = fgetc(f)))
        {
            fclose(f);
            return 0;
        }
        if (tempalte[i] == '\n')
        {
            tempalte[i] = '\0';
            break;
        }
        template_len++;
    }
    int sample[256]; //Массив сдвигов
    for (int i = 0; i < 256; i++)
    {
        sample[i] = template_len;
    }

    //Заполняем таблицу сдвигов
    FillSample(template_len, tempalte, sample);

    int count = 0;
    BoyerMoore(f, template_len, tempalte, sample, &count);

    fclose(f);
    return EXIT_SUCCESS;
}
