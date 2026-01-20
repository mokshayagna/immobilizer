#include "astr.h"

char* astrncpy(char *des,char *source, int n)
{
    int i = 0;
    while(source[i] != '\0' && i < n)
    {
        des[i] = source[i];
        i++;
    }
    des[i] = '\0';
    return des;
}

    