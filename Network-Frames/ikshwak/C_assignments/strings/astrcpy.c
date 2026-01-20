char* astrcpy(char *s, char *t){

    int i = 0;
    while(t[i]!='\0'){
        s[i] = t[i];
        i++;
    }
    s[i] = '\0';
    return s;
}