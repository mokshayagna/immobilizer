#include "astr.h"

char* astrncat(char *s, char *t, int n)
{
    int i =0,j=0;
    while(s[i]!='\0')
    {
        i++;        
    }
    while(t[j]!='\0' && j<n)
    {
        s[i]=t[j];
        i++;
        j++;
    }
    s[i]='\0';  
    return s;
}
