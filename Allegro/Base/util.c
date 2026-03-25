#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int util_rand_i(int lo, int hi)
{
    return lo + (rand() % (hi - lo));
}

float util_rand_f(float lo, float hi)
{
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

void must_init(bool test, const char* description)
{
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}