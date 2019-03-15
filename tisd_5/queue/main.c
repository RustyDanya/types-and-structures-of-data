#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>

#define SIZE 100

#define OK 0
#define FULL -1
#define EMPTY -2
#define MEMORY_ERROR -3

unsigned long long tick(void)
{
	unsigned long long d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}

typedef struct linked_list linked_list_t;
struct linked_list {
    int value;
    linked_list_t *next;
};

linked_list_t *create_list_node(int value) {
    linked_list_t *node = malloc(sizeof(linked_list_t));

    if (node) {
        node->value = value;
        node->next = NULL;
    }

    return node;
}

linked_list_t *remove_end(linked_list_t* head) {
    if (head == NULL) {
        return NULL;
    } else if (head->next == NULL) {
        free(head);
        return NULL;
    }
    linked_list_t *pre_last_node = head;
    while (pre_last_node->next->next != NULL) {
        pre_last_node = pre_last_node->next;
    }
    free(pre_last_node->next);
    pre_last_node->next = NULL;
    return head;
}

linked_list_t *insert_head(linked_list_t *head, linked_list_t *node) {
    node->next = head;
    return node;
}

void free_linked_list(linked_list_t *head) {
    linked_list_t *next = NULL;
    linked_list_t *cur = head;
    while (cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

void print_queue(FILE *stream, const linked_list_t *queue) {
    if (queue == NULL) {
        fprintf(stream, "Empty queue!");
    }
    for (; queue != NULL; queue = queue->next)
		fprintf(stream, "%d ", queue->value);
}

int push_arr(int *arr, int *arr_e, int **pin, int **pout, int *k, int temp)
{
	if (*pin == *pout && *k > 0)
		return FULL;
	**pin = temp;
	if (*k == 0)
		*pout = *pin;
	if (*pin == arr_e - 1)
		*pin = arr;
	else
		*pin += 1;
	*k += 1;
	return OK;
}

int pop_arr(int *arr, int *arr_e, int **pout, int *k)
{
	if (*k == 0)
		return EMPTY;
	int n = **pout;
	if (*pout == arr_e - 1)
		*pout = arr;
	else
		*pout += 1;
	*k -= 1;
	return n;
}

void print_arr(int *arr, int *arr_e, int **pin, int **pout, int *k)
{
	if (*k == 0){
		printf("Empty queue!");
		return;
	}
	int a[10];
	int *pa = a;
	while (*k > 0)
	{
		int n = pop_arr(arr, arr_e, pout, k);
		printf("%d ", n);
		*pa = n;
		pa++;
	}
	int *pb = a;
	while (pb < pa)
	{
		push_arr(arr, arr_e, pin, pout, k, *pb);
		pb++;
	}
}

unsigned long long test_list(FILE *f, linked_list_t *head, linked_list_t *node, int *n)
{
	unsigned long long tb, te;
	tb = tick();
	int value;
	while (fscanf(f, "%d", &value) == 1)
	{
		linked_list_t *node = create_list_node(value);
		head = insert_head(head, node);
		(*n)++;
	}
	while (head == NULL)
	{
		remove_end(head);
	}
	te = tick();
	return te - tb;
}
unsigned long long test_array(FILE *f, int *arr, int *arr_e, int **pin, int **pout, int *k, int temp)
{
	unsigned long long tb, te;
	int rc;
	tb = tick();
	int value;
	while (fscanf(f, "%d", &value) == 1)
	{
		push_arr(arr, arr_e, pin, pout, k, temp);
	}
	rc = pop_arr(arr, arr_e, pout, k);
	while (rc != EMPTY)
		rc = pop_arr(arr, arr_e, pout, k);
	
	te = tick();
	return te - tb;
}		


int main() {
    int status_code = OK;

    setbuf(stdout, NULL);

    int rc, action = 0;
	int arr[SIZE];
	int k = 0;
	int *arr_e = arr + SIZE;
	int *pin = arr, *pout = NULL;
	FILE *f;
	int n;
	
	unsigned long long time_list, time_array;

    linked_list_t *linked_list_queue = NULL;
    do {
        puts("1 - Add element in list queue");
        puts("2 - Delete element in list queue");
        puts("3 - Print list queue");
		puts("4 - Add element in array queue");
        puts("5 - Delete element in array queue");
        puts("6 - Print array queue");
		puts("7 - Efficiency evaluation of 100 elements");
        puts("0 - Exit");
        scanf("%d", &action);

        switch (action) {
            case 1:
                printf("Enter integer value: ");
                int value;
                scanf("%d", &value);
                linked_list_t *node = create_list_node(value);
                if (node == NULL) {
                    status_code = MEMORY_ERROR;
                } else {
                    linked_list_queue = insert_head(linked_list_queue, node);
                }
                break;
            case 2:
                if (linked_list_queue == NULL) {
                    printf("Empty queue!\n");
                } else {
                    linked_list_queue = remove_end(linked_list_queue);
                    printf("Dequeued item from queue.\n");
                }
                break;
            case 3:
				printf("Queue: ");
				print_queue(stdout, linked_list_queue);
				printf("\n");
                break;
			
			case 4:
				setbuf(stdin, NULL);
				int temp;
				printf("Enter integer value: ");
				scanf("%d", &temp);
				rc = push_arr(arr, arr_e, &pin, &pout, &k, temp);
				if (rc == FULL)
					printf("FULL!\n");
				break;
			case 5:
				rc = pop_arr(arr, arr_e, &pout, &k);
				if (rc == EMPTY)
					printf("Empty queue!\n");
				else
					printf("Dequeued item from queue.\n");
				break;
			case 6:
				printf("Queue: ");
				print_arr(arr, arr_e, &pin, &pout, &k);
				printf("\n");
				break;
			case 7: 
				f = fopen("test.txt", "r");
				time_list = test_list(f, linked_list_queue, node, &n);
				rewind(f);
				time_array = test_array(f, arr, arr_e, &pin, &pout, &k, temp);
				fclose(f);
				printf("\n");
				printf("Time of filling and delete in list queue is %lld tackts\n", time_list);
				printf("\n");
				printf("Allocating size of list queue is %d bytes\n",sizeof(linked_list_queue) * n);
				printf("\n");
				printf("Time of filling and delete in array queue is %lld tackts\n", time_array);
				printf("\n");
				printf("Allocating size of array queue is %d bytes\n",sizeof(arr));
				printf("\n");
				printf("Time difference is %lld tackts\n", abs(time_array - time_list));
				printf("\n");
				printf("Size difference is %d bytes\n",abs(sizeof(linked_list_queue) * n - sizeof(arr)));
				printf("\n");
				n = 0;
                break;
		}
    } while(action != 0 && status_code == OK);

    free_linked_list(linked_list_queue);

    return status_code;
}