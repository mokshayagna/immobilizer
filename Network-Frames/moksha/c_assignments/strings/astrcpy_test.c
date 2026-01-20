#include "astr.h"
#include <stdio.h>
#include "astr_test.h"

void test_astrcpy(void)
{
    char source[] = "Moksha";
    char destination[50];
    astrcpy(destination, source);
    printf("destination: %s\n", destination);
}