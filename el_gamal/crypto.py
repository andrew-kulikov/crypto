import random


def encrypt_bit(number, p, g, y):
    k = random.randint(2, p - 2)
    a = pow(g, k, p)
    b = number * pow(y, k, p) % p
    return a, b  # crypto pair


def decrypt_bit(crypto_pair, public_key, private_key):
    p, _, _ = public_key
    x = private_key
    a, b = crypto_pair

    decrypted = (b * a ** (p - 1 - x)) % p  # wiki формула для практических вычислений

    return decrypted


def encrypt(text, public_key):
    p, g, y = public_key
    encrypted = []
    for char in text:
        encrypted.append(encrypt_bit(ord(char), p, g, y))

    return encrypted


def decrypt(crypto_pairs, public_key, private_key):
    decrypted = []
    for crypto_pair in crypto_pairs:
        decrypted.append(chr(decrypt_bit(crypto_pair, public_key, private_key)))

    return decrypted