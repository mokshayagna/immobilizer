/*
The project code is divided into two parts:
1. Transponder ECU
2. Engine ECU

Secret Key        = SK
Random Number     = Rnd (Challenge)
Output            = Outp
*/

/* Definitions */
#define SK_VALUE           1234    // Secret Key
#define RN_VALUE           56      // Example Random Number

#define RETRY_INTERVAL_MS  20
#define TIMEOUT_MS         2000


////////////////
// ENGINE ECU
////////////////

// AES-128 algorithm in Engine ECU (example logic)
int ase_algo_in_engine(int SK, int Rnd)
{
    int Outp1;
    Outp1 = SK + (Rnd * 100);
    return Outp1;
}

// TRNG algorithm in Engine ECU
int trng(void)
{
    // Generates random challenge
    return RN_VALUE;
}


////////////////////
// TRANSPONDER ECU
////////////////////

// AES-128 algorithm in Transponder ECU (example logic)
int ase_algo_in_trans(int SK, int Rnd)
{
    int Outp2;
    Outp2 = SK + (Rnd * 100);
    return Outp2;
}


// Delay function (placeholder)
void delay_ms(int ms)
{
    // Timer-based delay (hardware dependent)
}


////////////////
// MAIN FUNCTION
////////////////



int main(void)
{
    int challenge;
    int engine_value;
    int transponder_value;
    int time = 0;

    // Generate random challenge
    challenge = trng();

    // Engine ECU calculates its value
    engine_value = ase_algo_in_engine(SK_VALUE, challenge);

    // Wait for transponder response with retry
    while (time < TIMEOUT_MS)
    {
        // Send challenge and receive response
        transponder_value = ase_algo_in_trans(SK_VALUE, challenge);

        // If response received within 20 ms
        if (transponder_value == engine_value)
        {
            return 1;   // ENGINE ON
        }

        // No response → wait 20 ms and retry
        delay_ms(RETRY_INTERVAL_MS);
        time += RETRY_INTERVAL_MS;
    }

    // No response even after 2 seconds
    return 0;   // ENGINE OFF
}
