#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    unsigned char rand[16];  // 128-bit random number

    fp = fopen("/dev/urandom", "rb");
    if (fp == NULL) {
        printf("Unable to open /dev/urandom\n");
        return 1;
    }

    fread(rand, 1, 16, fp);
    fclose(fp);

    printf("TRNG Output (128-bit): ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", rand[i]);
    }
    printf("\n");

    return 0;
}

