#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

void pop(Stack* stack) {
    if (stack->top != NULL) {
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

    int left_max = 0;
    int right_max = 0;

    if (updated_node->has_left) {
        left_max = updated_node->left->max_in_subtree;
    }
    if (updated_node->has_right) {
        right_max = updated_node->right->max_in_subtree;
    }
    updated_node->max_in_subtree = max(left_max, right_max);
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

void get(Stack* stack, int i) {
    Tree* current_tree = stack->top->tree;
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

int main() {
    printf("Hello, World!\n");

    Stack stack = new_array();

    set(&stack, 2, 17);
    get(&stack, 2);

    return 0;
}