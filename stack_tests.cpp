#include "stack_tests.h"
#include "stack_func.h"
#include <cstdio>


void test_stack(FILE* log, stack_t* variables)
{
    test_create_push_pop(log, &variables[1]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_create_push_pop PASSED\n");
    fprintf(log, "=================================================\n");
    test_null_capacity(log, &variables[2]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_null_capacity PASSED\n");
    fprintf(log, "=================================================\n");
    test_null_adress(log, &variables[3]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_null_adress PASSED\n");
    fprintf(log, "=================================================\n");
    test_incorrect_size(log, &variables[4]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_incorrect_size PASSED\n");
    fprintf(log, "=================================================\n");
    test_realloc_up_down(log, &variables[5]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_realloc_up_down PASSED\n");
    fprintf(log, "=================================================\n");
    test_bad_values(log, &variables[6]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_bad_values PASSED\n");
    fprintf(log, "=================================================\n");
    test_obosr_stack(log, &variables[7]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_obosr_stack PASSED\n");
    fprintf(log, "=================================================\n");
    test_obosr_canary(log, &variables[8]);
    fprintf(log, "\n=================================================\n");
    fprintf(log, "test_obosr_canary PASSED\n");
    fprintf(log, "=================================================\n");
}

void test_create_push_pop(FILE* log, stack_t* variable)
{
    int error = SUCCESS;
    stack_elem_t value = STACK_ELEM_INIT;

    error = stack_push(variable, 67);
    if (error != SUCCESS) {STACK_DUMP(log, variable);}
    STACK_DUMP(log, variable);

    error = stack_pop(variable, &value);
    if (error != SUCCESS) {STACK_DUMP(log, variable);}
    STACK_DUMP(log, variable);
}

void test_null_capacity(FILE* log, stack_t* variable)
{
    variable->capacity = 0;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}

void test_null_adress(FILE* log, stack_t* variable)
{
    variable->data = NULL;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}

void test_incorrect_size(FILE* log, stack_t* variable)
{
    variable->size = variable->capacity + 67;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}

void test_realloc_up_down(FILE* log, stack_t* variable)
{
    stack_elem_t value = STACK_ELEM_INIT;
    int error = SUCCESS;

    for (size_t i = 0; i < 10; i++) 
    {
        error = stack_push(variable, 67);
        if (error) {STACK_DUMP(log, variable); abort();}
    }

    STACK_DUMP(log, variable);

    for (size_t i = 0; i < 8; i++) 
    {
        error = stack_pop(variable, &value);
        if (error) {STACK_DUMP(log, variable); abort();}
    }

    STACK_DUMP(log, variable);
}

void test_bad_values(FILE* log, stack_t* variable)
{
    variable->size = -3;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}

void test_obosr_stack(FILE* log, stack_t* variable)
{
    variable->data[2] = 52;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}

void test_obosr_canary(FILE* log, stack_t* variable)
{
    variable->data[0] = 5252;
    variable->canary_begin = 6767;
    stack_verify(variable);
    STACK_DUMP(log, variable);
}
