#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
 
#define MAX_STRING_SIZE 256
 
#define OK 0
#define INVALID_INPUT_ERROR -1

unsigned long long tick(void)
{
	unsigned long long d;
	__asm__ __volatile__ ("rdtsc" : "=A" (d) );
	return d;
}
 
typedef struct bst_tree_node bst_tree_node_t;
struct bst_tree_node
{
    char *key;
    bst_tree_node_t *left;
    bst_tree_node_t *right;
};

typedef struct avl_tree_node avl_tree_node_t;
struct avl_tree_node
{
	char *key;
	unsigned char height;
	avl_tree_node_t* left;
	avl_tree_node_t* right;
};

bst_tree_node_t *create_bst_tree_node(char *word)
{
    bst_tree_node_t *node = malloc(sizeof(bst_tree_node_t));
 
    if (node)
    {
        node->key = word;
        node->left = node->right = NULL;
    }
 
    return node;
}

bst_tree_node_t *insert_bst_tree(bst_tree_node_t *root, char *word)
{
    if (root == NULL) return create_bst_tree_node(word);
   
    if (strcmp(word, root->key) == -1) {
        root->left = insert_bst_tree(root->left, word);
    } 
	else if (strcmp(word, root->key) == 1) {
        root->right = insert_bst_tree(root->right, word);    
    }
    return root;
}

void free_bst_tree(bst_tree_node_t *root)
{
    if (root)
    {
        free_bst_tree(root->right);
        free_bst_tree(root->left);
        free(root);
    }
}

bst_tree_node_t* search_bst_tree(bst_tree_node_t *root, char *key) 
{
    if (root == NULL)
        return NULL;
   
    if (strcmp(key, root->key) == -1)
        return search_bst_tree(root->left, key);
    else if (strcmp(key, root->key) == 1)
        return search_bst_tree(root->right, key);
    else
        return root;
}

unsigned int get_bst_tree_height(bst_tree_node_t *root)
{
    unsigned int l, r;
    if(root != NULL)
	{
        l = (root->left  != NULL) ? get_bst_tree_height(root->left) : 0;
        r = (root->right != NULL) ? get_bst_tree_height(root->right) : 0; 
        return ((l > r) ? l : r) + 1;
    }
    return 0;
}

unsigned char height(avl_tree_node_t *p)
{
    return p ? p->height : 0;
}

int balance_factor(avl_tree_node_t *p)
{
    return height(p->right) - height(p->left);
}

void reset_height(avl_tree_node_t *p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr? hl : hr) + 1;
}

avl_tree_node_t *rotate_right(avl_tree_node_t *p)
{
    avl_tree_node_t *q = p->left;
    p->left = q->right;
    q->right = p;
    reset_height(p);
    reset_height(q);
    return q;
}

avl_tree_node_t *rotate_left(avl_tree_node_t *q)
{
    avl_tree_node_t *p = q->right;
    q->right = p->left;
    p->left = q;
    reset_height(q);
    reset_height(p);
    return p;
}

