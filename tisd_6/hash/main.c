#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define SUCCESS 0
#define NOT_HERE 1
#define HERE 1

typedef struct list_element
{
	int data;
	struct list_element *next;
} list_element_t;

typedef struct table
{
	int size;
	list_element_t **lists;
} table_t;

int hash_by_num(int element, int num)
{
	return element % num;
}

int find_element_in_hash(table_t *tabl, int element, int (*hash)(int, int))
{
	if (tabl == NULL || hash == NULL)
		return ERROR;

	int num = 0, kol = 0;
	list_element_t *current;
	num = hash(element, tabl->size);
	current = (tabl->lists)[num];
	while (current != NULL){
		kol++;
		if (current->data == element){
			printf("\nElement found: %d\n", element);
			printf("Q-ty of compares: %d\n", kol);
			return SUCCESS;
		}
		current = current->next;
	}
	printf("Element %d isn`t found!\n", element);
	printf("Q-ty of compares: %d\n", kol);
	return NOT_HERE;
}

static int add_element_to_hash(table_t *tabl, int element, int (*hash)(int, int))
{
	if (tabl == NULL || hash == NULL)
		return ERROR;

	int num = 0;
	list_element_t *current;
	num = hash(element, tabl->size);
	
	if ((tabl->lists)[num] == NULL){
		(tabl->lists)[num] = (list_element_t*)malloc(sizeof(list_element_t));
		(tabl->lists)[num]->data = element;
		(tabl->lists)[num]->next = NULL;
		return SUCCESS;
	}
	else if (((tabl->lists)[num]->data == element))
		return HERE;

	current = (tabl->lists)[num];
	
	while (current->next != NULL){
		if (((tabl->lists)[num]->data == element))
			return HERE;
		
		current = current->next;
	}

	current->next = (list_element_t*)malloc(sizeof(list_element_t));
	current = current->next;
	current->next = NULL;
	current->data = element;
	return SUCCESS;
}

static int find_the_best_num(int num)
{
	if ((float)num / 2 - num / 2 == 0)
		return -1;
	
	for (int i = 3; i < num; i++){
		if ((float)num / i - num / i == 0)
			return -1;
	}
	return 1;
}

int create_hash_from_file(table_t **tabl, FILE *f)
{
	int tmp, err, kol = 0;
	
	do
	{
		kol++;
		err = fscanf(f, "%d", &tmp);
		if (err == 0){
			puts("UNKNOWN VALUE!");
			return ERROR;
		}
	} while (err == 1);
	
	kol--;
	
	while (find_the_best_num(kol) == -1)
		kol++;
	
	*tabl = (table_t*)malloc(sizeof(table_t));
	(*tabl)->lists = (list_element_t**)calloc(kol, sizeof(list_element_t *));
	(*tabl)->size = kol;
	return SUCCESS;
}

int fill_hash(table_t *tabl, FILE *f, int (*hash)(int, int))
{
	if (f == NULL || hash == NULL)
		return ERROR;
	
	int num = 0;
	while (fscanf(f, "%d", &num) == 1)
		add_element_to_hash(tabl, num, hash);
	
	return SUCCESS;
}

int print_table(table_t *tabl)
{
	if (tabl == NULL)
		return ERROR;
	
	list_element_t *current;
	
	for (int i = 0; i < tabl->size; i++){
		current = (tabl->lists)[i];
		if (current == NULL)
			continue;
		
		printf("%d:", i);
		
		while (current != NULL){
			printf("%d -> ", current->data);
			current = current->next;
		}
		if (current == NULL)
			puts("(null)");
	}
	return SUCCESS;
}

int menu()
{
	int c;
	printf("1. Print hash-table\n");
	printf("2. Find element\n");
	printf("0. Exit\n");
	printf("Input: ");
	scanf("%d", &c);
	return c;
}

int main(void)
{
	table_t *table = NULL;
	FILE *f = NULL;
	int num, c = 1;;
	f = fopen("in.txt", "r");
	create_hash_from_file(&table, f);
	rewind(f);
	fill_hash(table, f, hash_by_num);
	fclose(f);
	while (c){
		c = menu();
		switch(c){
			case 1:
				puts("\nHash-table:");
				print_table(table);
				printf("\n");
				printf("Size of table: %d\n\n", table->size);
				break;
			case 2:
				printf("\nInput the number which you want to find: ");
				scanf("%d", &num);
				if(find_element_in_hash(table, num, hash_by_num)){
					add_element_to_hash(table, num, hash_by_num);
					printf("Element added.\n");
				}
				printf("\n");
				break;
		}
	}
	return 0;
}
