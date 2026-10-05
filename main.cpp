#include "stack_func.h"
#include "stack_tests.h"

int main()
{
    stack_t variables[10];
    size_t init_size = 2;
    FILE* log = fopen("mimilogs.log", "a");

    for (size_t i = 0; i < 10; i++)
    {
        int init_res = stack_initialise(&variables[i], init_size);
        if (init_res != SUCCESS) {STACK_DUMP(log, &variables[i]); abort();}
    }

    test_stack(log, variables);

    for (size_t i = 0; i < 10; i++)
    {
        stack_destroy(&variables[i]);
    }
        
    fclose(log);
}

