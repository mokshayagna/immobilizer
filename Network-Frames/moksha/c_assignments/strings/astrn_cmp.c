#include "astr.h"

char* astrncmp(char *s1, char *s2, int n)
{
    int i = 0;

    while (i < n) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }

        if (s1[i] == '\0') {
            return 0;
        }

        i++;
    }

    return 0;
}
