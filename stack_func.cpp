#include "stack_func.h"
#include <cmath>
#include <cstdio>

stack_err_t stack_initialise(stack_t* variable, size_t init_capacity)
{
    if (variable == NULL) {return ERROR;}
    if (init_capacity < 1) {variable->error |= STK_NULL_CAPACITY; return ERROR;}

    variable->data = (stack_elem_t*) calloc(init_capacity + 2, sizeof(stack_elem_t));
    if (variable->data == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}

    for (size_t i = 0; i < init_capacity + 2; i++) {variable->data[i] = STACK_ELEM_INIT;}

    #ifdef CANARY_DEBUG 
    variable->data[0] = CANARY_1;
    variable->data[init_capacity + 1] = CANARY_2;

    variable->canary_begin = CANARY_1;
    variable->canary_end = CANARY_2;
    #endif

    variable->capacity = init_capacity;
    variable->size = 0;
    variable->error = STK_ALL_OKAY;

    #ifdef SHADOW_DEBUG
    variable->data_copy = (stack_elem_t*) calloc(init_capacity + 2, sizeof(stack_elem_t));
    if (variable->data_copy == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}

    memcpy(variable->data_copy, variable->data, (variable->capacity + 2) * sizeof(stack_elem_t));   
    #endif

    #ifdef HASHES_DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

stack_err_t stack_destroy(stack_t* variable)
{
    if (variable->data == NULL) {return ERROR;}

    variable->size = NAN;
    variable->capacity = NAN;
    free(variable->data);
    variable->data = NULL;

    #ifdef DEBUG 
    free(variable->data_copy);
    variable->data_copy = NULL;
    #endif

    return SUCCESS;
}

stack_err_t stack_verify(stack_t* variable)
{
    if (variable == NULL)
        return ERROR;

    variable->error = STK_ALL_OKAY;

    if (variable->data == NULL)
        variable->error |= STK_NULL_ADRESS;

    if (variable->capacity < 1)
        variable->error |= STK_NULL_CAPACITY;

    if (variable->size > variable->capacity)
        variable->error |= STK_INCORRECT_SIZE;

#ifdef HASHES_DEBUG
    uint32_t current_hash = variable->hash;
    variable->hash = 0;
    uint16_t current_error = variable->error;

    variable->error = STK_ALL_OKAY;
    uint32_t calculated_hash = hash_bytes(variable);

    variable->error = current_error;
    variable->hash = current_hash;

    if (current_hash != calculated_hash)
        variable->error |= STK_STRUCT_DAMAGE;
#endif


    if (variable->data != NULL && variable->capacity >= 1)
    {
    #ifdef CANARY_DEBUG
        if (!is_equal_digits(variable->data[0], CANARY_1) ||
            !is_equal_digits(variable->data[variable->capacity + 1], CANARY_2) ||
            !is_equal_digits(variable->canary_begin, CANARY_1) ||
            !is_equal_digits(variable->canary_end, CANARY_2))
        {
            variable->error |= STK_OUT_OF_BOUNDS;
        }
    #endif 

    #ifdef SHADOW_DEBUG
        if (variable->data_copy != NULL &&
            (variable->error & STK_INCORRECT_SIZE) != STK_INCORRECT_SIZE)
        {
            if (memcmp(variable->data, variable->data_copy, (variable->capacity + 2) * sizeof(stack_elem_t)) != 0)
            {
                variable->error |= STK_BAD_STACK;
            }
        }
    #endif
    }


    if (variable->error != STK_ALL_OKAY)
        return ERROR;
    return SUCCESS;
}

stack_err_t stack_push(stack_t* variable, stack_elem_t elem)
{
    if(stack_verify(variable)) {return ERROR;}

    if (variable->size == variable->capacity)
    {
        stack_realloc_up(variable);
    }

    variable->data[variable->size + 1] = elem;
    variable->size += 1;

    #ifdef SHADOW_DEBUG
    memcpy(variable->data_copy, variable->data, (variable->capacity + 2) * sizeof(stack_elem_t));
    #endif 

    #ifdef HASHES_DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

stack_err_t stack_pop(stack_t* variable, stack_elem_t* elem)
{
    if(stack_verify(variable)) {return ERROR;}

    if (variable->size > 0)
    {
        if (variable->size == variable->capacity / (FACTOR * FACTOR))
        {
            stack_realloc_down(variable);
        }
        
        *elem = variable->data[variable->size];
        variable->data[variable->size] = STACK_ELEM_INIT;
        variable->size -= 1;
    }

    #ifdef SHADOW_DEBUG
    memcpy(variable->data_copy, variable->data, (variable->capacity + 2) * sizeof(stack_elem_t));
    #endif

    #ifdef HASHES_DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

stack_err_t stack_realloc_up(stack_t* variable)
{
    if(stack_verify(variable)) {return ERROR;}

    variable->data = (stack_elem_t*) realloc(variable->data, (variable->capacity * FACTOR + 2) * sizeof(stack_elem_t));
    if (variable->data == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}

    #ifdef DEBUG
    variable->data_copy = (stack_elem_t*) realloc(variable->data_copy, (variable->capacity * FACTOR + 2) * sizeof(stack_elem_t));
    if (variable->data_copy == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}
    #endif

    for (size_t i = 0; i < variable->capacity * (FACTOR - 1) + 1; i++)
    {
        variable->data[variable->capacity + 1 + i] = STACK_ELEM_INIT;
    }

    variable->capacity = variable->capacity * FACTOR;

    #ifdef CANARY_DEBUG
    variable->data[variable->capacity + 1] = CANARY_2;
    #endif

    #ifdef HASHES_DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

stack_err_t stack_realloc_down(stack_t* variable)
{
    #ifdef DEBUG
    if(stack_verify(variable)) {return ERROR;}
    #endif

    variable->capacity = variable->capacity / FACTOR;

    variable->data = (stack_elem_t*) realloc(variable->data, (variable->capacity + 2) * sizeof(stack_elem_t));
    if (variable->data == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}

    #ifdef CANARY_DEBUG
    variable->data[variable->capacity + 1] = CANARY_2;
    #endif

    #ifdef SHADOW_DEBUG
    variable->data_copy = (stack_elem_t*) realloc(variable->data_copy, (variable->capacity + 2) * sizeof(stack_elem_t));
    if (variable->data_copy == NULL) {variable->error |= STK_NOT_REALLOCATE; return ERROR;}
    #endif

    #ifdef HASHES_DEBUG
    variable->hash = 0;
    variable->hash = hash_bytes(variable);
    #endif

    return SUCCESS;
}

int stack_dump(FILE* log, stack_t* variable, const char* file_name, const char* func_name, int line)
{
    error_t error = variable->error;

    if (error == STK_ALL_OKAY) {fprintf(log, "OK!\n");}
    if ((error & STK_NOT_REALLOCATE) == STK_NOT_REALLOCATE) {fprintf(log, "FATAL: Memory don`t allocate!\n");}
    if ((error & STK_OUT_OF_BOUNDS)  == STK_OUT_OF_BOUNDS)  {fprintf(log, "FATAL: Programm is out of bounds!\n");}
    if ((error & STK_NULL_CAPACITY)  == STK_NULL_CAPACITY)  {fprintf(log, "ERROR: The capacity of stack is NULL!\n");}
    if ((error & STK_NULL_ADRESS)    == STK_NULL_ADRESS)    {fprintf(log, "ERROR: The adress is NULL!\n");}
    if ((error & STK_INCORRECT_SIZE) == STK_INCORRECT_SIZE) {fprintf(log, "ERROR: The size of stack incorrect!\n");}
    if ((error & STK_BAD_STACK)      == STK_BAD_STACK)      {fprintf(log, "ERROR: stack is spoiled!\n");}
    if ((error & STK_STRUCT_DAMAGE)  == STK_STRUCT_DAMAGE)  {fprintf(log, "ERROR: struct was damage!\n");}

    fprintf(log, "Dump was summoned in\n"
                 "      file: <%s>\n"       
                 "      function: <%s>\n"      
                 "      line: <%d>\n", 
            file_name, func_name, line);

    fprintf(log, "\nCurrent parametres:\n");

    #ifdef CANARY_DEBUG
    fprintf(log, "      canary_begin: %X\n", variable->canary_begin);
    #endif

    fprintf(log, "      struct: variable[%p]\n"
                 "      stack: variable -> stack[%p]\n"
                 "      size: <%zu>\n"
                 "      capacity: <%zu>\n", variable, variable->data, variable->size, variable->capacity);

    #ifdef SHADOW_DEBUG
    fprintf(log, "      stack_copy: variable -> stack_copy[%p]\n", variable->data_copy);
    #endif

    #ifdef CANARY_DEBUG
    fprintf(log, "      canary_end: %X\n", variable->canary_end);
    #endif

    if ((error & STK_NULL_CAPACITY) != STK_NULL_CAPACITY && (error & STK_NULL_ADRESS) != STK_NULL_ADRESS)
    {
        fprintf(log, "\nORIGINAL stack:\n");
        fprintf(log, "-------------------------------------------------\n");
        print_arrays(log, variable->data, variable);
        fprintf(log, "-------------------------------------------------\n");

    #ifdef SHADOW_DEBUG
        fprintf(log, "\nCOPY stack:\n");
        fprintf(log, "-------------------------------------------------\n");
        print_arrays(log, variable->data_copy, variable);
        fprintf(log, "-------------------------------------------------\n");
    #endif 
    }

    if ((error & STK_OUT_OF_BOUNDS) == STK_OUT_OF_BOUNDS) {fflush(log); stack_destroy(variable); abort();}
    
    fflush(log);
    return 0;
}

int is_equal_digits(stack_elem_t elem_1, stack_elem_t elem_2)
{
    return  (abs(elem_1 - elem_2) < 1e-9);
}

void print_arrays(FILE* log, stack_elem_t* array, stack_t* variable)
{
    error_t error = variable->error;
    if ((error & STK_INCORRECT_SIZE) == STK_INCORRECT_SIZE)
    {
        for (size_t i = 0; i < variable->capacity + 2; i++)
        {
            fprintf(log, "*[%*zu]  -  <%X>\n", (int) variable->capacity / 10 + 1,  i, array[i]);

            #ifdef CANARY_DEBUG
            if (is_equal_digits(array[i], CANARY_2)) break;
            #endif
        }
    }

    else
    {
        for (size_t i = 0; i < variable->capacity + 2; i++)
        {
            if (i > 0 && i <= variable->size)
            {
                fprintf(log, " [%*zu]  -  <" MY_SP ">\n", (int) variable->capacity / 10 + 1, i, array[i]);
            }

            else
            {
                fprintf(log, "*[%*zu]  -  <%X>\n", (int) variable->capacity / 10 + 1, i, array[i]);
            }

            #ifdef CANARY_DEBUG
            if (is_equal_digits(array[i], CANARY_2)) break;
            #endif
        }
    }
}

#ifdef HASHES_DEBUG
uint32_t hash_bytes(const void* variable)              //Used hash algorithm - FNV:    https://www.isthe.com/chongo/tech/comp/fnv/
{
    const uint8_t* array = (const uint8_t*) variable;
    uint32_t hash = FNV_BASIS;
    for (size_t i = 0; i < sizeof(stack_t); i++)
    {
        hash ^= array[i];
        hash *= FNV_PRIME;
    }

    return hash;
}
#endif