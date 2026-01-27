#include "functions.h"
#include <stdio.h>

void trng(uint8_t *rand, int len)
{
    FILE *fp = fopen("/dev/urandom", "rb");
    if (!fp) return;

    fread(rand, 1, len, fp);
    fclose(fp);
}
