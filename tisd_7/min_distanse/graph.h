#ifndef GRAPH_H
#define GRAPH_H

int minimum(int a, int b);

int **allocate_matrix(int n, int m);

int **fill_matrix(FILE *file_in, int **matrix, int cnt);

void print_matrix(int **matrix, int cnt, int flag);

int *dijkstra(int cnt, int **matrix, int v0, int *min_len);

#endif
