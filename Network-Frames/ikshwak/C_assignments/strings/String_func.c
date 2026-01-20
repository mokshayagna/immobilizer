#include<stdio.h>

int astrlen(char *c){
    int i = 0;
    while(c[i] != '\0'){
        i++;
    }
    return i;
}

int astrcmp(char *s, char *t){
    
    int i = 0;
    while(s[i]!='\0' && t[i]!='\0' ){
        if(s[i]!=t[i]){
            return -1;
        }
        i++;
    }
    return 0;
}

int astrncmp(char *s, char *t, int n){

    int i = 0;
    while(i<n && s[i]!='\0' && t[i]!='\0'){
        if(s[i]!=t[i]){
            return -1;
        }
        i++;
    }
    if(i<n){
        return -1;
    }
    return 0;
}

char* astrcpy(char *s, char *t){

    int i = 0;
    while(t[i]!='\0'){
        s[i] = t[i];
        i++;
    }
    s[i] = '\0';
    return s;
}

char* astrncpy(char *s, char *t, int n){
    
    int i = 0;

    while(i<n && t[i]!='\0'){
        s[i] = t[i];
        i++;
    }
    s[i] = '\0';
    return s;
}

char *astrcat(char *s, char *t){

    int i = 0;
    int j = 0;
    while(s[i]!='\0'){
        i++;
    }
    while(t[j]!='\0'){
        s[i] = t[j];
        i++;
        j++;
    }
    s[i] = '\0';
    return s;
}

char* astrncat(char *s, char *t, int n){
    int i = 0;
    int j = 0;
    while(s[i]!='\0'){
        i++;
    }
    while(j<n && t[j]!='\0'){
        s[i] = t[j];
        i++;
        j++;
    }
    s[i] = '\0';
    return s;
}





int main(){
    char s[20] = "Hello";
    char t[] = "world";
}