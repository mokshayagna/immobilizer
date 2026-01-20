#include<stdio.h>
#include"astr_test.h"
#include"astring.h"

int main(){
    test_astrlen();
    test_astrcmp();
    test_astrncmp();
    test_astrcpy();
    test_astrncpy();
    test_astrcat();
    test_astrncat();
    return 0;
}

