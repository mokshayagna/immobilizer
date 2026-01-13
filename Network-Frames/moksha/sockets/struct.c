#include<stdio.h>

struct student {
    char k;
    char j;
    short int i;
};
int main(){
    struct student s1;
    printf("size of s1:%lu\n",sizeof(s1));
   // printf("base add of s1.i:%p\n",&s1.i);
    
    printf("base add of s1.c:%p\n",&s1.k);
    printf("base add of s1.c:%p\n",&s1.j);
    printf("base add of s1.shi:%p\n",&s1.i);
    return 0;
}