avl_tree_node_t *balance(avl_tree_node_t *p)
{
    reset_height(p);
    if (balance_factor(p) == 2)
    {
        if (balance_factor(p->right) < 0)
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }
    if (balance_factor(p) == -2)
    {
        if (balance_factor(p->left) > 0)
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    return p;
}

avl_tree_node_t *avl_add(avl_tree_node_t *root, char *word)
{
    if (root)
    {
        if (strcmp(word, root->key) == -1) 
			root->left = avl_add(root->left, word);
		else if (strcmp(word, root->key) == 1)
			root->right = avl_add(root->right, word);    
		
		return balance(root);
    }
    else
    {
        avl_tree_node_t *newel = malloc(sizeof(avl_tree_node_t));
        newel->key = word;
        newel->left = newel->right = NULL;
        newel->height = 1;
        return newel;
    }
}

void bst_to_avl(bst_tree_node_t *tree, avl_tree_node_t **root)
{
    if (tree && root)
    {
        *root = avl_add(*root, tree->key);
        bst_to_avl(tree->left, root);
        bst_to_avl(tree->right, root);
    }
}

void free_avl_tree(avl_tree_node_t *root)
{
    if (root)
    {
        free_avl_tree(root->right);
        free_avl_tree(root->left);
        free(root);
    }
}

avl_tree_node_t* search_avl_tree(avl_tree_node_t *root, char *key) 
{
    if (root == NULL)
        return NULL;
   
    if (strcmp(key, root->key) == -1)
        return search_avl_tree(root->left, key);
    else if (strcmp(key, root->key) == 1)
        return search_avl_tree(root->right, key);
    else
        return root;
}

unsigned int get_avl_tree_height(avl_tree_node_t *root)
{
    unsigned int l, r;
    if(root != NULL)
	{
        l = (root->left  != NULL) ? get_avl_tree_height(root->left) : 0;
        r = (root->right != NULL) ? get_avl_tree_height(root->right) : 0; 
        return ((l > r) ? l : r) + 1;
    }
    return 0;
}

void bst_to_dot(FILE *f1, bst_tree_node_t *root)
{
    if (f1 && root)
    {
        if (root->left)
            fprintf(f1, "%s -> %s;\n", root->key, root->left->key);
        if (root->right)
            fprintf(f1, "%s -> %s;\n", root->key, root->right->key);
        bst_to_dot(f1, root->left);
        bst_to_dot(f1, root->right);
    }
}

void bst_export(FILE *f1, bst_tree_node_t *tree)
{
    fprintf(f1, "digraph BST_TREE {\n");
    bst_to_dot(f1, tree);   
	fprintf(f1, "}\n");
}

void avl_to_dot(FILE *f2, avl_tree_node_t *root)
{
    if (f2 && root)
    {
        if (root->left)
            fprintf(f2, "%s -> %s;\n", root->key, root->left->key);
        if (root->right)
            fprintf(f2, "%s -> %s;\n", root->key, root->right->key);
        avl_to_dot(f2, root->left);
        avl_to_dot(f2, root->right);
    }
}

void avl_export(FILE *f2, avl_tree_node_t *tree)
{
	fprintf(f2, "digraph AVL_TREE {\n");
    avl_to_dot(f2, tree);
    fprintf(f2, "}\n");
}
 
int main() 
{
    int status_code = OK;
   
    setbuf(stdout, NULL);
 
    int word_count = 0;
    printf("Input amount of words to enter: ");
    scanf("%d", &word_count);
    if (word_count <= 0) {
        fprintf(stderr, "Invalid input!");
        status_code = INVALID_INPUT_ERROR;
    } else {
        bst_tree_node_t *root_binary = NULL;
        avl_tree_node_t *balanced_root_binary = NULL;

        for (int i = 0; i < word_count; i++) {
            char *str = (char *)malloc(sizeof(char) * MAX_STRING_SIZE);
            printf("Enter string %d: ", i + 1);
            scanf("%s", str);
            if (i == 0) {
                root_binary = insert_bst_tree(root_binary, str);				
            } else {
                insert_bst_tree(root_binary, str);
            }
			bst_to_avl(root_binary, &balanced_root_binary);
        }
        int action;
        do {
            puts("1 - Get height of a binary search tree");
            puts("2 - Get height of a balanced binary search tree");
            puts("3 - Search binary search tree");
            puts("4 - Search balanced binary search tree");
			puts("5 - Efficiency evaluation");
            puts("0 - EXIT");
            scanf("%d", &action);
			printf("\n");
			char key[MAX_STRING_SIZE];           
            switch(action) {
                case 1:
                    printf("Height of a tree: %d\n", get_bst_tree_height(root_binary));
					printf("\n");
                    break;
                case 2:
					printf("Height of a tree: %d\n", get_avl_tree_height(balanced_root_binary));
					printf("\n");
                    break;
                case 3:
					printf("Enter a search key: ");
					scanf("%s", key);
					printf("\n");
					printf("Word is %s\n", search_bst_tree(root_binary, key) != NULL ? "found" : "not found");
					printf("\n");
                    break;
                case 4:
					printf("Enter a search key: ");
					scanf("%s", key);
					printf("\n");
					printf("Word is %s\n", search_avl_tree(balanced_root_binary, key) != NULL ? "found" : "not found");
					printf("\n");
                    break;
				case 5:
					printf("Enter a search key: ");
					scanf("%s", key);
					printf("\n");
					unsigned long tb, te;
					tb = tick();
					search_bst_tree(root_binary, key);
					te = tick();
					unsigned long time_bst = te - tb;
					tb = tick();
					search_avl_tree(balanced_root_binary, key);
					te = tick();
					unsigned long time_avl = te - tb;
					printf("Time of search in binary search tree is %lu tackts\n", time_bst);
					printf("\n");
					printf("Time of search in balanced binary search tree is %lu tackts\n", time_avl);
					printf("\n");
					printf("Time difference is %d tackts\n", abs(time_bst - time_avl));
					printf("\n");
                    break;
            }
        } while(action != 0);
		FILE *f1, *f2;
		f1 = fopen("bst.dot", "w");
		f2 = fopen("avl.dot", "w");
		bst_export(f1, root_binary);
		avl_export(f2, balanced_root_binary);
		fclose(f1);
		fclose(f2);
		system("dot -Tpng bst.dot -o bst.png");
		system("dot -Tpng avl.dot -o avl.png");
        free_bst_tree(root_binary);
		free_avl_tree(balanced_root_binary);
    }
    return status_code;
}