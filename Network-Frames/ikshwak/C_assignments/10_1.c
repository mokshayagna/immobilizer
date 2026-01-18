#include<stdio.h>

struct node{
    int marks;
    int roll;
};

int main(){
    struct node v;
    v.marks = 90;
    v.roll = 10;

    printf("Marks = %d\n", v.marks);
    printf("Roll number = %d\n", v.roll);
    return 0;
}