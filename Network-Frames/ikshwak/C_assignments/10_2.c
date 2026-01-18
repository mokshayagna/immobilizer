#include<stdio.h>

struct node{
    int marks;
    int roll;
};

int main(){
    struct node v, t;
    v.marks = 90;
    v.roll = 10;
    t = v;

    printf("Marks = %d\n", t.marks);
    printf("Roll number = %d\n", t.roll);
    return 0;
}