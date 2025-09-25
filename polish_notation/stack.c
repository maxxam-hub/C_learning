#include "stack.h"

void stack_init(Stack *stack, int size) {
    stack->data = (char **)malloc(sizeof(char *) * size);
    stack->size = size;
    stack->top = -1;
}

int stack_empty(const Stack *stack) { return stack->top == -1; }

void stack_push(Stack *stack, const char *value) {
    if (stack->top < stack->size - 1) {
        stack->top++;
        stack->data[stack->top] = (char *)malloc(sizeof(char) * (strlen(value) + 1));
        strcpy(stack->data[stack->top], value);
    }
}

char *stack_pop(Stack *stack) {
    char *value = NULL;
    if (!stack_empty(stack)) {
        value = stack->data[stack->top];
        stack->top--;
    }
    return value;
}

char *stack_top(const Stack *stack) {
    char *value = NULL;
    if (!stack_empty(stack)) {
        value = stack->data[stack->top];
    }
    return value;
}

void stack_free(Stack *stack) {
    for (int i = 0; i <= stack->top; i++) {
        free(stack->data[i]);
    }
    free(stack->data);
}