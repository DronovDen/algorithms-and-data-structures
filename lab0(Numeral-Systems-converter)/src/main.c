#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int invalidBase(int base1, int base2)
{
    if (base1 > 16 || base1 < 2 || base2 > 16 || base2 < 2)
    {
        printf("Invalid base of the number system!");
        return 1;
    }
    return 0;
}

int invalidNumString(char *numString, int *numLen, bool *dotPresent, int *dotPosition, int base1)
{
    for (int i = 0; numString[i] != '\0'; i++)
    {
        (*numLen)++;
        numString[i] = tolower(numString[i]);
        if (numString[i] == '.')
        {
            if (*dotPresent)
            {
                printf("More than one dots!");
                return 1;
            }
            *dotPosition = i;
            *dotPresent = true;
            continue;
        }
        if (numString[i] - '0' < 10 && (numString[i] - '0' > base1 - 1 || numString[i] - '0' < 0))
        {
            printf("bad input");
            return 1;
        }
        if (numString[i] - '0' > 10 && (numString[i] - 'a' + 10 > base1 - 1 || numString[i] - 'a' + 10 < 10))
        {
            printf("bad input");
            return 1;
        }
    }

    if (*dotPosition == (*numLen) - 1 || *dotPosition == 0)
    {
        printf("bad input");
        return 1;
    }

    return 0;
}

void intPartToDecimal(int *dotPosition, char *numString, unsigned long long *beforeDot, double *powBase1, int base1)
{
    for (int i = *dotPosition - 1; i >= 0; i--)
    {
        if (numString[i] - '0' < 10)
        {
            *beforeDot += (numString[i] - '0') * (unsigned long long)(*powBase1);
        }
        else
        {
            *beforeDot += (numString[i] - 'a' + 10) * (unsigned long long)(*powBase1);
        }
        (*powBase1) *= base1;
    }
}

void floatPartToDecimal(int *dotPosition, char *numString, double *afterDot, int numLen, double *powBase1, int base1)
{
    for (int i = (*dotPosition) + 1; i < numLen; i++)
    {
        if (numString[i] - '0' < 10)
        {
            (*afterDot) += (numString[i] - '0') * (*powBase1);
        }
        else
        {
            (*afterDot) += (numString[i] - 'a' + 10) * (*powBase1);
        }
        (*powBase1) /= base1;
    }
}

void intPartFromDecimal(int beforeDot, int *numLen, int *tmp, int base2, char *result)
{
    if (beforeDot == 0)
    {
        tmp[0] = 0;
        (*numLen) = 1;
    }
    for (int i = 0; beforeDot > 0; i++)
    {
        tmp[i] = beforeDot % base2;
        (*numLen)++;
        beforeDot /= base2;
    }

    for (int i = 0; i < (*numLen); i++)
    {
        if (tmp[*numLen - i - 1] < 10)
        {
            result[i] = tmp[(*numLen) - i - 1] + '0';
        }
        else
        {
            result[i] = tmp[(*numLen) - i - 1] + 'a' - 10;
        }
    }
}

void floatPartFromDecimal(double *afterDot, int numLen, int base2, int *tmp, char *result)
{
    for (int i = numLen + 1; i < numLen + 13; i++)
    {
        (*afterDot) *= base2;
        tmp[i] = (int)(*afterDot);
        afterDot -= (int)(*afterDot);
        if (tmp[i] < 10)
        {
            result[i] = tmp[i] + '0';
        }
        else
        {
            result[i] = tmp[i] + 'a' - 10;
        }
    }
}

int main()
{
    int base1, base2;
    char numString[14];

    if (!(scanf("%d %d", &base1, &base2)))
        return 0;
    if (!(scanf("%13s", numString)))
        return 0;

    if (invalidBase(base1, base2))
        return 0;

    int dotPosition = -1;
    int numLen = 0;
    bool dotPresent = false;

    if (invalidNumString(numString, &numLen, &dotPresent, &dotPosition, base1))
        return 0;

    if (dotPosition == -1)
    {
        dotPosition = numLen;
        dotPresent = false;
    }

    //Перевод из base1-ичной сс 10-ую целой части
    unsigned long long beforeDot = 0;
    double powBase1 = 1;
    intPartToDecimal(&dotPosition, numString, &beforeDot, &powBase1, base1);

    //Перевод из base1-ичной сс в 10-ую дробной части
    double afterDot = 0.0;
    powBase1 = 1.0 / base1;

    floatPartToDecimal(&dotPosition, numString, &afterDot, numLen, &powBase1, base1);

    //Перевод из 10-ой сс в base2-ичную целой части
    numLen = 0;
    int tmp[50];
    char result[50];
    intPartFromDecimal(beforeDot, &numLen, tmp, base2, result);

    //Если нет дробной части, то выводим число
    if (afterDot == 0.0 || dotPresent == false)
    {
        result[numLen] = '\0';
        printf("%s", result);
        return 0;
    }

    //Если дробная часть есть, ставим точку
    result[numLen] = '.';

    //Перевод в base2-ичную сс дробной части
    floatPartFromDecimal(&afterDot, numLen, base2, tmp, result);

    result[numLen + 13] = '\0';
    printf("%s", result);
    return 0;
}
