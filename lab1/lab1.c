#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct TreeNode {
    uint32_t max_in_subtree;
    uint32_t value;  // exists if leaf
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct Tree {
    TreeNode* root;
    uint32_t height;
} Tree;

typedef struct Stack {
    Tree tree;
    struct Stack* next;
} Stack;

Stack new_array() {
    Tree tree;
    Stack stack;
    stack.tree = tree;
    stack.next = NULL;
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

void push() {

}


void pop() {

}


void set(Stack* stack, int i, int value) {
    Stack new_stack = new_array();
    new_stack.next = stack;

}


int main() {
    // Your code here
    printf("Hello, World!\n");
    navigate_tree(4, 5); // Example usage: height = 4, index = 5
    return 0;
}