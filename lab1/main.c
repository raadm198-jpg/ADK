#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#define max(a, b) ((a) > (b) ? (a) : (b))

typedef struct TreeNode {
    int max_in_subtree;
    int value;  // exists if leaf (somewhere)
    bool has_left;
    bool has_right;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Tree {
    TreeNode* root;
    int height;
} Tree;

typedef struct StackNode {
    struct Tree* tree;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
    int count;
} Stack;

void push(Stack* stack, Tree* tree) {
    StackNode* new_node = malloc(sizeof(StackNode));
    new_node->tree = tree;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->count++;
}

void unset(Stack* stack) {
    if (stack->count > 1) {
        StackNode* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
        stack->count--;
    }
}

Stack new_array() {
    Stack stack = {
        .top = NULL,
        .count = 0
    };

    Tree* tree = malloc(sizeof(Tree));
    tree->root = NULL;
    tree->height = 0;

    push(&stack, tree);

    return stack;
}

TreeNode* copy_node(TreeNode* current_node) {
    TreeNode* new_node = malloc(sizeof(TreeNode));

    if (current_node == NULL) {
        new_node->max_in_subtree = 0;
        new_node->value = 0;
        new_node->has_left = false;
        new_node->has_right = false;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    *new_node = *current_node;
    return new_node;
}


void update_max_in_subtree(TreeNode* updated_node) {

    bool found_something = false;

    int subtree_max = 0;

    if (updated_node->has_left) {
        subtree_max = updated_node->left->max_in_subtree;
        found_something = true;
    }
    if (updated_node->has_right) {
        int value = updated_node->right->max_in_subtree;
        if (!found_something || value > subtree_max) subtree_max = value;
        found_something = true;
    }
    if (found_something) updated_node->max_in_subtree = subtree_max;
}

TreeNode* extend_tree_height(TreeNode* root, int old_height, int new_height) {
    while (old_height < new_height) {
        TreeNode* new_root = copy_node(NULL);

        new_root->left = root;

        if (root != NULL) {
            new_root->has_left = true;
            new_root->max_in_subtree = root->max_in_subtree;
        }

        root = new_root;
        old_height++;
    }
    return root;
}

TreeNode* recursive_set(TreeNode* current_node, int i, int level, int height, int value) {

    TreeNode* updated_node = copy_node(current_node);

    if (height == level) {
        updated_node->value = value;
        updated_node->max_in_subtree = value;
        return updated_node;
    }

    bool go_right = (i >> (height - level - 1)) & 1;
    if (go_right) {
        updated_node->right = recursive_set(current_node ? current_node->right: NULL, i, level + 1, height, value);
        updated_node->has_right = true;
    }
    else {
        updated_node->left = recursive_set(current_node ? current_node->left: NULL, i, level + 1, height, value);
        updated_node->has_left = true;
    }

    update_max_in_subtree(updated_node);

    return updated_node;
}

void set(Stack* stack, int i, int value) {
    Tree* current_tree = stack->top->tree;
    Tree* updated_tree = malloc(sizeof(Tree));

    int required_height;

    if (i == 0) {required_height = 1;
    } else {required_height = 32 - __builtin_clz((int)i);}

    required_height = max(required_height, current_tree->height);
    updated_tree->height = required_height;

    TreeNode* root = extend_tree_height(current_tree->root, current_tree->height, required_height);

    updated_tree->root = recursive_set(root, i, 0, required_height, value);
    push(stack, updated_tree);
}

int maxleftsegment(TreeNode* root, int level, int index) {
    if (root == NULL) return 0;
    if (level == 0) return root->value;

    int bit = (index >> (level - 1)) & 1;

    if (!bit) {
        int left = maxleftsegment(root->left, level - 1, index);
        int right = root->right ? root->right->max_in_subtree : 0;
        return max(left, right);
    }

    return maxleftsegment(root->right, level - 1, index);
}

int maxrightsegment(TreeNode* root, int level, int index) {
    if (root == NULL) return 0;
    if (level == 0) return root->value;

    int bit = (index >> (level - 1)) & 1;

    if (!bit) {
        return maxrightsegment(root->left, level - 1, index);
    }

    int left = root->left ? root->left->max_in_subtree : 0;
    int right = maxrightsegment(root->right, level - 1, index);

    return max(left, right);
}

int maxsegment(TreeNode* root, int level, int left_index, int right_index) {
    if (root == NULL) return 0;
    if (level == 0) return root->value;

    int left_bit = (left_index >> (level - 1)) & 1;
    int right_bit = (right_index >> (level - 1)) & 1;

    if (left_bit == 0 && right_bit == 0)
        return maxsegment(root->left, level - 1, left_index, right_index);

    if (left_bit == 1 && right_bit == 1)
        return maxsegment(root->right, level - 1, left_index, right_index);

    int left_max = maxleftsegment(root->left, level - 1, left_index);
    int right_max = maxrightsegment(root->right, level - 1, right_index);

    return max(left_max, right_max);
}

int max_in_interval(Stack* stack, int left, int right) {

    int level = stack->top->tree->height;
    long long max_index = (1LL << stack->top->tree->height) - 1;

    // Entire interval is outside the represented tree
    if (left > max_index) return 0;
    // Clamp the right endpoint to the tree
    if (right > max_index) right = (int)max_index;
    return maxsegment(stack->top->tree->root, level, left, right);
}

void get(Stack* stack, int i) {
    Tree* current_tree = stack->top->tree;

    if (current_tree->root == NULL) {
        printf("0\n");
        return;
    }

    TreeNode* current_node = current_tree->root;

    int height = current_tree->height;
    for (int j = height-1; j >= 0; j--) {
        int bit = (i >> j) & 1;
        if (bit == 0 && current_node->has_left) {
            current_node = current_node->left;
        } else if (bit == 1 && current_node->has_right) {
            current_node = current_node->right;
        } else {
            printf("0\n");
            return;
        }
    }
    printf("%d\n", current_node->value);
}

// int main(void) {

//     // initialize version stack
//     Stack array = new_array();
//     int index;
//     int value;
//     int left;
//     int right;

//     char command[30];

//     while (scanf("%s", command) == 1) {

//         if (strcmp(command, "get") == 0) {


//             scanf("%d", &index);
//             if (index >= 0) {
//                 get(&array, index);
//             }

//         } else if (strcmp(command, "set") == 0) {

//             scanf("%d %d", &index, &value);
//             if (index >= 0) {
//                 set(&array, index, value);
//             }

//         } else if (strcmp(command, "unset") == 0) {

//             unset(&array);

//         } else if (strcmp(command, "maxininterval") == 0) {

//             scanf("%d %d", &left, &right);
//             if (left >= 0 && right >= 0 && left <= right) {
//                 printf("%d \n", max_in_interval(&array, left, right));
//             }
//             else {
//                 printf("0\n");
//             }
//             // read left and right
//             // call maxininterval

//         }
//     }

//     return 0;
// }

int main(void){
    Stack array = new_array();
    // Test 1
    
//    set (&array, 0, 5);
//    set (&array, 1, 10);
//    set (&array, 2, 15);
//    set (&array, 3, 20);
//    get (&array, 0); // should print 5
//    get (&array, 1); // should print 10
//    get (&array, 3); // should print 20
//    unset (&array);
//    get (&array, 3); // should print 0
//    printf("%d\n", max_in_interval (&array, 0, 3)); // should print 15
//    printf("%d\n", max_in_interval (&array, 4, 6)); // should print 0
//
//
// Test 2
   set (&array, 0, 5);
   set (&array, 1, 10);
   set (&array, 0, 50);
   set (&array, 3, 20);
   set (&array, 4, 25);
   printf("%d\n", max_in_interval (&array, 0, 4)); // should print 50
   printf("%d\n", max_in_interval (&array, 1, 3)); // should print 20
   set (&array, 2000, 500);
   printf("%d\n", max_in_interval (&array, 0, 2000)); // should print 500
   printf("%d\n", max_in_interval (&array, 0, 1999)); // should print 50

}