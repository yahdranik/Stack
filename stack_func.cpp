#include "stack_func.h"
#include <cmath>
#include <cstdio>

int stack_initialise(stack_t* variable, size_t init_capacity)
{
    if (variable == NULL) {return ERROR;}
    if (init_capacity < 1) {variable->error = STK_NULL_CAPACITY; return ERROR;}

    variable->stack = (stack_elem_t*) calloc(init_capacity + 2, sizeof(stack_elem_t));
    if (variable->stack == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}

    for (size_t i = 0; i < init_capacity + 2; i++) {variable->stack[i] = STACK_ELEM_INIT;}

    variable->stack[0] = CANARY_1;
    variable->stack[init_capacity + 1] = CANARY_2;

    variable->canary_begin = CANARY_1;
    variable->canary_end = CANARY_2;

    variable->capacity = init_capacity;
    variable->size = 0;
    variable->error = STK_ALL_OKAY;

    #ifdef DEBUG
    variable->stack_copy = (stack_elem_t*) calloc(init_capacity + 2, sizeof(stack_elem_t));
    if (variable->stack_copy == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}
    
    variable->hash = 0;
    variable->hash = hash_bytes(variable);

    memcpy(variable->stack_copy, variable->stack, (variable->capacity + 2) * sizeof(stack_elem_t));
    #endif

    return SUCCESS;
}

int stack_destroy(stack_t* variable)
{
    if (variable->stack == NULL) {return ERROR;}

    variable->size = NAN;
    variable->capacity = NAN;
    free(variable->stack);
    variable->stack = NULL;

    #ifdef DEBUG 
    free(variable->stack_copy);
    variable->stack_copy = NULL;
    #endif

    return SUCCESS;
}

int stack_verify(stack_t* variable)
{
// Adresses and parametres ---------------------------------------------------------------------------
    if (variable == NULL) {variable->error = STK_NULL_ADRESS; return ERROR;}
    else if (variable->stack == NULL) {variable->error = STK_NULL_ADRESS; return ERROR;}
    else if (variable->capacity < 1) {variable->error = STK_NULL_CAPACITY; return ERROR;}
    else if (variable->size > variable->capacity) {variable->error = STK_INCORRECT_SIZE; return ERROR;}

// Canary --------------------------------------------------------------------------------------------
    else if (!is_equal_digits(variable->stack[0], CANARY_1) ||
        !is_equal_digits(variable->stack[variable->capacity + 1], CANARY_2) ||
        !is_equal_digits(variable->canary_begin, CANARY_1) ||
        !is_equal_digits(variable->canary_end, CANARY_2))
    {
        variable->error = STK_OUT_OF_BOUNDS;
        return ERROR;
    }

// Stack values ---------------------------------------------------------------------------------------
    else if (memcmp(variable->stack, variable->stack_copy, (variable->capacity + 2) * sizeof(stack_elem_t)) != 0)
    {
        variable->error = STK_BAD_STACK;
        return ERROR;
    }

// Hashes on struct -----------------------------------------------------------------------------------
    uint32_t current_hash = variable->hash;
    variable->hash = 0;
    uint32_t calculated_hash = hash_bytes(variable);
    variable->hash = current_hash;

    if (current_hash != calculated_hash) {variable->error = STK_STRUCT_DAMAGE; return ERROR;}

    return SUCCESS;
}

