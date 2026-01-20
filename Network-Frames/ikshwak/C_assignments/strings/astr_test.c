#include "astring.h"
#include <stdio.h>

int test_astrlen()
{
    if (astrlen("hello") != 5) return 1;
    if (astrlen("") != 0) return 1;
    if (astrlen("C programming") != 13) return 1;

    printf("astrlen tests passed\n");
    return 0;
}

int test_astrcmp()
{
    if (astrcmp("hello", "hello") != 0) return 1;
    if (astrcmp("hello", "hellO") == 0) return 1;
    if (astrcmp("abc", "abcd") == 0) return 1;

    printf("astrcmp tests passed\n");
    return 0;
}

int test_astrncmp()
{
    if (astrncmp("hello", "hello", 5) != 0) return 1;
    if (astrncmp("hello", "hellO", 4) != 0) return 1;
    if (astrncmp("hello", "hellO", 5) == 0) return 1;

    printf("astrncmp tests passed\n");
    return 0;
}

int test_astrcpy()
{
    char dest[20];

    astrcpy(dest, "hello");
    if (astrcmp(dest, "hello") != 0) return 1;

    astrcpy(dest, "");
    if (astrcmp(dest, "") != 0) return 1;

    printf("astrcpy tests passed\n");
    return 0;
}

int test_astrncpy()
{
    char dest[20];

    astrncpy(dest, "hello", 3);
    if (astrcmp(dest, "hel") != 0) return 1;

    astrncpy(dest, "hi", 5);
    if (astrcmp(dest, "hi") != 0) return 1;

    printf("astrncpy tests passed\n");
    return 0;
}

int test_astrcat()
{
    char dest[30] = "hello";

    astrcat(dest, " world");
    if (astrcmp(dest, "hello world") != 0) return 1;

    printf("astrcat tests passed\n");
    return 0;
}

int test_astrncat()
{
    char dest[30] = "hello";

    astrncat(dest, " world", 3);
    if (astrcmp(dest, "hello wo") != 0) return 1;

    printf("astrncat tests passed\n");
    return 0;
}