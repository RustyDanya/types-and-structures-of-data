#include <stdlib.h>
#include <stdio.h>
#define OK 0
#define ERR_MEMORY -1
#define ERR_PARAM -2
#define ERR_FILE -3
#define ERR_CMD -4

typedef struct list list_t;

struct list
{
    int num;
    list_t *next;
};

list_t* create_list(int num)
{
    list_t *tmp = malloc(sizeof(list_t));

    if (tmp)
    {
        tmp->num = num;
        tmp->next = NULL;
    }
	else
	{
		free(tmp);
		tmp = NULL;
	}

    return tmp;
}

list_t* add_to_list(list_t *head, list_t *number)
{
    list_t *cur = head;

    if (!head)
        return number;
	
    for ( ;cur->next; cur = cur->next)
        ;
    cur->next = number;
    return head;
}

void free_list(list_t *head)
{
    list_t *next;

    for ( ;head; head = next)
    {
        next = head->next;
        free(head);
    }
}

list_t *find_element(list_t *head, int number)
{
	while (head)
	{
		if (head->num == number)
			return head;
		head = head->next;
	}
	
	return NULL;
}

list_t *delete_element(list_t *head, list_t *element)
{
	list_t *start_point = head;
	if (head == element)
	{
		start_point = head->next;
		free(element);
	}
	else
	{
		for ( ; head->next != element; head = head->next)
			;
		
		head->next = element->next;
		free(element);
	}
	
	return start_point;
}

void swap(list_t *a, list_t *b) 
{ 
    int temp = a->num; 
    a->num = b->num; 
    b->num = temp; 
} 

void sort(list_t *start) 
{ 
    int swapped; 
   list_t *ptr1; 
    list_t *lptr = NULL; 
 
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1->num > ptr1->next->num) 
            {  
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } while (swapped); 
} 

void print_list(list_t *head)
{
	if (!head)
		printf("Empty list\n");
	
	while(head)
	{
		printf("%d ", head->num);
		head = head->next;
	}
}

int main()
{
	setbuf(stdout, NULL);
	list_t *head = NULL;
	int action;
	do {
		puts("1 - Add element in list");
        puts("2 - Delete element");
        puts("3 - Print list");
        puts("0 - Exit");
        scanf("%d", &action);
	    printf("\n");
        switch(action) {
			case 1:
			{
				int num;
				printf("Enter integer value: ");
				scanf("%d",&num);
				list_t *tmp = create_list(num);
				if (tmp)
					head = add_to_list(head, tmp);
				else 
					free_list(head);
				printf("\n");
				sort(head);
				break;
			}
			case 2:
			{
				printf("Enter value to delete: ");
				int number;
				if (scanf("%d", &number) == 1)
				{
					list_t *element = NULL;
					element = find_element(head, number);
					if (element)
						head = delete_element(head, element);
				}
				else 
					printf("Error enter!");
			}
			case 3:
			{
				printf("list: ");
				print_list(head);
				printf("\n");
				break;
			}
		}
	} while(action != 0);
	
	free_list(head);
	return 0;
}