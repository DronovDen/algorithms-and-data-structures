#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define INT_MAX 2147483647

typedef struct edge
{
    int from;
    int to;
    int weight;
} edge;

void MemoryCleaning(edge *edges, int *frame, int *parent, int *rank)
{
    free(edges);
    free(frame);
    free(parent);
    free(rank);
}

void int_swap(int *a, int *b)
{
    int tmp = *b;
    *b = *a;
    *a = tmp;
}

int Partition(edge *a, int l, int r)
{
    edge v = a[(l + r) / 2];
    int left = l;
    int right = r;
    while (left <= right)
    {
        while (a[left].weight < v.weight)
        {
            left++;
        }
        while (a[right].weight > v.weight)
        {
            right--;
        }
        if (left >= right)
        {
            break;
        }
        // swap
        edge tmp = a[left];
        a[left] = a[right];
        a[right] = tmp;
        left++;
        right--;
    }
    return right;
}

void QuickSort(edge *a, int l, int r)
{
    if (l < r)
    {
        int q = Partition(a, l, r);
        QuickSort(a, l, q);
        QuickSort(a, q + 1, r);
    }
}

void MakeSet(int *parent, int *rank, int v)
{
    for (int i = 0; i < v; i++)
    {
        parent[i] = i;
        rank[i] = 0;
    }
}

int FindSet(int *parent, int v)
{
    if (v == parent[v])
        return v;
    return parent[v] = FindSet(parent, parent[v]);
}

void UnionSets(int a, int b, int *parent, int *rank)
{
    a = FindSet(parent, a);
    b = FindSet(parent, b);
    if (a == b)
        return;
    if (rank[a] < rank[b])
        int_swap(&a, &b);
    parent[b] = a;
    if (rank[a] == rank[b])
        ++rank[a];
}

void Kruskal(int *parent, edge *edges, int *frame, int *rank, int m, int *position)
{
    for (int i = 0; i < m; i++)
    {
        if (FindSet(parent, edges[i].from) == FindSet(parent, edges[i].to))
            continue;
        UnionSets(edges[i].from, edges[i].to, parent, rank);
        frame[*position] = i;
        (*position)++;
    }
}

int SingleRootCheck(int *parent, int n)
{
    int root = parent[0];
    for (int i = 0; i < n; i++)
    {
        if (root != FindSet(parent, i))
        {
            printf("no spanning tree");
            return 0;
        }
    }
    return 1;
}

int CorrectData(int n, int m)
{
    if (n < 0 || n > 5000)
    {
        printf("bad number of vertices");
        return 0;
    }

    if (m < 0 || m > n * (n + 1) / 2)
    {
        printf("bad number of edges");
        return 0;
    }

    if (n == 0 || (m == 0 && n != 1))
    {
        printf("no spanning tree");
        return 0;
    }
    if (n == 1)
        return 0;
    if (m < n - 1)
    {
        printf("no spanning tree");
        return 0;
    }
    return 1;
}

int CheckMemoryAllocation(edge *edges, int *parent, int *frame, int *rank)
{
    if (edges == NULL || parent == NULL || rank == NULL || frame == NULL)
    {
        printf("mem error");
        return 0;
    }
    return 1;
}

int ScanEdges(FILE *fin, edge *edges, int n, int m)
{
    for (int i = 0; i < m; ++i)
    {
        if (fscanf(fin, "%d %d %d\n", &edges[i].from, &edges[i].to, &edges[i].weight) == EOF)
        {
            printf("bad number of lines");
            return 0;
        }
        if (edges[i].from < 1 || edges[i].from > n || edges[i].to < 1 || edges[i].to > n)
        {
            printf("bad vertex");
            return 0;
        }
        if (edges[i].weight < 0 || edges[i].weight > INT_MAX)
        {
            printf("bad length");
            return 0;
        }
        edges[i].from--;
        edges[i].to--;
    }
    return 1;
}

void PrintResult(edge *edges, int *frame, int position)
{
    for (int i = 0; i < position; i++)
    {
        printf("%d %d\n", edges[frame[i]].from + 1, edges[frame[i]].to + 1);
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
    if (!CorrectData(n, m))
    {
        fclose(fin);
        return EXIT_SUCCESS;
    }

    edge *edges = (edge *)malloc(sizeof(edge) * m);
    int *frame = (int *)malloc(sizeof(int) * m);
    int *parent = (int *)malloc(sizeof(int) * n);
    int *rank = (int *)malloc(sizeof(int) * n);

    if (!CheckMemoryAllocation(edges, parent, frame, rank))
    {
        MemoryCleaning(edges, frame, parent, rank);
        fclose(fin);
        return EXIT_SUCCESS;
    }
    if (!ScanEdges(fin, edges, n, m))
    {
        MemoryCleaning(edges, frame, parent, rank);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    QuickSort(edges, 0, m - 1);
    MakeSet(parent, rank, n);

    int position = 0;
    Kruskal(parent, edges, frame, rank, m, &position);

    if (!SingleRootCheck(parent, n))
    {
        MemoryCleaning(edges, frame, parent, rank);
        fclose(fin);
        return EXIT_SUCCESS;
    }

    PrintResult(edges, frame, position);

    MemoryCleaning(edges, frame, parent, rank);
    fclose(fin);
    return EXIT_SUCCESS;
}
