// C program for linked list implementation of stack 
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "stack.h"
  
struct _StackNode { 
    int data; 
    StackNode* next; 
};
  
StackNode* stack_newNode(int data) 
{ 
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode)); 
    stackNode->data = data; 
    stackNode->next = NULL; 
    return stackNode; 
} 
  
int stack_isEmpty(StackNode* root) 
{ 
    return !root; 
} 
  
void stack_push(StackNode** root, int data) 
{ 
    StackNode* stackNode = stack_newNode(data); 
    stackNode->next = *root; 
    *root = stackNode;
} 
  
int stack_pop(StackNode** root) 
{ 
    if (stack_isEmpty(*root)) 
        return INT_MIN; 
    StackNode* temp = *root; 
    *root = (*root)->next; 
    int popped = temp->data; 
    free(temp); 
  
    return popped; 
} 
  
int stack_peek(StackNode* root) 
{ 
    if (stack_isEmpty(root)) 
        return INT_MIN; 
    return root->data; 
} 