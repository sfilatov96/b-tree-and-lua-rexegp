#include <stdio.h>
#include <stdlib.h>
int degree; // Параметр дерева


struct node{
    int is_leaf; // Является ли узел листом(нет потомков)
    int is_root; // Является ли узел основанием всего дерева (Устанавливается один раз)
    int n; // Количество ключей в массиве
    int* keys; // Массив ключей
    struct node **p; // Массив указтелей на другие узлы
};

/* Очищает узел(используется каждый раз при создании узла)*/
void clear_node(node* root){
    root->keys = (int*)malloc(sizeof(int)*degree);
    root->p = (node**)malloc(sizeof(node*)*(degree+1));
    for (int i = 0; i < degree; ++i) {
        root->keys[i] = 0;
        root->p[i]=NULL;
    }
    root->n=0;
    root->p[degree+1]=NULL;
    root->is_leaf=1;
    root->is_root=0;
}

void print_b_tree(node* root){
    printf("|");
    for (int j = 0; j < root->n; ++j) {
        printf("%d ", root->keys[j]);

    }
    printf("|");
    for (int i = 0; i != root->n + 1; ++i) {
        if (root->p[i] ) {
            print_b_tree(root->p[i]);
        }
    }
}

/* Перепроверяет всё дерево на листы и -не листы  и изменяет параметр root->is_leaf*/
void check_leaf(node* root){
    for (int i = 0; i != root->n + 1; ++i) {
        if (root->p[i] ) {
            root->is_leaf=0;
            check_leaf(root->p[i]);
        } else {
            root->is_leaf=1;
        }
    }
}

/* Добавлеят новое значение в массив ключей */
int add_value_in_array(node* root, int value){
    if (root -> is_leaf == 1) {
        int step = root->n;
        for (int i = 0; i < degree; i++) {
            if (root->keys[i] > value) {
                step = i;
                break;

            }
        }
        for (int j = degree - 1; j >= step; j--) {
            root->keys[j] = root->keys[j - 1];
        }
        root->keys[step] = value;
        root->n = root->n + 1;
        return 0;
    }
    else {
        if (root->n == degree){
            return 0;
        }
        if (root->keys[0] > value){
            add_value_in_array(root->p[0], value);
        }
        if (root->keys[root->n-1] < value){
            add_value_in_array(root->p[root->n], value);
        }
        for (int i = 0; i < root->n; ++i) {
            if ((root->keys[i] < value) && (root->keys[i+1] > value)) {
                add_value_in_array(root->p[i+1], value);
            }
        }
    }
    return 0;
}

/* Делит массив ключей и массив указателей на левую и правую половины*/
int split(node* root){
    int split_pos;
    if (degree % 2 == 0){
        split_pos = degree/2 - 1;
    } else {
        split_pos = degree/2;
    }
    int middle_val;
    node *ptr = NULL;
    for (int k = 0; k < root->n+1; ++k) {
        ptr = root->p[k];
        if (ptr && ptr->n >= degree) {
            middle_val = ptr->keys[split_pos];
            node *left = (node *) malloc(sizeof(struct node));
            node *right = (node *) malloc(sizeof(struct node));
            clear_node(left);
            clear_node(right);
            for (int j = 0; j < degree; ++j) {
                if (j < split_pos) {
                    left->keys[j] = ptr->keys[j];
                    left->p[j] = ptr->p[j];
                    left->n = left->n + 1;
                }
                if (j > split_pos) {
                    right->keys[j- split_pos - 1] = ptr->keys[j];
                    right->p[j - split_pos - 1] = ptr->p[j+1];
                    right->n = right->n +1;
                }
            }
            left->p[split_pos] = ptr->p[split_pos];
            right->p[0] = ptr->p[split_pos + 1];

            int step = root->n;
            for (int i = 0; i < root->n; i++) {
                if (root->keys[i] > middle_val) {
                    step = i;
                    break;

                }
            }
            for (int j = degree - 1; j >= step; j--) {
                root->keys[j] = root->keys[j - 1];
                root->p[j + 1] = root->p[j];
            }
            root->p[step] = left;
            root->p[step+1] = right;
            root->keys[step] = middle_val;
            root->n = root->n + 1;
        }
    }
    return 0;
}

