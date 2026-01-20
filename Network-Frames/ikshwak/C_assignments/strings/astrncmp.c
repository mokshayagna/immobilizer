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