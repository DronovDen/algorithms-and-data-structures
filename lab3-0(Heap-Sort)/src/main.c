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

int order(int l, int r, int arr[])
{
    int x = arr[(l + r) / 2];
    int L = l;
    int R = r;
    while (L <= R)
    {
        while (arr[L] < x)
            ++L;
        while (arr[R] > x)
            --R;
        if (L >= R)
        {
            break;
        }
        swap(&arr[L++], &arr[R--]);
    }
    return R;
}

void qsortt(int mas[], int l, int r)
{
    if (l >= r)
        return;
    int c = order(l, r, mas);
    qsortt(mas, l, c);
    qsortt(mas, c + 1, r);
}

int main()
{
    int SizeOfMas;
    if (!(scanf("%d\n", &SizeOfMas)))
    {
        return 0;
    }
    int *m = (int *)malloc(SizeOfMas * sizeof(int));

    for (int j = 0; j < SizeOfMas; j++)
    {
        if (!(scanf("%d", &m[j])))
        {
            free(m);
            return 0;
        }
    }
    qsortt(m, 0, SizeOfMas - 1);

    for (int k = 0; k < SizeOfMas; k++)
        printf("%d ", m[k]);

    free(m);
    return 0;
}
