#include<stdio.h>
#include "astr.h"
#include "astr_test.h"

void test_astrncat(void)
{
    char s[20]="Hello ";
    char t[]="World!";
    astrncat(s,t,1);
    printf("After concatenation: %s\n",s);
}