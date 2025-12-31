#include <stdio.h>
#include <stdint.h>
#include <string.h>

void calculate_final_result(uint8_t *random_input, uint8_t *final_output);

int main()
{
    uint8_t input[16];
    uint8_t output[16];

    printf("Enter 16 bytes of input (in hex, e.g., 00112233445566778899AABBCCDDEEFF):\n");
    for (int i = 0; i < 16; i++)
    {
        unsigned int val;
        scanf("%2x", &val);   // Read each byte as hex
        input[i] = (uint8_t)val;
    }

    calculate_final_result(input, output);

    printf("\nInput:\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", input[i]);

    printf("\n\nAES Output:\n");
    for (int i = 0; i < 16; i++)
        printf("%02X ", output[i]);

    printf("\n");
    return 0;
}
