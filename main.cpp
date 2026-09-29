#include "stack.h"

int main()
{
    stack_t variable_1 = {};
    size_t init_size = 2;
    FILE* log = fopen("mimilogs.log", "a");

    int init_res = stack_initialise(&variable_1, init_size);
    if (init_res != ALL_OKAY) {abort();}
    int res_of_verity = stack_verity(&variable_1);
    if (res_of_verity != ALL_OKAY) {abort();}

    stack_push(&variable_1, 6.7);
    stack_push(&variable_1, 6.767676);
    STACK_DUMP(log, &variable_1, ALL_OKAY);
    stack_push(&variable_1, 67.67);
    STACK_DUMP(log, &variable_1, ALL_OKAY);
    stack_push(&variable_1, 5.);
    STACK_DUMP(log, &variable_1, ALL_OKAY);
    stack_pop(&variable_1); 
    STACK_DUMP(log, &variable_1, ALL_OKAY);
    fclose(log);
    FREE_MASS(variable_1.stack)
}

int stack_dump(FILE* log, stack_t* variable, Errors error, const char* file_name, const char* func_name, int line)
{
    if (error == ALL_OKAY) {fprintf(log, "All is OK!\n");}
    if (error == NOT_ALLOCATE) {fprintf(log, "ERROR: Memory don`t allocate!\n");}
    if (error == NULL_CAPACITY) {fprintf(log, "ERROR: The capacity of stack is NULL!\n");}
    if (error == NULL_ADRESS) {fprintf(log, "ERROR: The adress of struct or stack is NULL!\n");}
    if (error == INCORRECT_SIZE) {fprintf(log, "ERROR: The size of stack if incorrect!\n");}

    fprintf(log, "Dumb was summoned in\n file: <%s>\n function: <%s>\n line: <%d>\n", file_name, func_name, line);
    fprintf(log, "Current parametres:\n");
    fprintf(log, "size: <%zu>\n", variable->size);
    fprintf(log, "capacity: <%zu>\n", variable->capacity);

    for (size_t i = 0; i < variable->capacity; i++)
    {
        fprintf(log, "[%zu]  -  <%lg>\n", i, variable->stack[i]);
    }
    fprintf(log, "------------------------------------------------------------------------\n");

    return 0;
}

int stack_initialise(stack_t* variable, size_t init_capacity)
{
    if (init_capacity == 0) {return NULL_CAPACITY;}

    variable->stack = (StackElem*) calloc(init_capacity, sizeof(StackElem));
    if (variable == NULL) {return NOT_ALLOCATE;}

    variable->capacity = init_capacity;

    return stack_verity(variable);
}

int stack_verity(stack_t* variable)
{
    enum Errors res = ALL_OKAY;
    FILE* log = fopen("mimilogs.log", "a");

    if (variable == NULL) {res = NULL_ADRESS; }
    if (variable->stack == NULL) {res = NULL_ADRESS;}
    if (variable->capacity == 0) {res = NULL_CAPACITY;}
    if (variable->size > variable->capacity) {res = INCORRECT_SIZE;}

    if (res != ALL_OKAY) {STACK_DUMP(log, variable, res);}

    fclose(log);
    return res;
}

int stack_push(stack_t* variable, StackElem elem)
{
    int res_of_verity = stack_verity(variable);
    if (res_of_verity != ALL_OKAY) {return res_of_verity;}

    if (variable->size < variable->capacity)
    {
        variable->stack[variable->size] = elem;
        variable->size += 1;
    }

    else 
    {
        realloc_up(variable);
        variable->stack[variable->size] = elem;
        variable->size += 1;
    }

    return ALL_OKAY;
}

StackElem stack_pop(stack_t* variable)
{
    int res_of_verity = stack_verity(variable);
    if (res_of_verity != ALL_OKAY) {return res_of_verity;}

    StackElem elem = STACK_ELEM_INIT;

    if (variable->size > 0)
    {
        if (variable->size == variable->capacity / (MULTIP * MULTIP))
        {
            realloc_down(variable);
        }
        
        else
        {
            elem = variable->stack[variable->size - 1];
            variable->stack[variable->size - 1] = STACK_ELEM_INIT;
            variable->size -= 1;
        }
    }

    return elem;
}

int realloc_up(stack_t* variable)
{
    int res_of_verity = stack_verity(variable);
    if (res_of_verity != ALL_OKAY) {return res_of_verity;}

    variable->stack = (StackElem*) realloc(variable->stack, variable->capacity * MULTIP * sizeof(StackElem));
    if (variable->stack == NULL) {return NOT_ALLOCATE;}

    for (size_t i = 0; i < variable->capacity * (MULTIP - 1); i++)
    {
        *(variable->stack + (variable->capacity + i)) = STACK_ELEM_INIT;
    }

    variable->capacity = variable->capacity * MULTIP;
    return ALL_OKAY;
}

int realloc_down(stack_t* variable)
{
    int res_of_verity = stack_verity(variable);
    if (res_of_verity != ALL_OKAY) {return res_of_verity;}

    variable->capacity = variable->capacity / MULTIP;

    variable->stack = (StackElem*) realloc(variable->stack, variable->capacity * sizeof(StackElem));
    if (variable->stack == NULL) {return NOT_ALLOCATE;}

    variable->stack[variable->size - 1] = 0;
    variable->size -= 1;

    return ALL_OKAY;
}


void print_arr(StackElem* arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%lg  ", arr[i]);
    }
    printf("\n");
}
