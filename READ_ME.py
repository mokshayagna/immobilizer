# The project code is divided into two parts:
# 1. Transponder ECU
# 2. Engine ECU

# Secret Key = SK
# Random Number (Challenge) = RN
# Output = ANS


#####################
# ENGINE ECU
#####################

# AES algorithm in Engine ECU
def ase_algo_in_engine(SK, RN):
    # Example calculation (not real AES)
    ANS = SK + RN * 100
    return ANS


# TRNG algorithm in Engine ECU
def trng():
    # Generates random challenge
    return RN


#####################
# TRANSPONDER ECU
#####################

# AES algorithm in Transponder ECU
def ase_algo_in_trans(SK, RN):
    # Same calculation as Engine ECU
    output = SK + RN * 100
    return output


#####################
# MAIN FUNCTION
#####################

def engine_main():
    challenge = trng()

    engine_value = ase_algo_in_engine(SK, challenge)
    transponder_value = ase_algo_in_trans(SK, challenge)

    if engine_value == transponder_value:
        return "ENGINE ON"
    else:
        return "ENGINE OFF"
