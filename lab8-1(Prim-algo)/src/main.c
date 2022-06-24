#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 2147483648
#define uint unsigned int

typedef enum Errors
{
    NO_ERRORS = 0,
    BAD_NUMBER_OF_LINES = 1,
    BAD_VERTEX = 2,
    BAD_LENGTH = 3,
    BAD_NUMBER_OF_VERTICES = 4,
    BAD_NUMBER_OF_EDGES = 5,
    NO_SPANNING_TREE = 6,
    ONE_VERTEX = 7,
    MEMORY_ALLOCATION = 8
} Errors;

void PrintErrorMessage(Errors error)
{
    switch (error)
    {
    case BAD_NUMBER_OF_LINES:
        printf("bad number of lines");
        break;
    case BAD_VERTEX:
        printf("bad vertex");
        break;
    case BAD_LENGTH:
        printf("bad length");
        break;
    case BAD_NUMBER_OF_VERTICES:
        printf("bad number of vertices");
        break;
    case BAD_NUMBER_OF_EDGES:
        printf("bad number of edges");
        break;
    case NO_SPANNING_TREE:
        printf("no spanning tree");
        break;
    case MEMORY_ALLOCATION:
        printf("memory allocation error");
        break;
    case ONE_VERTEX:
    case NO_ERRORS:
        break;
    }
}

Errors PrimAlgorithm(uint *matrix, char *used, short *answer, int n)
{
    uint *minEdge = (uint *)malloc(n * sizeof(uint));
    short *endOfMinEdge = (short *)malloc(n * sizeof(short));
    memset(used, 0, n);
    memset(endOfMinEdge, -1, n * sizeof(short));
    for (int i = 0; i < n; i++)
    {
        minEdge[i] = INF;
    }
    minEdge[0] = 0;
    for (int i = 0, iterator = 0; i < n; i++)
    {
        int v = -1;
        //Поиск минимального ребра выходящего из данной, не включенной в остов
        //вершины
        for (int j = 0; j < n; j++)
        {
            if (!used[j] && (v == -1 || minEdge[j] < minEdge[v]))
                v = j;
        }
        if (minEdge[v] == INF)
        {
            free(minEdge);
            free(endOfMinEdge);
            return NO_SPANNING_TREE;
        }
        used[v] = 1;
        //Запись ребра в массив answer
        if (endOfMinEdge[v] != -1)
        {
            answer[iterator] = v;
            answer[(n - 1) + (iterator++)] = endOfMinEdge[v];
        }
        //Пересчет минимальных длин ребер, ведущих из концов добавленного ребра
        for (int to = 0; to < n; to++)
        {
            if (matrix[n * v + to] < minEdge[to])
            {
                minEdge[to] = matrix[n * v + to];
                endOfMinEdge[to] = v;
            }
        }
    }
    free(minEdge);
    free(endOfMinEdge);
    return NO_ERRORS;
}

Errors ReadGraph(FILE *fin, uint *matrix, int n, int m)
{
    for (int i = 0; i < m; i++)
    {
        int from, to;
        int weight;
        if (fscanf(fin, "%d %d %d\n", &from, &to, &weight) == EOF)
        {
            return BAD_NUMBER_OF_LINES;
        }
        if (from < 1 || from > n || to < 1 || to > n)
        {
            return BAD_VERTEX;
        }
        if (weight < 0 || weight > INT_MAX)
        {
            return BAD_LENGTH;
        }
        matrix[n * (from - 1) + (to - 1)] = weight;
        matrix[n * (to - 1) + (from - 1)] = weight;
    }
    return NO_ERRORS;
}

void MatrixInitialization(uint *matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i * n + j] = INF;
        }
    }
}

Errors CorrectData(int n, int m)
{
    if (n < 0 || n > 5000)
    {
        return BAD_NUMBER_OF_VERTICES;
    }

    if (m < 0 || m > n * (n + 1) / 2)
    {
        return BAD_NUMBER_OF_EDGES;
    }

    if (n == 0 || (m == 0 && n != 1) || m < n - 1)
    {
        return NO_SPANNING_TREE;
    }
    if (n == 1)
        return ONE_VERTEX;
    return NO_ERRORS;
}

Errors CheckMemoryAllocation(uint *matrix, char *used, short *answer)
{
    if (matrix == NULL || used == NULL || answer == NULL)
    {
        return MEMORY_ALLOCATION;
    }
    return NO_ERRORS;
}

void MemoryCleaning(uint *matrix, char *used, short *answer)
{
    free(matrix);
    free(used);
    free(answer);
}

void PrintResult(short *answer, int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        printf("%d %d\n", answer[i] + 1, answer[(n - 1) + i] + 1);
    }
}

int main(void)
{
    FILE *fin = fopen("in.txt", "r");
    if (fin == NULL)
    {
        return EXIT_SUCCESS;
    }

    int n, m;
    if (fscanf(fin, "%d\n%d\n", &n, &m) == EOF)
    {
        fclose(fin);
        return EXIT_SUCCESS;
    }

    Errors error = CorrectData(n, m);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    uint *matrix = (uint *)malloc(n * n * sizeof(uint));
    char *used = (char *)malloc(n * sizeof(short));
    short *answer = (short *)malloc((n - 1) * 2 * sizeof(short));

    error = CheckMemoryAllocation(matrix, used, answer);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        MemoryCleaning(matrix, used, answer);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    MatrixInitialization(matrix, n);

    error = ReadGraph(fin, matrix, n, m);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        MemoryCleaning(matrix, used, answer);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    error = PrimAlgorithm(matrix, used, answer, n);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        MemoryCleaning(matrix, used, answer);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    PrintResult(answer, n);
    MemoryCleaning(matrix, used, answer);
    fclose(fin);
    return EXIT_SUCCESS;
}
