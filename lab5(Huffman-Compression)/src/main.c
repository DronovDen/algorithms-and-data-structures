#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SZ 256
typedef unsigned char uchar;
typedef unsigned int uint;

struct Node
{
    int frequency;
    int symbol;
    struct Node *right;
    struct Node *left;
};
typedef struct Node Node;

typedef enum Errors
{
    NO_ERRORS = 0,
    MEMORY_ALLOCATION = 1,
    NO_TREE_ROOT = 2,
    NO_MESSAGE = 3,
} Errors;

void PrintErrorMessage(Errors status)
{
    switch (status)
    {
    case MEMORY_ALLOCATION:
        printf("Memory allocation error!");
        break;
    case NO_TREE_ROOT:
    case NO_MESSAGE:
    case NO_ERRORS:
        break;
    }
}

Node *createNode(Node *left, Node *right, uint frequency, int symbol)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    if (temp == NULL)
        return NULL;
    temp->frequency = frequency;
    temp->symbol = symbol;
    temp->left = left;
    temp->right = right;
    return temp;
}

Node *BuildTree(uint *freq)
{
    Node *nodesArray[MAX_SZ] = {NULL};
    int count = 0;
    for (int i = 0; i < MAX_SZ; ++i)
    {
        if (freq[i])
        {
            nodesArray[count] = createNode(NULL, NULL, freq[i], i);
            if (nodesArray[count] == NULL)
                return NULL;
            count++;
        }
    }
    while (count > 1)
    {
        for (int i = 0; i < count - 1; ++i)
        {
            for (int j = 0; j < count - i - 1; ++j)
            {
                if (nodesArray[j]->frequency < nodesArray[j + 1]->frequency)
                {
                    Node *temp = nodesArray[j];
                    nodesArray[j] = nodesArray[j + 1];
                    nodesArray[j + 1] = temp;
                }
            }
        }
        uint summary_freq =
            nodesArray[count - 2]->frequency + nodesArray[count - 1]->frequency;
        Node *NewNode = createNode(nodesArray[count - 2], nodesArray[count - 1],
                                   summary_freq, 0);
        if (NewNode == NULL)
        {
            return NULL;
        }
        nodesArray[count - 2] = NewNode;
        count--;
    }
    Node *root = nodesArray[0];
    return root;
}

Errors MakeTable(uchar *path, uchar level, uchar **code, Node *root)
{
    Errors status = NO_ERRORS;
    if (root->right || root->left)
    {
        path[level] = '0';
        status = MakeTable(path, level + 1, code, root->left);
        if (status != NO_ERRORS)
            return status;
        path[level] = '1';
        status = MakeTable(path, level + 1, code, root->right);
        if (status != NO_ERRORS)
            return status;
    }
    else
    {
        code[root->symbol] = (uchar *)malloc((level + 1) * sizeof(uchar));
        if (code[root->symbol] == NULL)
        {
            return MEMORY_ALLOCATION;
        }
        path[level] = '\0';
        memcpy(code[root->symbol], path, level + 1);
    }
    return NO_ERRORS;
}

void DeleteTree(Node *root)
{
    if (root->left != NULL)
        DeleteTree(root->left);
    if (root->right != NULL)
        DeleteTree(root->right);
    if (root != NULL)
        free(root);
}

void writebit(FILE *fout, uint *bitpos, uchar bit, uchar *buffer)
{
    if ((*bitpos) == 8)
    {
        fwrite(buffer, sizeof(uchar), 1, fout);
        *buffer = 0;
        *bitpos = 0;
    }
    (*buffer) += bit << (7 - (*bitpos));
    (*bitpos)++;
}

void writebyte(FILE *fout, uint *bitpos, uchar byte, uchar *buffer)
{
    for (int i = 0; i < 8; i++)
    {
        writebit(fout, bitpos, byte >> (7 - i) & 1, buffer);
    }
}

void EncodeMessage(FILE *fin, FILE *fout, uchar *buffer, uint *bitpos,
                   int length, uchar **codeTable)
{
    for (int i = 0; i < length; i++)
    {
        uchar symbol = fgetc(fin);
        uchar *sym_code = codeTable[symbol];
        for (int j = 0; sym_code[j] != '\0'; j++)
        {
            writebit(fout, bitpos, sym_code[j] - 48, buffer);
        }
    }
    if (bitpos != 0)
        fwrite(buffer, sizeof(char), 1, fout);
}

void SaveTree(FILE *fout, Node *root, uint *bitpos, uchar *buffer)
{
    if (root)
    {
        if ((!root->left) && (!root->right))
        {
            writebit(fout, bitpos, 1, buffer);
            writebyte(fout, bitpos, root->symbol, buffer);
        }
        else
        {
            writebit(fout, bitpos, 0, buffer);
            SaveTree(fout, root->right, bitpos, buffer);
            SaveTree(fout, root->left, bitpos, buffer);
        }
    }
}

