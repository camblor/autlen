
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h>

typedef struct _StackNode StackNode;
  
StackNode* newNode(int data);
  
int stack_isEmpty(StackNode* root);
  
void stack_push(StackNode** root, int data);
  
int stack_pop(StackNode** root);
  
int stack_peek(StackNode* root);