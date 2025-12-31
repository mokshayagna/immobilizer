#include <stdio.h>
#include <sys/random.h>
#include <errno.h>
#include <stdint.h>

int generate_trng(uint8_t *buffer, size_t length)
{
    ssize_t ret = getrandom(buffer, length, 0);
    if (ret == -1)
    {
        perror("getrandom failed");
        return -1;
    }
    return 0;
}