Errors encode(FILE *fin, FILE *fout)
{
    uint *frequency = (uint *)malloc(MAX_SZ * sizeof(uint));
    if (frequency == NULL)
        return MEMORY_ALLOCATION;
    memset(frequency, 0, MAX_SZ * sizeof(uint));

    fseek(fin, 3, 0);
    int symbol;
    uint message_len = 0;
    while ((symbol = fgetc(fin)) != EOF)
    {
        message_len++;
        frequency[symbol]++;
    }

    Node *codeTree = BuildTree(frequency);
    if (codeTree == NULL)
    {
        free(frequency);
        return NO_TREE_ROOT;
    }

    uchar *codeTable[MAX_SZ] = {0};
    uchar path[MAX_SZ] = {0};
    Errors status = MakeTable(path, 0, codeTable, codeTree);
    if (status != NO_ERRORS)
    {
        free(frequency);
        DeleteTree(codeTree);
        return status;
    }

    uchar buffer = 0;
    uint bitpos = 0;
    fwrite(&message_len, sizeof(uint), 1, fout);
    SaveTree(fout, codeTree, &bitpos, &buffer);
    fseek(fin, 3, 0);
    EncodeMessage(fin, fout, &buffer, &bitpos, message_len, codeTable);

    free(frequency);
    DeleteTree(codeTree);
    return NO_ERRORS;
}

void ReadByte(FILE *fin, uchar *buf, uint *bitpos)
{
    if (fread(buf, sizeof(uchar), 1, fin) != 1)
        return;
    *bitpos = 0;
}

uchar ReadBit(FILE *fin, uchar *buf, uint *bitpos)
{
    if (*bitpos == 8)
        ReadByte(fin, buf, bitpos);
    (*bitpos)++;
    return (*buf >> (8 - *bitpos)) & 1;
}

uchar ReadSymb(FILE *fin, uchar *buf, uint *bitpos)
{
    uchar symb = 0;
    for (int i = 0; i < 8; i++)
        symb |= ReadBit(fin, buf, bitpos) << (7 - i);
    return symb;
}

Node *ReadTree(FILE *fin, uchar *buf, uint *bitpos)
{
    Node *root = (Node *)malloc(sizeof(Node));
    if (root == NULL)
    {
        return NULL;
    }
    root->left = NULL;
    root->right = NULL;

    uchar c = ReadBit(fin, buf, bitpos);
    if (c == 0)
    {
        root->right = ReadTree(fin, buf, bitpos);
        root->left = ReadTree(fin, buf, bitpos);
    }
    else
    {
        root->left = NULL;
        root->right = NULL;
        root->symbol = ReadSymb(fin, buf, bitpos);
    }
    return root;
}

void DecodeMessage(FILE *fin, FILE *fout, Node *root, uint *bitpos,
                   uchar *buf)
{
    if ((!root->right) && (!root->left))
    {
        fwrite(&(root->symbol), sizeof(uchar), 1, fout);
        return;
    }
    uchar bit = ReadBit(fin, buf, bitpos);
    if (bit == 1)
        DecodeMessage(fin, fout, root->right, bitpos, buf);
    else
        DecodeMessage(fin, fout, root->left, bitpos, buf);
}

Errors decode(FILE *fin, FILE *fout)
{
    uint bitpos = 0;
    uchar buf = 0;
    uint message_len = 0;
    fseek(fin, 3, 0);
    if (!fread(&message_len, 4, 1, fin))
        return NO_MESSAGE;
    buf = fgetc(fin);

    Node *codeTree = ReadTree(fin, &buf, &bitpos);
    if (codeTree == NULL)
    {
        return NO_TREE_ROOT;
    }

    while (message_len > 0)
    {
        DecodeMessage(fin, fout, codeTree, &bitpos, &buf);
        message_len--;
    }
    DeleteTree(codeTree);
    return NO_ERRORS;
}

int main(void)
{
    FILE *fin = fopen("in.txt", "rb");
    if (fin == NULL)
    {
        printf("An error occured while opening the file!");
        return EXIT_SUCCESS;
    }

    FILE *fout = fopen("out.txt", "wb");
    if (fout == NULL)
    {
        printf("An error occured while opening the file!");
        fclose(fin);
        return EXIT_SUCCESS;
    }

    char workMode;
    if (!fscanf(fin, "%c", &workMode))
    {
        fclose(fin);
        fclose(fout);
        return EXIT_SUCCESS;
    }

    Errors MainStatus = NO_ERRORS;
    if (workMode == 'c')
    {
        MainStatus = encode(fin, fout);
        if (MainStatus != NO_ERRORS)
        {
            PrintErrorMessage(MainStatus);
        }
    }
    else if (workMode == 'd')
    {
        MainStatus = decode(fin, fout);
        if (MainStatus != NO_ERRORS)
        {
            PrintErrorMessage(MainStatus);
        }
    }

    fclose(fin);
    fclose(fout);
    return EXIT_SUCCESS;
}
