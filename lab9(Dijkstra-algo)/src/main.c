#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define INT_MAX_PLUS 2147483648
#define uint unsigned int
#define lli long long int

enum Errors
{
    NO_ERRORS = 0,
    BAD_NUMBER_OF_LINES = 1,
    BAD_VERTEX = 2,
    BAD_LENGTH = 3,
    BAD_NUMBER_OF_VERTICES = 4,
    BAD_NUMBER_OF_EDGES = 5,
    NO_PATH = 6,
    OVERFLOW = 7,
    MEMORY_ALLOCATION = 8
};

void PrintErrorMessage(enum Errors error)
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
    case NO_PATH:
        printf("no path");
        break;
    case OVERFLOW:
        printf("overflow");
        break;
    case MEMORY_ALLOCATION:
        printf("memory allocation error");
        break;
    case NO_ERRORS:
        break;
    }
}

void Dijkstra(int *matrix, char *used, uint *dist, short *ways_count,
              short *parents, int vertex, int start)
{
    dist[start] = 0;
    ways_count[start] = 1;
    for (int i = 0; i < vertex; i++)
    {
        int v = -1;
        for (int j = 0; j < vertex; j++)
        {
            if (!used[j] && (v == -1 || dist[j] < dist[v]))
                v = j;
        }
        if (dist[v] == UINT_MAX)
            break;
        used[v] = 1;
        for (int k = 0; k < vertex; k++)
        {
            if (matrix[v * vertex + k] != -1)
            {
                if (ways_count[k] < 3)
                {
                    ways_count[k] += ways_count[v];
                }
                if ((lli)dist[v] + (lli)matrix[v * vertex + k] < dist[k])
                {
                    parents[k] = v;
                    if ((lli)dist[v] + (lli)matrix[v * vertex + k] > INT_MAX)
                        dist[k] = INT_MAX_PLUS;
                    else
                        dist[k] = dist[v] + matrix[v * vertex + k];
                }
            }
        }
    }
}

void PrintWayLength(uint *dist, int vertex)
{
    for (int i = 0; i < vertex; i++)
    {
        if (dist[i] == UINT_MAX)
            printf("oo ");
        else if (dist[i] == INT_MAX_PLUS)
            printf("INT_MAX+ ");
        else
            printf("%u ", dist[i]);
    }
    printf("\n");
}

void PrintWay(uint *dist, int start, int finish, short *parents,
              short *ways_count)
{
    if (dist[finish] == UINT_MAX)
    {
        printf("no path");
        return;
    }
    if (dist[finish] > INT_MAX && ways_count[finish] >= 2)
    {
        printf("overflow");
        return;
    }
    printf("%d ", finish + 1);
    int path = finish;
    while (path != start)
    {
        path = parents[path];
        printf("%d ", path + 1);
    }
}

int ReadGraph(FILE *fin, int *matrix, int vertex, int edge)
{
    for (int i = 0; i < edge; i++)
    {
        int from, to;
        int weight;
        if (fscanf(fin, "%d %d %d\n", &from, &to, &weight) == EOF)
        {
            return BAD_NUMBER_OF_LINES;
        }
        if (from < 1 || from > vertex || to < 1 || to > vertex)
        {
            return BAD_VERTEX;
        }
        if (weight < 0 || weight > INT_MAX)
        {
            return BAD_LENGTH;
        }
        matrix[vertex * (from - 1) + (to - 1)] = weight;
        matrix[vertex * (to - 1) + (from - 1)] = weight;
    }
    return NO_ERRORS;
}

int CheckMemoryAllocation(int *matrix, uint *dist, char *used, short *parents,
                          short *ways_count)
{
    if (matrix == NULL || used == NULL || dist == NULL || parents == NULL ||
        ways_count == NULL)
    {
        return MEMORY_ALLOCATION;
    }
    return NO_ERRORS;
}

void MemoryCleaning(int *matrix, uint *dist, char *used, short *parents,
                    short *ways_count)
{
    free(matrix);
    free(used);
    free(dist);
    free(parents);
    free(ways_count);
}

int CorrectData(int vertex, int start, int finish, int edges)
{
    if (vertex < 0 || vertex > 5000)
    {
        return BAD_NUMBER_OF_VERTICES;
    }

    if (edges < 0 || edges > vertex * (vertex + 1) / 2)
    {
        return BAD_NUMBER_OF_EDGES;
    }

    if (start <= 0 || start > vertex || finish <= 0 || finish > vertex)
    {
        return BAD_VERTEX;
    }

    return NO_ERRORS;
}

void Iinitialization(int *matrix, uint *dist, short *ways_count, int vertex)
{
    for (int i = 0; i < vertex; i++)
    {
        ways_count[i] = 0;
        dist[i] = UINT_MAX;
        for (int j = 0; j < vertex; j++)
        {
            matrix[i * vertex + j] = -1;
        }
    }
}

int main(void)
{
    FILE *fin = fopen("in.txt", "r");
    if (fin == NULL)
    {
        return EXIT_SUCCESS;
    }

    int vertex, start, finish, edges;
    if (fscanf(fin, "%d %d %d %d", &vertex, &start, &finish, &edges) == EOF)
    {
        fclose(fin);
        return EXIT_SUCCESS;
    }

    enum Errors error = CorrectData(vertex, start, finish, edges);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    int *matrix = (int *)malloc(vertex * vertex * sizeof(int));
    uint *dist = (uint *)malloc(vertex * sizeof(uint));
    short *parents = (short *)malloc(2 * vertex * sizeof(short));
    short *ways_count = (short *)malloc(vertex * sizeof(short));
    char *used = (char *)calloc(vertex, sizeof(char));

    Iinitialization(matrix, dist, ways_count, vertex);

    error = CheckMemoryAllocation(matrix, dist, used, parents, ways_count);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        MemoryCleaning(matrix, dist, used, parents, ways_count);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    error = ReadGraph(fin, matrix, vertex, edges);
    if (error != NO_ERRORS)
    {
        PrintErrorMessage(error);
        MemoryCleaning(matrix, dist, used, parents, ways_count);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    Dijkstra(matrix, used, dist, ways_count, parents, vertex, start - 1);
    PrintWayLength(dist, vertex);
    PrintWay(dist, start - 1, finish - 1, parents, ways_count);

    MemoryCleaning(matrix, dist, used, parents, ways_count);
    fclose(fin);
    return EXIT_SUCCESS;
}
