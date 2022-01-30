#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    char *token;
    int type:4,
        prio:3;
} Token_t;

typedef struct {
    Token_t *tokens;
    int capacity;
} TokenArr_t;

typedef struct Stack {
    struct Stack *next;
    void *value;
} Stack_t;

typedef union Cast {
    double d;
    long long int li;
    float f;
    int i;
    short s;
    char c
} Cast_t;

TokenArr_t *_strtok(char *);
TokenArr_t *rpnTranslator(TokenArr_t *);
double stackMachine(double, TokenArr_t *);

void stack_push(Stack_t **head, void *value);
void *stack_pop(Stack_t **head);
void *stack_top(const Stack_t *head);

#endif // TYPES_H_INCLUDED
