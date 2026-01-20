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