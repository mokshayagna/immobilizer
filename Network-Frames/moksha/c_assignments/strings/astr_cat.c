#include "astr.h"

char* astrcat(char *s, char *t)
{
    int i = 0, j = 0;

    while (s[i] != '\0')
        i++;

    while (t[j] != '\0') {
        s[i++] = t[j++];
    }

    s[i] = '\0';
    return s;
}
