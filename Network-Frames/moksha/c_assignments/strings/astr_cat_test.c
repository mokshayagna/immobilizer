#include <stdio.h>
#include "astr.h"
#include "astr_test.h"

void test_astrcat(void)
{
    char str1[50] = "Hello, ";
    char str2[] = "World!";

    char *result = astrcat(str1, str2);
    printf("Concatenated String: %s\n", result);
}
