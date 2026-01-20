#include <stdio.h>
#include "astr.h"
#include "astr_test.h"

void test_astrlen(void)
{
    char s[] = "Hello";
    printf("astrlen: %d\n", astrlen(s));
}
