#include <stdio.h>
#include <string.h>

int main(){
    char str1[] = "Hello, ";
    char str2[] = "World!";
    char a[50];
    // strcpy appends str2 to str1
    
    printf("%ld\n", strlen(str1)); 
    strcpy(str1+strlen(str1),str2);
    printf("After strcpy, str1: %s\n", str1);


    
    // strcasecmp compares two strings ignoring case
    /*
    char str3[] = "HELLO, " ;
    if (strcasecmp(str1, str3) == 0) {
        printf("str1 and str3 are equal (case insensitive)\n");
    } else {
        printf("str1 and str3 are not equal\n");
    }
    */

    // strcat appends str2 to str1
    /*
    strcat(str2, str1);
    printf("After strcat, str2: %s\n", str2);
    */
    
    //strchr finds the first occurrence of a character in a string
    /*
    char *ptr = strchr(str1, 'e');
    if (ptr != NULL) {
        printf("Character 'e' found at position: %ld\n", ptr - str1);
    } else {
        printf("Character 'e' not found in str1\n");
    }
    */

    //strcmp compares two strings
    /*
    printf("%d\n", strcmp("abc", "abc")); 
    printf("%d\n", strcmp("abc", "abd"));
*/

    
    //strcpy copies str1 into a
    /*
    strcpy(a, str1);
    printf("%s\n", a);  
    */

    /*strcspn returns the length of the initial segment of str1 
    which consists entirely of characters not in str2*/
    /*
    printf("%ld\n", strcspn("abc123", "0123456789"));
    */

    //strncmp returns the first n characters of two strings
    /*
    printf("%d\n", strncmp("abcdef", "abcxyz", 3));
    */


    return 0;


}
