#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG
#ifdef DEBUG 
#define STACK_DUMP(log, variable, error) stack_dump(log, variable, error, __FILE__, __FUNCTION__, __LINE__)
#else
#define STACK_DUMP(log, variable) ((void)0)
#endif


#define STACK_ELEM_INIT 0;

#define FREE_MASS(array)                                \
    free(array);                                        \
    array = NULL;   


typedef double StackElem;
const int MULTIP = 2;

enum Errors
{
    ALL_OKAY,
    NOT_ALLOCATE,
    NULL_CAPACITY,
    NULL_ADRESS,
    INCORRECT_SIZE,
};

struct stack_t 
{
    StackElem* stack;
    size_t size;
    size_t capacity;
};

int realloc_up(stack_t* variable);
int stack_initialise(stack_t* variable, size_t init_capacity);
int stack_verity(stack_t* variable);
int stack_push(stack_t* variable, StackElem elem);
StackElem stack_pop(stack_t* variable);
void print_arr(StackElem* arr, size_t size);
int realloc_down(stack_t* variable);
int stack_dump(FILE* log, stack_t* variable, Errors error, const char* file_name, const char* func_name, int line);