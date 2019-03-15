#include "define.h"

int export_to_dot(int **matrix, int n, FILE *f, const char *name)
{
    if (matrix == NULL){
        printf("matrix is empty.\n");
        return 0;
    }
    fprintf(f, "graph %s {\n", name);
    for (int i = 0; i < n; i++){
        fprintf(f, "%d;\n", i);
        for (int j = 0; j < n; j++)
            if (j > i)
                if (matrix[i][j] != 0)
                    fprintf(f, "%d -- %d [label=%d] ;\n", i, j, matrix[i][j]);
    }
    fprintf(f, "}\n");
    return 1;
}

int main()
{
	int choice = -1;
	int **matrix = NULL;
	int vertex;
	int cnt;
    FILE *in, *out;
	setbuf(stdout, NULL);

	do{
		printf("1 - Enter graph in file\n");
		printf("0 - Exit and fill dot file\n");
		printf("\nInput: ");
		if (scanf("%d", &choice) != 1){
			printf("Failure. Wrong menu option.\n");
			return -1;
		}	
		switch(choice){
			case 1:
				printf("Input vertex: ");
				scanf("%d", &vertex);
				printf("\n");
				in = fopen("in.txt", "r");
				fscanf(in,"%d", &cnt);
				matrix = allocate_matrix(cnt, cnt);
				matrix = fill_matrix(in, matrix, cnt);
				printf("Label matrix of Edges:\n");
				print_matrix(matrix, cnt, 1);
				printf("\nMatrix of weight: \nVertex A  | Vertex B  | Smallest distance\n");
                for (int i = 0; i < cnt; i++){
					int *min_len = malloc(sizeof(int) * cnt);
					min_len = dijkstra(cnt, matrix, i, min_len);
					if (min_len[vertex] != INFINITY)
						printf("   %2d     |    %2d     |        %3d        \n", vertex , i , min_len[vertex]);
				}
				/*
				For all pairs of vertexes
				
                	for (int i = 0; i < cnt; i++){
					int *min_len = malloc(sizeof(int) * cnt);
					min_len = dijkstra(cnt, matrix, i, min_len);
					for (int j = i+1; j < cnt; j++)
						if (min_len[j] != INFINITY)
							printf("   %2d     |    %2d     |        %3d        \n", i , j , min_len[j]);
				}	
				*/
				printf("\n");
				out = fopen("out.dot", "w");
                export_to_dot(matrix, cnt, out, "my_graph");
				break;
			default:
				printf("Error input!\n");
				printf("\n");
				break;
		}
	} while (choice != 0);
	free(matrix);
	fclose(in);
	fclose(out);
	system("dot -Tpng out.dot -o out.png");
}