/* Перестраивает корень(основание) дерева если количесво ключей в узле достигло максимума*/
int rebuild_root(node* root){
    if (root->is_root == 1) {
        if (root->n >= degree) {
            int split_pos;
            if (degree % 2 == 0){
                split_pos = degree/2 - 1;
            } else {
                split_pos = degree/2;
            }
            int middle_val;
            middle_val = root->keys[split_pos];
            node *left = (node *) malloc(sizeof(struct node));
            node *right = (node *) malloc(sizeof(struct node));
            clear_node(left);
            clear_node(right);
            for (int j = 0; j < degree; ++j) {
                if (j < split_pos) {
                    left->keys[j] = root->keys[j];
                    left->p[j] = root->p[j];
                    left->n = left->n + 1;
                }
                if (j > split_pos) {
                    right->keys[j - split_pos - 1] = root->keys[j];
                    right->p[j - split_pos ] = root->p[j+1];
                    right->n = right->n + 1;
                }
            }
            left->p[split_pos] = root->p[split_pos];
            right->p[0] = root->p[split_pos + 1];

            if (root->p[0]){
                root->is_leaf=0;
            }

            root->p[0] = left;
            root->p[1] = right;
            root->n = 1;
            root->keys[0] = middle_val;
            root->is_leaf = 0;
        }
    }
    return 0;
}

/* Перестраивает узлы в дереве если узлы переполнены */
int rebuild_b_tree(node*root){
    if (root) {
            for (int i = 0; i < root->n + 1; ++i) {
                rebuild_b_tree(root->p[i]);
            }
            split(root);
    }
    return 0;
}

void insert(node* root, int value){
    check_leaf(root);
    add_value_in_array(root, value);
    rebuild_b_tree(root);
    rebuild_root(root);
    print_b_tree(root);
}

int get(node* root, int value){
    node *ptr = root;
    while(ptr) {
        for (int i = 0; i < ptr->n; ++i) {
            if (ptr->keys[i] == value) {
                printf("Value %d found in B-tree!\n", ptr->keys[i]);
                return 0;
            }
        }
        if (ptr->is_leaf == 1){
            printf("Value not found in B-tree!\n");
            return 0;
        }
        if (ptr->keys[0] > value) {
            ptr = ptr -> p[0];
        }
        if (ptr->keys[ptr->n - 1] < value) {
            ptr = ptr->p[ptr->n];
        }

        for (int i = 0; i < ptr->n; ++i) {
            if ((ptr->keys[i] < value) && (ptr->keys[i + 1] > value)) {
                ptr = ptr->p[i + 1];
            }
        }
    }

    return  0;
}


int main() {
    printf("B-tree degreeodel Builder!\n");
    printf("a -- Added new value in B-tree\n");
    printf("f -- Find value in tree\n");
    printf("d -- Display all data from B-tree\n");
    printf("q -- Quit from program\n");

    printf("Enter B-tree degree: ");
    scanf("%d", &degree);
    if (degree < 2) {
        printf("Bad degree! Degree must be greater than 1!");
        exit(0);
    }

    node *root = (node*)malloc(sizeof(struct node));
    clear_node(root);
    root->is_root=1;
    char choice;
    int value;
    while(1) {
        printf("\nEnter command: ");
        scanf("%s", &choice);
        switch (choice){
            case 'a':
                printf("Enter Value: ");
                scanf("%d", &value);
                insert(root,value);
                break;
            case 'f':
                printf("Enter Value: ");
                scanf("%d", &value);
                get(root, value);
                break;
            case 'd':
                print_b_tree(root);
                break;
            case 'q':
                free(root);
                printf("Bye!\n");
                exit(0);
            default:
                printf("Undefined Command!\n");
                break;
        }
    }
    return 0;
}