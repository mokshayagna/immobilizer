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