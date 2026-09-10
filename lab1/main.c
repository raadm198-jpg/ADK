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

int max_in_interval(Stack* stack, int left, int right) {

    bool same_path = true;
    bool continue_left = true;
    bool continue_right = true;
    bool found_something = false;

    int height = stack->top->tree->height;
    int max_index = (1 << height) - 1;
    if (left > max_index) return 0;
    if (right > max_index) right = max_index;

    int max_value = 0;
    int leftmost = 0;
    int rightmost = 0;

    TreeNode* left_end = stack->top->tree->root;
    TreeNode* right_end = stack->top->tree->root;

    int left_path;
    int right_path;
    
    for (int i = height - 1; i >= 0; i--) {
        left_path = (left >> i) & 1;
        right_path = (right >> i) & 1;
        if (same_path) {
            if (left_path == right_path) {
                switch (left_path)
                {
                case 0:
                    if (left_end->has_left) {
                        left_end = left_end->left;
                        right_end = right_end->left;
                        continue;
                    }
                    else return 0;
                    break;
                
                case 1:
                    if (right_end->has_right) {
                        left_end = left_end->right;
                        right_end = right_end->right;
                        continue;
                    }
                    else return 0;
                    break;
                }
            }

            else {
                same_path = false;
                if (!left_end->has_left) continue_left = false;
                else left_end = left_end->left;
                if (!right_end->has_right) continue_right = false;
                else right_end = right_end->right;
                if (!continue_left && !continue_right) return 0;
                continue;
            }
        }

        if (continue_left) {
            switch (left_path)
            {
            case 0:
                if (left_end->has_right) {
                    int value = left_end->right->max_in_subtree;
                    if (!found_something || value > max_value) {
                        max_value = value;
                        found_something = true;
                    }
                }
                if (left_end->has_left) {
                    left_end = left_end->left;
                }
                else continue_left = false;
                break;
            
            case 1:
                if (left_end->has_right) {
                    left_end = left_end->right;
                }
                else continue_left = false;
                break;
            }
        }
        if (continue_right) {
            switch (right_path)
            {
            case 0:
                if (right_end->has_left) {
                    right_end = right_end->left;
                }
                else continue_right = false;
                break;
            
            case 1:
                if (right_end->has_left) {
                    int value = right_end->left->max_in_subtree;

                    if (!found_something || value > max_value) {
                        max_value = value;
                        found_something = true;
                    }
                }

                if (right_end->has_right) {
                    right_end = right_end->right;
                }
                else {
                    continue_right = false;
                }
                break;
            }
        }

        if (!continue_left && !continue_right) {
            return max_value;
        }
    }

    if (continue_left) {
        int value = left_end->value;

        if (!found_something || value > max_value) {
            max_value = value;
            found_something = true;
        }
    }

    if (continue_right) {
        int value = right_end->value;

        if (!found_something || value > max_value) {
            max_value = value;
            found_something = true;
        }
    }

    if (!found_something)
        return 0;

    return max_value;
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
    printf("%u\n", current_node->value);
}

int main(void) {

    // initialize version stack
    Stack array = new_array();
    int index;
    int value;
    int left;
    int right;

    char command[30];

    while (scanf("%s", command) == 1) {

        if (strcmp(command, "get") == 0) {


            scanf("%d", &index);
            if (index > 0) {
                get(&array, index);
            }

        } else if (strcmp(command, "set") == 0) {

            scanf("%d %d", &index, &value);
            if (index > 0) {
                set(&array, index, value);
            }

        } else if (strcmp(command, "unset") == 0) {

            unset(&array);

        } else if (strcmp(command, "maxininterval") == 0) {

            scanf("%d %d", &left, &right);
            if (left > 0) {
                printf("%d \n", max_in_interval(&array, left, right));
            }
            // read left and right
            // call maxininterval

        }
    }

    return 0;
}