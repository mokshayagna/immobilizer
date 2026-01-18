#include "astring.h"
#include <stdio.h>

int test_astrcmp()
{

    char s1[] = "hello";
    char s2[] = "hello";
    char s3[] = "hellO";

    if(astrcmp(s1, s2) != 0){
        printf("astrcmp tests failed\n");
        return 1; // Test failed
    }

    if(astrcmp(s1, s3) == 0){
        printf("astrcmp tests failed\n");
        return 1; // Test failed
    }

    printf("astrcmp tests passed\n");
    return 0; // All tests passed
}

int test_astrlen()
{
    char s1[] = "hello";
    char s2[] = "";
    char s3[] = "C programming";

    if(astrlen(s1) != 5){
        printf("astrlen tests failed\n");
        return 1; // Test failed
    }

    if(astrlen(s2) != 0){
        printf("astrlen tests failed\n");
        return 1; // Test failed
    }

    if(astrlen(s3) != 13){
        printf("astrlen tests failed\n");
        return 1; // Test failed
    }

    printf("astrlen tests passed\n");
    return 0; // All tests passed
}