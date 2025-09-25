#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **data;
    int size;
    int top;
} Stack;

void stack_init(Stack *stack, int size);
int stack_empty(const Stack *stack);
void stack_push(Stack *stack, const char *value);
char *stack_pop(Stack *stack);
char *stack_top(const Stack *stack);
void stack_free(Stack *stack);

#endif