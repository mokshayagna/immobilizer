#include <stdio.h>
#include "astr.h"
#include "astr_test.h"

void test_astrcmp(void)
{
    char s1[] = "Hello";
    char s2[] = "Hello";
    printf("astrcmp: %d\n", astrcmp(s1, s2));

}
