#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

bool NextPermutation(int array[], int length)
{
    int flag = 1;
    for (int k = 0; k < length - 1; ++k)
        if (array[k] < array[k + 1])
        {
            flag = 0;
            break;
        }
    if (flag)
    {
        return false;
    }
    int i = length - 1;
    while (i > 0 && array[i - 1] >= array[i])
        i--;
    int j = length - 1;
    while (array[j] <= array[i - 1])
        j--;
    swap(&array[i - 1], &array[j]);

    j = length - 1;
    while (i < j)
    {
        swap(&array[i], &array[j]);
        i++;
        j--;
    }
    return true;
}

int main(void)
{
    char sequency[35];
    int len = 0;
    if (scanf("%c", &sequency[len]) == '\n')
    {
        return EXIT_SUCCESS;
    }
    len++;
    while (sequency[len - 1] != '\n')
    {
        if (!scanf("%c", &sequency[len]))
            return EXIT_SUCCESS;
        len++;
    }
    sequency[len - 1] = '\0';
    len--;

    int *mas = (int *)malloc(len * sizeof(int));
    int check[10] = {0};
    for (int i = 0; i < len; ++i)
    {
        if (sequency[i] < '0' || sequency[i] > '9')
        {
            printf("bad input");
            free(mas);
            return EXIT_SUCCESS;
        }
        if (check[sequency[i] - '0'])
        {
            printf("bad input");
            free(mas);
            return EXIT_SUCCESS;
        }
        mas[i] = sequency[i] - '0';
        check[mas[i]] = 1;
    }
    int N;
    if (!scanf("%d", &N))
    {
        free(mas);
        return EXIT_SUCCESS;
    }
    for (int k = 0; k < N; ++k)
    {
        if (!NextPermutation(mas, len))
            return 0;
        for (int i = 0; i < len; ++i)
            printf("%d", mas[i]);
        printf("\n");
    }
    free(mas);
    return EXIT_SUCCESS;
}
