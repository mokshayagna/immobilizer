#include <stdio.h>

int main() {
    char sendBuff[100];

    char x[] = "OP=ADD";
    char y[] = "A=10";
    char z[] = "B=20";

    sprintf(sendBuff, "%s;%s;%s;", x, y, z);

    printf("%s\n", sendBuff);
    return 0;
}
