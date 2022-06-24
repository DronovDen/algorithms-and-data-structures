#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void swap(int *a, int *b)
{
    int t;
    t = *a;
    *a = *b;
    *b = t;
}

void sift(int *mas, int len, int i)
{
    while ((2 * i + 1 < len && mas[i] < mas[2 * i + 1]) ||
           (2 * i + 2 < len && mas[i] < mas[2 * i + 2]))
    {
        if (2 * i + 2 >= len)
        {
            swap(&mas[i], &mas[2 * i + 1]);
            i = 2 * i + 1;
        }
        else
        {
            if (mas[2 * i + 1] > mas[2 * i + 2])
            {
                swap(&mas[i], &mas[2 * i + 1]);
                i = 2 * i + 1;
            }
            else
            {
                swap(&mas[i], &mas[2 * i + 2]);
                i = 2 * i + 1;
                ++i;
            }
        }
    }
}

void heapify(int a[], int size)
{
    for (int i = size / 2; i >= 0; --i)
    {
        sift(a, size, i);
    }
}

int main(void)
{
    int Size;
    if (!(scanf("%d\n", &Size)))
    {
        return 0;
    }
    int *m = (int *)malloc(Size * sizeof(int));

    for (int j = 0; j < Size; j++)
    {
        if (!(scanf("%d", &m[j])))
        {
            free(m);
            return 0;
        }
    }

    heapify(m, Size);
    int heapsize = Size;
    for (int i = 0; i < Size - 1; ++i)
    {
        swap(&m[0], &m[Size - 1 - i]);
        heapsize--;
        sift(m, heapsize, 0);
    }

    for (int k = 0; k < Size; k++)
        printf("%d ", m[k]);

    free(m);
    return EXIT_SUCCESS;
}
