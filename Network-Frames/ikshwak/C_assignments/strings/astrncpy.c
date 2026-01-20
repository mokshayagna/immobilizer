char* astrncpy(char *s, char *t, int n){
    
    int i = 0;

    while(i<n && t[i]!='\0'){
        s[i] = t[i];
        i++;
    }
    s[i] = '\0';
    return s;
}