#include <stdio.h>
#include "astr.h"
#include "astr_test.h"

void test_astrncmp(void){
    char str1[20] = "Hello, World!";
    char str2[20] = "Hello, Everyone!";
    int n = 8;
    int result = astrncmp(str1, str2, n);
    printf("Comparing first %d characters of \"%s\" and \"%s\": %d\n", n, str1, str2, result);
}