#include "define.h"
#include "graph.h"

int minimum(int a, int b)
{
	return (a > b) ? a : b;
}

int **allocate_matrix(int n, int m)
{
	int **matrix = malloc(n*sizeof(int*) + n*m*sizeof(int));

	if (!matrix)
		return NULL;

	for (int i = 0; i < n; i++)
		matrix[i] = (int*)((char*)matrix + n*sizeof(int*) + i*m*sizeof(int));

    return matrix;
}

int **fill_matrix(FILE *file, int **matrix, int cnt)
{
	for (int i = 0; i < cnt; i++)
		for (int j = 0; j < cnt; j++)
			fscanf(file, "%d", &matrix[i][j]);

	return matrix;
}

void print_matrix(int **matrix, int cnt, int flag)
{
	switch(flag)
	{
		case 1:
			for (int i = 0; i < cnt; i++){
				for (int j = 0; j < cnt; j++){
					if (matrix[i][j] == 0)
						printf("   -");
					else
						printf("%4d",matrix[i][j]);
				}
				printf("\n");
			}
			break;
		case 2:
			for (int i = 0; i < cnt; i++){
				for (int j = 0; j < cnt; j++){
					if (matrix[i][j] != 0)
						printf("   1");
					else
						printf("%4d", matrix[i][j]);
				}
				printf("\n");
			}
			break;
	}
}

int *dijkstra(int cnt, int **matrix, int v0, int *min_len)
{
	int already_used[cnt];
	int min_ind, min, tmp;

    for (int i = 0; i < cnt; i++){
		min_len[i] = INFINITY;
		already_used[i] = 1;
	}
	
	min_len[v0] = 0;
	
    do{
		min_ind = INFINITY;
		min = INFINITY;
		for (int i = 0; i < cnt; i++){
			if (already_used[i] == 1 && min_len[i] < min){
				min = min_len[i];
				min_ind = i;
			}
		}
		if (min_ind != INFINITY){
			for (int i = 0; i < cnt; i++){
				if (matrix[min_ind][i] > 0){
					tmp = min + matrix[min_ind][i];
        			if (tmp < min_len[i])
						min_len[i] = tmp; 
				}
			}
			already_used[min_ind] = 0;
		}
	} while (min_ind < INFINITY);

	return min_len;
}