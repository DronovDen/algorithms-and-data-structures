#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct stack
{
    int mas[10000];
    int size;
} stack;

void Init(struct stack *st)
{
    st->size = 0;
}

bool IsOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool GoodSymbol(char c)
{
    return IsOperator(c) || IsDigit(c) || c == '\n';
}

int Priority(char c)
{
    if (c == '(' || c == ')')
        return 1;
    if (c == '+' || c == '-')
        return 2;
    if (c == '*' || c == '/')
        return 3;
    else
        return 0;
}

int Pop(stack *st)
{
    if (st->size == 0)
    {
        printf("syntax error");
        exit(0);
    }
    st->size--;
    return st->mas[st->size];
}

void Push(stack *st, const int val)
{
    st->mas[st->size] = val;
    st->size++;
}

int GetTop(stack *st)
{
    if (st->size <= 0)
    {
        printf("syntax error");
        exit(0);
    }
    return st->mas[st->size - 1];
}

void ToPolish(char *str, char *polish)
{
    stack operators;
    Init(&operators);
    int i = 0;
    int psize = 0;
    while (str[i] != '\0')
    {
        if (IsDigit(str[i]))
        {
            while (!IsOperator(str[i]))
            {
                polish[psize] = str[i];
                psize++;
                i++;
                if (str[i] == '\0')
                    break;
            }
            polish[psize] = ' ';
            psize++;
        }

        if (IsOperator(str[i]))
        {
            if (str[i] == '(')
            {
                Push(&operators, (int)str[i]);
                i++;
            }
            else if (str[i] == ')')
            {
                if (i == 0 || str[i - 1] == '(')
                {
                    printf("syntax error");
                    exit(0);
                }
                char tmp = (char)Pop(&operators);
                while (tmp != '(')
                {
                    polish[psize] = tmp;
                    polish[psize + 1] = ' ';
                    psize += 2;
                    tmp = (char)Pop(&operators);
                }
                i++;
            }
            else
            {
                if (operators.size > 0 && Priority((char)GetTop(&operators)) >= Priority(str[i]))
                {
                    polish[psize] = (char)Pop(&operators);
                    polish[psize + 1] = ' ';
                    psize += 2;
                }
                Push(&operators, (int)str[i]);
                i++;
            }
        }
    }

    while (operators.size > 0)
    {
        polish[psize] = (char)Pop(&operators);
        polish[psize + 1] = ' ';
        psize += 2;
    }
    polish[psize] = '\0';
}

int Count(char *polish)
{
    stack operands;
    Init(&operands);
    int k = 0;
    while (polish[k] != '\0')
    {
        if (polish[k] == ' ')
        {
            k++;
            continue;
        }
        if (IsDigit(polish[k]))
        {
            int p = 0;
            while (!IsOperator(polish[k]) && polish[k] != ' ')
            {
                p *= 10;
                p += (int)(polish[k] - '0');
                k++;
                if (polish[k] == '\0')
                    break;
            }
            Push(&operands, p);
            k++;
        }
        else if (IsOperator(polish[k]))
        {
            int a = (int)Pop(&operands);
            int b = (int)Pop(&operands);
            int res = 0;
            if (polish[k] == '+')
            {
                res = b + a;
            }
            if (polish[k] == '-')
            {
                res = b - a;
            }
            if (polish[k] == '*')
            {
                res = b * a;
            }
            if (polish[k] == '/')
            {
                if (a == 0)
                {
                    printf("division by zero");
                    exit(0);
                }
                res = b / a;
            }
            Push(&operands, res);
            k++;
        }
    }
    return Pop(&operands);
}

int main(void)
{

    char str[2500];
    if (!scanf("%1001[^\n]s", str))
    {
        printf("syntax error");
        return 0;
    }

    int k = 0;
    while (str[k] != '\0')
    {
        if (!GoodSymbol(str[k]))
        {
            printf("syntax error");
            return 0;
        }
        k++;
    }

    char polish[2500];
    ToPolish(str, polish);
    printf("%d", Count(polish));
    return EXIT_SUCCESS;
}
