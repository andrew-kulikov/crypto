import random
from math_utils import generate_prime_number, primitive_root
from crypto import encrypt_bit, encrypt, decrypt_bit, decrypt


def main():
    p = generate_prime_number()
    g = primitive_root(p)
    x = random.randint(2, p - 2)  # private key
    y = (g ** x) % p

    public_key = (p, g, y)
    private_key = x

    source = 44

    crypto_pair = encrypt_bit(source, p, g, y)
    decrypted_value = decrypt_bit(crypto_pair, public_key, private_key)

    print('Source bit: ', source)
    print('Crypted pair: ', crypto_pair)
    print('Decrypted value: ', decrypted_value)

    source = """To be, or not to be--that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune
Or to take arms against a sea of troubles
And by opposing end them. To die, to sleep--
No more--and by a sleep to say we end
The heartache, and the thousand natural shocks
That flesh is heir to."""
    encrypted_text = encrypt(source, public_key)
    decrypted_text = decrypt(encrypted_text, public_key, private_key)

    print('Source text: ', source)
    print('Encrypted text:', encrypted_text)
    print('Decrypted text:', ''.join(decrypted_text))


if __name__ == '__main__':
    main()
