#include <stdlib.h>

void free_params(char *argv[])
{
    int i = 0;
    while (argv[i]) {
        free(argv[i]);
        i++;
    }
}
