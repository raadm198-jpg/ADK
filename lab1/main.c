#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct TreeNode {
    uint32_t max_in_subtree;
    uint32_t value;  // exists if leaf (somewhere)
    bool has_left;
    bool has_right;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Tree {
    TreeNode* root;
    uint32_t height;
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
    Tree tree;
    Stack stack;
    push(&stack, &tree);
    return stack;
}

int navigate_tree(uint32_t height, uint32_t index) {
    for (uint32_t i = height-1; i >= 0; i--) {
        uint32_t bit = (index >> i) & 1;
        if (bit == 0) {
            printf("Left\n");
        } else {
            printf("Right\n");
        }
    }
}


TreeNode* new_tree_node(bool root, uint32_t value, uint32_t max_in_subtree, TreeNode* current_node) {

    TreeNode* new_node = malloc(sizeof(TreeNode));
    new_node->max_in_subtree = max_in_subtree;
    new_node->value = value;
    new_node->has_left = false;
    new_node->has_right = false;
    new_node->left = NULL;
    new_node->right = NULL;

    if (current_node->has_left) {
        new_node->left = current_node->left;
        new_node->has_left = true;
    }
    if (current_node->has_right) {
        new_node->right = current_node->right;
        new_node->has_right = true;
    }
    return new_node;
}

TreeNode* follow_or_depart_old_tree(TreeNode* parent_node, bool is_left, int value) {

    if (is_left) {
        if (parent_node->has_left) {
        uint32_t max_in_sub_tree = parent_node->left->max_in_subtree;
        if (max_in_sub_tree < value) {
            TreeNode* next_tree_node = new_tree_node(parent_node, is_left, value);
            return next_tree_node;
        }
        } else {
            TreeNode* next_tree_node = new_tree_node(parent_node, is_left, value);
            return next_tree_node;
        }
    } else if (parent_node->has_right) {
        uint32_t max_in_sub_tree = parent_node->right->max_in_subtree;
    } else {
        new_tree_node(parent_node, is_left);
    }

}


void recursive_set(Stack* stack, TreeNode* updated_tree_node, TreeNode* current_node, int i, int level, int height, int value) {

    

}

void set(Stack* stack, int i, int value) {

    Tree* updated_tree = malloc(sizeof(Tree));
    uint32_t new_height = max(31 - __builtin_clz(i), stack->top->tree->height);
    updated_tree->height = new_height;
    TreeNode* current_node = stack->top->tree->root;
    TreeNode* updated_tree_root;
    if (value > current_node->max_in_subtree) {
        updated_tree_root = new_tree_node(true, 0, value, current_node);
        updated_tree->root = updated_tree_root;
    } else {
        updated_tree->root = current_node;
        updated_tree_root = current_node;
    }
    recursive_set(stack, updated_tree_root, current_node, i, 1, new_height, value);
}


int main() {
    // Your code here
    printf("Hello, World!\n");
    navigate_tree(4, 5); // Example usage: height = 4, index = 5
    return 0;
}