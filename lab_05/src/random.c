#include "random.h"

static bool srand_called = false;

double gen_random_less_than(double top)
{
    if (!srand_called)
    {
        srand_called = true;
        srand(time(NULL));
    }

    return ((double)rand() / (double)(RAND_MAX)) * top;
}
