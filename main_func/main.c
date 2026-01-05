#include <stdio.h>
#include <stdint.h>
#include "logic.h"

int main() {
    uint8_t random_number[16];
    trng(random_number, 16);

    printf("Random Challenge:\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", random_number[i]);
    printf("\n");

    uint8_t secret_key[16] = {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
    };

    uint8_t output[16];

    /* ✅ FIXED ORDER */
    aes128_encrypt(random_number, output, secret_key);

    printf("Output of AES:\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", output[i]);
    printf("\n");

    printf("done\n");
    return 0;
}
