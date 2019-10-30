def encrypt_int(message: int, ekey: int, n: int) -> int:
    return pow(message, ekey, n)


def decrypt_int(cyphertext: int, dkey: int, n: int) -> int:
    return pow(cyphertext, dkey, n)
