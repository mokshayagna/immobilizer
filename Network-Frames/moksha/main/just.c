#include <stdio.h>
#include <stdint.h>
#include "functions.h"

void print_hex(const char *label, uint8_t *data, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void prepare_challenge(uint8_t *challenge)
{
    static uint8_t counter = 1;

    // Step 1: generate random bytes
    trng(challenge, 17);
    print_hex("Generated Random", challenge, 17);

    // Step 2: put counter into first byte
    challenge[0] = counter;
    print_hex("Generated Random", challenge, 17);
    // Step 3: increment counter
    counter++;
    if (counter == 0) {   // avoid 0 if you want
        counter = 1;
    }
    
}
/* ===================== MAIN ===================== */
int main(void)
{
    uint8_t challenge[17];
    trng(challenge, 17);
    prepare_challenge(challenge);
    return 0;   // never reached
}
