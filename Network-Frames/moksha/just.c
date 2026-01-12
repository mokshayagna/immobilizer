#include <stdio.h>
#include <string.h>

int main() {
    char a[20] = "Moksha Yagna";
    char *b;

    b = strtok(a, " ");
    while (b != NULL) {
        printf("value: %s\n", b);
        b = strtok(NULL, " ");
    }

    return 0;
}
