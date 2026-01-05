#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>

/* TRNG function */
void trng(uint8_t *rand, int len);

/* AES-128 function */
void aes128_encrypt(uint8_t *pt,uint8_t *ct,uint8_t *key);

#endif
