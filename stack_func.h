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

#define FNV_BASIS 2166136261u
#define FNV_PRIME 16777619u

#define CANARY_1 0xBEDA1
#define CANARY_2 0xBEDA2

typedef int stack_elem_t;
typedef uint16_t error_t;
const int FACTOR = 2;

enum stack_err_codes_t
{
    STK_ALL_OKAY            = 0,
    STK_NOT_REALLOCATE      = 1,
    STK_NULL_CAPACITY       = 2,
    STK_NULL_ADRESS         = 4,
    STK_INCORRECT_SIZE      = 8,
    STK_OUT_OF_BOUNDS       = 16,
    STK_BAD_STACK           = 32,
    STK_STRUCT_DAMAGE       = 64
};

enum stack_err_t
{
    SUCCESS = 0,
    ERROR   = 1
};

struct stack_t 
{
    stack_elem_t canary_begin;

    stack_elem_t* data;
    size_t size;
    size_t capacity;
    error_t error;

    stack_elem_t canary_end;

    #ifdef DEBUG
    stack_elem_t* data_copy;
    uint32_t hash;
    #endif
};

stack_err_t stack_realloc_up(stack_t* variable);
stack_err_t stack_initialise(stack_t* variable, size_t init_capacity);
stack_err_t stack_verify(stack_t* variable);
stack_err_t stack_push(stack_t* variable, stack_elem_t elem);
stack_err_t stack_pop(stack_t* variable, stack_elem_t* elem);
stack_err_t stack_realloc_down(stack_t* variable);
int stack_dump(FILE* log, stack_t* variable, const char* file_name, const char* func_name, int line);
stack_err_t stack_destroy(stack_t* variable);
int is_equal_digits(stack_elem_t elem_1, stack_elem_t elem_2);
void print_arrays(FILE* log, stack_elem_t* array, stack_t* variable);
uint32_t hash_bytes(stack_t* variable);

#endif