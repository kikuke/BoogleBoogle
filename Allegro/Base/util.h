#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdbool.h>

void must_init(bool test, const char* description);
int util_rand_i(int lo, int hi);
float util_rand_f(float lo, float hi);

#endif // !__UTIL_H__
