#include "../include/types.h"

void stack_push(Stack_t  **head, void *value)
{
    Stack_t *tmp = malloc(sizeof(Stack_t));

    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

void *stack_pop(Stack_t **head)
{
    Stack_t *out;
    void *value;

    if (*head == NULL)
        return NULL;

    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);

    return value;
}

void  *stack_top(const Stack_t *head)
{
    if (head == NULL) {
        return NULL;
    }
    return head->value;
}

