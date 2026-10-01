#ifndef STACK_TESTS_H
#define STACK_TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include "stack_func.h"

void test_create_push_pop(FILE* log, stack_t* variable);
void test_null_capacity(FILE* log, stack_t* variable);
void test_null_adress(FILE* log, stack_t* variable);
void test_incorrect_size(FILE* log, stack_t* variable);
void test_realloc_up_down(FILE* log, stack_t* variable);
void test_stack(FILE* log, stack_t* variables);
void test_bad_values(FILE* log, stack_t* variable);
void test_obosr_stack(FILE* log, stack_t* variable);
void test_obosr_canary(FILE* log, stack_t* variable);
void test_obosr_struct(FILE* log, stack_t* variable);

#endif