int stack_push(stack_t* variable, stack_elem_t elem)
{
    #ifdef DEBUG
    if(stack_verify(variable)) {return ERROR;}
    #endif 

    if (variable->size == variable->capacity)
    {
        stack_realloc_up(variable);
    }

    variable->stack[variable->size + 1] = elem;
    variable->size += 1;

    #ifdef DEBUG
    memcpy(variable->stack_copy, variable->stack, (variable->capacity + 2) * sizeof(stack_elem_t));
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

int stack_pop(stack_t* variable, stack_elem_t* elem)
{
    #ifdef DEBUG
    if(stack_verify(variable)) {return ERROR;}
    #endif

    if (variable->size > 0)
    {
        if (variable->size == variable->capacity / (FACTOR * FACTOR))
        {
            stack_realloc_down(variable);
        }
        
        *elem = variable->stack[variable->size];
        variable->stack[variable->size] = STACK_ELEM_INIT;
        variable->size -= 1;
    }

    #ifdef DEBUG
    memcpy(variable->stack_copy, variable->stack, (variable->capacity + 2) * sizeof(stack_elem_t));
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

int stack_realloc_up(stack_t* variable)
{
    #ifdef DEBUG
    if(stack_verify(variable)) {return ERROR;}
    #endif

    variable->stack = (stack_elem_t*) realloc(variable->stack, (variable->capacity * FACTOR + 2) * sizeof(stack_elem_t));
    if (variable->stack == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}

    #ifdef DEBUG
    variable->stack_copy = (stack_elem_t*) realloc(variable->stack_copy, (variable->capacity * FACTOR + 2) * sizeof(stack_elem_t));
    if (variable->stack_copy == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}
    #endif

    for (size_t i = 0; i < variable->capacity * (FACTOR - 1) + 1; i++)
    {
        variable->stack[variable->capacity + 1 + i] = STACK_ELEM_INIT;
    }

    variable->capacity = variable->capacity * FACTOR;
    variable->stack[variable->capacity + 1] = CANARY_2;

    #ifdef DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

int stack_realloc_down(stack_t* variable)
{
    #ifdef DEBUG
    if(stack_verify(variable)) {return ERROR;}
    #endif

    variable->capacity = variable->capacity / FACTOR;

    variable->stack = (stack_elem_t*) realloc(variable->stack, (variable->capacity + 2) * sizeof(stack_elem_t));
    if (variable->stack == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}

    variable->stack[variable->capacity + 1] = CANARY_2;

    #ifdef DEBUG
    variable->stack_copy = (stack_elem_t*) realloc(variable->stack_copy, (variable->capacity + 2) * sizeof(stack_elem_t));
    if (variable->stack_copy == NULL) {variable->error = STK_NOT_ALLOCATE; return ERROR;}
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif
    return SUCCESS;
}

int stack_dump(FILE* log, stack_t* variable, const char* file_name, const char* func_name, int line)
{
    stack_err_codes_t error = variable->error;

    if (error == STK_ALL_OKAY) {fprintf(log, "OK!\n");}
    if (error == STK_NOT_ALLOCATE) {fprintf(log, "FATAL: Memory don`t allocate!\n");}
    if (error == STK_OUT_OF_BOUNDS) {fprintf(log, "FATAL: Programm is out of bounds!\n");}
    if (error == STK_NULL_CAPACITY) {fprintf(log, "ERROR: The capacity of stack is NULL!\n");}
    if (error == STK_NULL_ADRESS) {fprintf(log, "ERROR: The adress is NULL!\n");}
    if (error == STK_INCORRECT_SIZE) {fprintf(log, "ERROR: The size of stack incorrect!\n");}
    if (error == STK_BAD_STACK) {fprintf(log,"ERROR: stack is spoiled!\n");}
    if (error == STK_STRUCT_DAMAGE) {fprintf(log,"ERROR: struct was damage!\n");}

    fprintf(log, "Dump was summoned in\n"
                 "      file: <%s>\n"       
                 "      function: <%s>\n"      
                 "      line: <%d>\n", file_name, func_name, line);

    fprintf(log, "\nCurrent parametres:\n"
                 "      canary_begin: %X\n"
                 "      struct: variable[%p]\n"
                 "      stack: variable -> stack[%p]\n"
                 "      stack_copy: variable -> stack_copy[%p]\n"
                 "      size: <%zu>\n"
                 "      capacity: <%zu>\n"
                 "      canary_end: %X\n",
                variable->canary_begin, variable, variable->stack, variable->stack_copy,
                variable->size, variable->capacity, variable->canary_end);

    if (error == STK_NOT_ALLOCATE)
    { 
        fflush(log);
        abort();
    }

    else if (error != STK_NULL_CAPACITY && error != STK_NULL_ADRESS)
    {
        fprintf(log, "\nORIGINAL stack:\n");
        fprintf(log, "-------------------------------------------------\n");
        print_arrays(log, variable->stack, variable);
        fprintf(log, "-------------------------------------------------\n");

        fprintf(log, "\nCOPY stack:\n");
        fprintf(log, "-------------------------------------------------\n");
        print_arrays(log, variable->stack_copy, variable);
        fprintf(log, "-------------------------------------------------\n");
    }

    if (error == STK_OUT_OF_BOUNDS) {fflush(log); abort();}
    
    fflush(log);
    return 0;
}

int is_equal_digits(stack_elem_t elem_1, stack_elem_t elem_2)
{
    return  (abs(elem_1 - elem_2) < 1e-9);
}

void print_arrays(FILE* log, stack_elem_t* array, stack_t* variable)
{
    if (variable->error == STK_INCORRECT_SIZE)
    {
        for (size_t i = 0; i < variable->capacity + 2; i++)
        {
            fprintf(log, "[%zu]  -  <%X>\n", i, array[i]);
            if (is_equal_digits(array[i], CANARY_2)) {break;}
        }
    }

    else
    {
        for (size_t i = 0; i < variable->capacity + 2; i++)
        {
            if (i > 0 && i <= variable->size)
            {
                fprintf(log, "[%zu]  -  <" MY_SP ">\n", i, array[i]);
            }

            else
            {
                fprintf(log, "[%zu]  -  <%X>\n", i, array[i]);
            }

            if (is_equal_digits(array[i], CANARY_2)) {break;}
        }
    }
}

uint32_t hash_bytes(stack_t* variable)
{
    uint8_t* array = (uint8_t*) variable;
    uint32_t hash = FNV_CONST;
    for (size_t i = 0; i < sizeof(stack_t); i++)
    {
        hash ^= array[i];
        hash *= FNV_PRIME;
    }

    return hash;
}