#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define WHITE 0
#define GREY 1
#define BLACK 2

int DepthFirstSearch(char *graph, char *used, int v, int n, short *output, int *position)
{
    if (used[v] == BLACK)
        return 0;
    if (used[v] == GREY)
    {
        printf("impossible to sort");
        return 1;
    }
    used[v] = 1;
    for (int i = n - 1; i >= 0; i--)
    {
        if (graph[n * v + i])
            if (DepthFirstSearch(graph, used, i, n, output, position))
                return 1;
    }
    output[*position] = v;
    used[v] = BLACK;
    (*position)++;
    return 0;
}

int CheckCorrectData(int n, int m, FILE *fin)
{
    if (n < 0 || n > 2000)
    {
        printf("bad number of vertices");
        fclose(fin);
        return 1;
    }

    if (m < 0 || m > n * (n + 1) / 2)
    {
        printf("bad number of edges");
        fclose(fin);
        return 1;
    }
    return 0;
}

int ScanEdges(char *graph, char *used, int n, int m, FILE *fin)
{
    for (int i = 0; i < m; i++)
    {
        int from, to;
        if (fscanf(fin, "%d %d", &from, &to) == EOF)
        {
            printf("bad number of lines");
            fclose(fin);
            free(graph);
            free(used);
            return 1;
        }

        if (from < 1 || from > n || to < 1 || to > n)
        {
            printf("bad vertex");
            fclose(fin);
            free(graph);
            free(used);
            return 1;
        }
        graph[n * (--from) + (--to)] = 1;
    }
    return 0;
}

int main(void)
{
    FILE *fin = fopen("in.txt", "rt");
    if (fin == NULL)
    {
        return EXIT_SUCCESS;
    }

    int n, m;
    if (fscanf(fin, "%d", &n) == EOF)
    {
        printf("bad number of lines");
        fclose(fin);
        return EXIT_SUCCESS;
    }

    if (fscanf(fin, "%d", &m) == EOF)
    {
        printf("bad number of lines");
        fclose(fin);
        return EXIT_SUCCESS;
    }

    if (CheckCorrectData(n, m, fin))
        return EXIT_SUCCESS;

    char *used = (char *)calloc(n, sizeof(char));
    if (used == NULL)
    {
        printf("mem error");
        fclose(fin);
        return EXIT_SUCCESS;
    }

    char *graph = (char *)calloc(n * n, sizeof(char));
    if (graph == NULL)
    {
        printf("mem error");
        fclose(fin);
        free(used);
        return EXIT_SUCCESS;
    }

    if (ScanEdges(graph, used, n, m, fin))
        return EXIT_SUCCESS;

    short *output = (short *)malloc(sizeof(short) * n);
    if (output == NULL)
    {
        printf("mem error");
        fclose(fin);
        free(used);
        free(graph);
        return EXIT_SUCCESS;
    }

    int cur_out = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!used[i])
        {
            if (DepthFirstSearch(graph, used, i, n, output, &cur_out))
            {
                free(graph);
                free(used);
                free(output);
                fclose(fin);
                return EXIT_SUCCESS;
            }
        }
    }

    for (int i = cur_out - 1; i >= 0; --i)
    {
        printf("%d ", output[i] + 1);
    }

    fclose(fin);
    free(graph);
    free(used);
    free(output);
    return EXIT_SUCCESS;
}
