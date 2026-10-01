#include "stack_func.h"
#include "stack_tests.h"

//TODO - top(), stack_empty() empty нужно в initialise 
//TODO - чек валидность указателя, но не NULL

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

    fclose(log);

    for (size_t i = 0; i < 10; i++)
    {
        stack_destroy(&variables[i]);
    }
}

