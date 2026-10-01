#ifndef STACK_FUNC_H
#define STACK_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cstdio>
#include <stdint.h>

#define DEBUG
#ifdef DEBUG 
#define STACK_DUMP(log, variable) stack_dump(log, variable, __FILE__, __FUNCTION__, __LINE__)
#else
#define STACK_DUMP(log, variable) ((void)0)
#endif

#define STACK_ELEM_INIT 0xBEBE
#define MY_SP "%d"

#define FNV_CONST 2166136261u
#define FNV_PRIME 16777619u

#define CANARY_1 0xBEDA1
#define CANARY_2 0xBEDA2

typedef int stack_elem_t;
const int FACTOR = 2;

enum stack_err_codes_t
{
    STK_ALL_OKAY,
    STK_NOT_ALLOCATE,
    STK_NULL_CAPACITY,
    STK_NULL_ADRESS,
    STK_INCORRECT_SIZE,
    STK_OUT_OF_BOUNDS,
    STK_BAD_STACK,
    STK_STRUCT_DAMAGE
};

enum stack_err_t
{
    SUCCESS = 0,
    ERROR = 1
};

struct stack_t 
{
    stack_elem_t canary_begin;

    stack_elem_t* stack;
    size_t size;
    size_t capacity;
    stack_err_codes_t error;

    stack_elem_t canary_end;

    #ifdef DEBUG
    stack_elem_t* stack_copy;
    uint32_t hash;
    #endif
};

int stack_realloc_up(stack_t* variable);
int stack_initialise(stack_t* variable, size_t init_capacity);
int stack_verify(stack_t* variable);
int stack_push(stack_t* variable, stack_elem_t elem);
int stack_pop(stack_t* variable, stack_elem_t* elem);
int stack_realloc_down(stack_t* variable);
int stack_dump(FILE* log, stack_t* variable, const char* file_name, const char* func_name, int line);
int stack_destroy(stack_t* variable);
int is_equal_digits(stack_elem_t elem_1, stack_elem_t elem_2);
void print_arrays(FILE* log, stack_elem_t* array, stack_t* variable);
uint32_t hash_bytes(stack_t* variable);

#endif